CASADI_INCLUDE_DIR=/home/erik/Documents/casadi3.0/include/
CASADI_LIB_DIR=/home/erik/Documents/casadi3.0/casadi/

CXX = gcc-4.7
MATLABSWIG = ~/Documents/swig-matlab/install


all: _Basis.so _Basis_matlab.so

Basis.o: Basis.cpp
	${CXX}  -fPIC -c  Basis.cpp -std=c++11 -I${CASADI_INCLUDE_DIR}

libBasis.so: Basis.o
	${CXX} -fPIC -shared -Wl,-soname,libBasis.so -o libBasis.so  Basis.o

Basis_wrap.cxx: Basis.i Basis.h
	swig -c++ -python -I${CASADI_INCLUDE_DIR} -o Basis_wrap.cxx Basis.i

_Basis.so: Basis_wrap.cxx libBasis.so
	${CXX} -fPIC -std=c++11 -c Basis_wrap.cxx -o Basis_wrap.o -I. -I${CASADI_INCLUDE_DIR}  -I/usr/include/python2.7 -I/usr/include/python2.7/numpy
	c++ -fPIC  -std=c++11 -shared Basis_wrap.o -L${CASADI_LIB_DIR} -lcasadi -ldl -lrt -Wl,-rpath,${CASADI_LIB_DIR} -L. -Wl,-R. -lBasis -lpython2.7 -o _Basis.so

Basis_wrap_matlab.cxx:	
	PATH=${PATH}:${MATLABSWIG}/bin/${MATLABSWIG}/share ${MATLABSWIG}/bin/swig -c++ -matlab -I${CASADI_INCLUDE_DIR} -o Basis_wrap_matlab.cxx Basis.i

_Basis_matlab.so: Basis_wrap_matlab.cxx libBasis.so
	${CXX} -D__STDC_UTF_16__ -fPIC -std=c++11 -c Basis_wrap_matlab.cxx -o Basis_wrap_matlab.o -I. -I${CASADI_INCLUDE_DIR} -I/usr/local/MATLAB/R2014a/extern/include/
	c++ -fPIC  -std=c++11 -shared Basis_wrap_matlab.o -L${CASADI_LIB_DIR} -L/usr/local/MATLAB/R2014a/bin/glnxa64/ -lcasadi -ldl -lrt -lmex -lut -lmx -leng -Wl,-rpath,${CASADI_LIB_DIR} -L. -Wl,-R. -lBasis -o BasisMEX.mexa64

clean:
	rm -f Basis.py Basis.pyc Basis_wrap.cxx Basis.o _Basis.so libBasis.so Basis_wrap.o Basis_wrap.h
	rm -f Basis_wrap_matlab.cxx Basis_wrap_matlab.o Basis_wrap_matlab.h
	rm -rf build

test: _Basis.so
	python -c "import Basis;import casadi as C;b = Basis.Basis(4);assert(b.get_degree()==4)"
	echo "addpath('/home/erik/Documents/casadi-matlab3.0/');import Basis.*;b = Basis.Basis(4);disp(b.get_degree());quit" | matlab -nodesktop -nosplash
