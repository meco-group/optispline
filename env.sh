if [ -z "$BUILD_LANG" ]; then
    echo "You must set BUILD_LANG to python27 or matlabR2014a"
fi

export CASADIVERSION=00e148f
export SWIG_HOME_REL=swig/2.0
export SWIG_HOME=$DEBIAN_BINARIES/$SWIG_HOME_REL

export SWIG_LIB=$SWIG_HOME/share/swig/3.0.11
export PATH=$SWIG_HOME/bin:$SWIG_HOME/share:$PATH

export CASADI_PREFIX_REL=$BUILD_LANG/casadi/$CASADIVERSION

export CASADI_PREFIX=$DEBIAN_BINARIES/$CASADI_PREFIX_REL
export CC="gcc-4.7"
export CXX="g++-4.7"
