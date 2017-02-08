if [ -z "$BUILD_LANG" ]; then
    echo "You must set BUILD_LANG to python27 or matlabR2014a"
fi

export CASADIVERSION=3.1.0
export SWIG_HOME=$DEBIAN_BINARIES/swig/2.0
export SWIG_LIB=$SWIG_HOME/share/swig/3.0.11
export PATH=$SWIG_HOME/bin:$SWIG_HOME/share:$PATH

export CASADI_PREFIX=$DEBIAN_BINARIES/$BUILD_LANG/casadi/$CASADIVERSION/
#export CC="gcc-4.7"
#export CXX="g++-4.7"
gcc --version
export CC=
export CXX=