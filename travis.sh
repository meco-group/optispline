git clone https://github.com/meco-group/mockups.git
pushd mockups/matlab
mkdir -p build
pushd build
cmake -DCMAKE_TOOLCHAIN_FILE=/local/cmake/toolchain64.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/matlab ..
make install
popd
popd

wget https://github.com/casadi/casadi/releases/download/3.5.3/casadi-windows-matlabR2014a-v3.5.3.zip -O casadi.zip
mkdir -p /opt/casadi
unzip casadi.zip -d /opt/casadi

git clone https://github.com/jgillis/mingw-std-threads.git
export CPPFLAGS="$CPPFLAGS -I$(pwd)/mingw-std-threads"

wget https://github.com/meco-group/mockups/releases/download/Perpetual/swig_matlab.tar.gz
mkdir -p /opt/swig
tar -xf swig_matlab.tar.gz -C /opt/swig

mkdir -p build
pushd build

rm -f CMakeCache.txt
export BUILD_LANG=matlabR2014a
source /local/env.sh
export CASADI_PREFIX=/opt/casadi
export CASADI_DIR=$CASADI_PREFIX/cmake

export SWIG_HOME=/opt/swig

export SWIG_LIB=$SWIG_HOME/share/swig/3.0.11
export PATH=$SWIG_HOME/bin:$SWIG_HOME/share:$PATH

cmake -DCMAKE_TOOLCHAIN_FILE=/local/cmake/toolchain64.cmake -DCMAKE_PREFIX_PATH=/opt/matlab/cmake -DCMAKE_BUILD_TYPE=Release -DWITH_PYTHON=OFF -DWITH_MATLAB=ON /local
make
mv swig/ /local/output



