CASADI_INCLUDE_DIR=/home/erik/Documents/casadi3.0/include/
CASADI_LIB_DIR=/home/erik/Documents/casadi3.0/casadi/

CXX = gcc-4.7


all: _Basis.so

Basis.o: Basis.cpp
	${CXX}  -fPIC -c  Basis.cpp -std=c++11 -I${CASADI_INCLUDE_DIR}

libBasis.so: Basis.o
	${CXX} -fPIC -shared -Wl,-soname,libBasis.so -o libBasis.so  Basis.o

Basis_wrap.cxx: Basis.i Basis.h
	swig -c++ -python -I${CASADI_INCLUDE_DIR} -o Basis_wrap.cxx Basis.i

_Basis.so: Basis_wrap.cxx libBasis.so
	${CXX} -fPIC -std=c++11 -c Basis_wrap.cxx -o Basis_wrap.o -I. -I${CASADI_INCLUDE_DIR}  -I/usr/include/python2.7 -I/usr/include/python2.7/numpy
	c++ -fPIC  -std=c++11 -shared Basis_wrap.o -L${CASADI_LIB_DIR} -lcasadi -ldl -lrt -Wl,-rpath,${CASADI_LIB_DIR} -L. -Wl,-R. -lBasis -lpython2.7 -o _Basis.so

clean:
	rm -f Basis.py Basis.pyc Basis_wrap.cxx Basis.o _Basis.so libBasis.so Basis_wrap.o


	rm -rf build

test: _Basis.so
	python -c "import Basis;import casadi as C;print Basis.casadi2scalar(C.DM(4))"