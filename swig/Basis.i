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
// Incude cmath early on, see #622
%begin %{
#include <cmath>
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
%}

#ifdef SWIGPYTHON
%pythoncode %{

_swig_repr_default = _swig_repr
def _swig_repr(self):
  if hasattr(self,'getRepresentation'):
    return self.getRepresentation()
  else:
    return _swig_repr_default(self)

%}
#endif // WITH_SWIGPYTHON

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
#include <tensor.hpp>
#include <src/SharedObject/SharedObject.h>
#include <src/SharedObject/SharedObjectNode.h>
#include <src/Function/Argument.h>
#include <src/Basis/Basis.h>
#include <src/Basis/UnivariateBasis.h>
#include <src/Basis/MonomialeBasis.h>
#include <src/Basis/BSplineBasis.h>
#include <src/Basis/vectorUtilities.h> // Debug

//#include <src/Coefficients/Coefficients.h>

//#include <src/Function/Function.h>

#include <casadi/casadi.hpp>
%}

%include <tensor.hpp>
%include <src/SharedObject/SharedObject.h>
%include <src/SharedObject/SharedObjectNode.h>
%include <src/Function/Argument.h>
%include <src/Basis/Basis.h>
%include <src/Basis/UnivariateBasis.h>
%include <src/Basis/MonomialeBasis.h>
%include <src/Basis/BSplineBasis.h>
%include <src/Basis/vectorUtilities.h> // Debug

//%include <src/Coefficients/Coefficients.h>

//%include <src/Function/Function.h>

%template(BasisVector) std::vector< spline::Basis >;
%template(DTensor) Tensor<DM>;

