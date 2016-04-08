%module(package="Basis") Basis

// WORKAROUNDS BEGINS: Due to Python-related issues in casadi.i
#ifdef SWIGPYTHON
%{
#define SWIG_FILE_WITH_INIT
#include "numpy.hpp"
#define SWIG_PYTHON_CAST_MODE 1
%}

%init %{
import_array();
%}
#endif // SWIGPYTHON
// WORKAROUNDS END

%import "casadi.i"

%{
#include <Basis.h>
#include <PolynomialBasis.h>
#include <casadi/casadi.hpp>
%}

%include <Basis.h>
%include <PolynomialBasis.h>

