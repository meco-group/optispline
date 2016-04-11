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

// Renameing PYTHON 
//{% if false %}
//%{
//%remame(__eval__) Basis::evalBasis
//%}
//{% endif %}

// Renameing MATLAB	 
//{% if false %}
//%{
//%remame(f) Basis::evalBasis
//%}
//{% endif %}



%include "std_vector.i"
// Instantiate templates used by example
namespace std {
   %template(DoubleVector) vector<double>;
}


%import "casadi.i"

%{
#include <src/Basis/Basis.h>
#include <src/Basis/UnivariateBasis.h>
#include <src/Basis/MonomialeBasis.h>
#include <casadi/casadi.hpp>
%}



%include <src/Basis/Basis.h>
%include <src/Basis/UnivariateBasis.h>
%include <src/Basis/MonomialeBasis.h>
