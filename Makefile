CASADI_INCLUDE_DIR=/home/erik/Documents/casadi3.0/include/
CASADI_LIB_DIR=/home/erik/Documents/casadi3.0/casadi/

CXX = gcc-4.7
MATLABSWIG = ~/Documents/swig-matlab/install

SOURCES = $(shell find src/ -type f -name '*.cpp') 
OBJECTS = $(SOURCES:.cpp=.o)

OBJECTS_TEST = $(notdir $(OBJECTS))

SRCDIR = src
OBJDIR = obj
OUTDIR = bin

OBJ = $(foreach o,$(OBJECTS_TEST), $(OBJDIR)/$o)

SWIG = swig/Basis.i

# color
red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

# start:
# 	@echo ${red} $(source) ${reset} 
# 	@echo ${red} $(object_files) ${reset} 

all: dir _Basis.so _Basis_matlab.so

# %.o: %.cpp
# 	${CXX}  -fPIC -c $@ -std=c++11 -I${CASADI_INCLUDE_DIR}
# 	
# 	
$(OBJECTS_TEST): $(SOURCES)
	${CXX} -o $(OBJDIR)/$@  -fPIC -c $(filter %/$*.cpp,$^) -std=c++11 -I${CASADI_INCLUDE_DIR}

libBasis.so: $(OBJECTS_TEST)
	${CXX} -fPIC -shared -Wl,-soname,libBasis.so -o libBasis.so $(OBJ)

Basis_wrap.cxx: $(SWIG) src/Basis/Basis.h
	@echo ${green}SWIG${reset} 
	swig -c++ -python -I${CASADI_INCLUDE_DIR} -o Basis_wrap.cxx $(SWIG) 

_Basis.so: Basis_wrap.cxx libBasis.so
	@echo ${green}PYTHON${reset} 
	${CXX} -fPIC -std=c++11 -c Basis_wrap.cxx -o Basis_wrap.o -I. -I${CASADI_INCLUDE_DIR}  -I/usr/include/python2.7 -I/usr/include/python2.7/numpy
	c++ -fPIC  -std=c++11 -shared Basis_wrap.o -L${CASADI_LIB_DIR} -lcasadi -ldl -lrt -Wl,-rpath,${CASADI_LIB_DIR} -L. -Wl,-R. -lBasis -lpython2.7 -o _Basis.so

Basis_wrap_matlab.cxx: $(SWIG) src/Basis/Basis.h
	PATH=${PATH}:${MATLABSWIG}/bin/${MATLABSWIG}/share ${MATLABSWIG}/bin/swig -c++ -matlab -I${CASADI_INCLUDE_DIR} -o Basis_wrap_matlab.cxx swig/Basis.i

_Basis_matlab.so: Basis_wrap_matlab.cxx libBasis.so
	@echo ${green}MATLAB${reset} 
	${CXX} -D__STDC_UTF_16__ -fPIC -std=c++11 -c Basis_wrap_matlab.cxx -o Basis_wrap_matlab.o -I. -I${CASADI_INCLUDE_DIR} -I/usr/local/MATLAB/R2014a/extern/include/
	c++ -fPIC  -std=c++11 -shared Basis_wrap_matlab.o -L${CASADI_LIB_DIR} -L/usr/local/MATLAB/R2014a/bin/glnxa64/ -lcasadi -ldl -lrt -lmex -lut -lmx -leng -Wl,-rpath,${CASADI_LIB_DIR} -L. -Wl,-R. -lBasis -o BasisMEX.mexa64

clean:
	rm -f Basis.py Basis.pyc Basis_wrap.cxx Basis.o _Basis.so libBasis.so Basis_wrap.o Basis_wrap.h
	rm -f Basis_wrap_matlab.cxx Basis_wrap_matlab.o Basis_wrap_matlab.h
	rm -rf build

test: _Basis.so
	python -c "import Basis;import casadi as C;b = Basis.Basis(4);assert(b.get_degree()==4)"
	echo "addpath('/home/erik/Documents/casadi-matlab3.0/');import Basis.*;b = Basis.Basis(4);disp(b.get_degree());quit" | matlab -nodesktop -nosplash

dir:
	mkdir -p $(OBJDIR)

binary:
	# tar -zcvf cpp_spline.tar.gz Basis.py _Basis.so libBasis.so BasisMEX.mexa64 +Basis SwigMem.m SwigRef.m SwigStorage.m
	cp Basis.py _Basis.so libBasis.so BasisMEX.mexa64 SwigMem.m SwigRef.m SwigStorage.m ~/Dropbox/cpp_spline
	cp -R +Basis ~/Dropbox/cpp_spline