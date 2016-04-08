CASADI_INCLUDE_DIR=/home/erik/Documents/casadi3.0/include/
CASADI_LIB_DIR=/home/erik/Documents/casadi3.0/casadi/

CXX = gcc-4.7

all: _extending_casadi.so

extending_casadi.o: extending_casadi.cpp
	${CXX}  -fPIC -c  extending_casadi.cpp -std=c++11 -I${CASADI_INCLUDE_DIR}

libextending_casadi.so: extending_casadi.o
	${CXX} -fPIC -shared -Wl,-soname,libextending_casadi.so -o libextending_casadi.so  extending_casadi.o

extending_casadi_wrap.cxx: extending_casadi.i extending_casadi.hpp
	swig -c++ -python -I${CASADI_INCLUDE_DIR} -o extending_casadi_wrap.cxx extending_casadi.i

_extending_casadi.so: extending_casadi_wrap.cxx libextending_casadi.so
	${CXX} -fPIC -std=c++11 -c extending_casadi_wrap.cxx -o extending_casadi_wrap.o -I. -I${CASADI_INCLUDE_DIR}  -I/usr/include/python2.7 -I/usr/include/python2.7/numpy
	c++ -fPIC  -std=c++11 -shared extending_casadi_wrap.o -L${CASADI_LIB_DIR} -lcasadi -ldl -lrt -Wl,-rpath,${CASADI_LIB_DIR} -L. -Wl,-R. -lextending_casadi -lpython2.7 -o _extending_casadi.so

clean:
	rm -f extending_casadi.py extending_casadi.pyc extending_casadi_wrap.cxx extending_casadi.o _extending_casadi.so libextending_casadi.so extending_casadi_wrap.o


	rm -rf build

test: _extending_casadi.so
	python -c "import extending_casadi;import casadi as C;print extending_casadi.casadi2scalar(C.DM(4))"