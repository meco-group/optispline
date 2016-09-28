%module(package="Basis") Basis

// WORKAROUNDS BEGINS: Due to Python-related issues in casadi.i
#ifdef SWIGPYTHON
%{
#define SWIG_FILE_WITH_INIT
#include "numpy.hpp"
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

%include "exception.i"
%exception {
  try {
    $action
   } catch(const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

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

%include "exception.i"
%import "casadi.i"
%exception {
  try {
    $action
   } catch(const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

%{
#include <tensor.hpp>
#include <src/SharedObject/SharedObject.h>
#include <src/SharedObject/SharedObjectNode.h>

#include <src/Basis/utils/CommonBasis.h>
#include <src/Basis/SubBasis.h>
#include <src/Basis/SubUnivariateBasis.h>
#include <src/Basis/SubMonomialBasis.h>
#include <src/Basis/SubBSplineBasis.h>
#include <src/Basis/Basis.h>
#include <src/Basis/UnivariateBasis.h>
#include <src/Basis/MonomialBasis.h>
#include <src/Basis/BSplineBasis.h>
#include <src/Basis/utils/vectorUtilities.h> // Debug

#include <src/Coefficients/Coefficient.h>

#include <src/Function/Function.h>
#include <src/Function/Polynomial.h>
#include <src/Function/Argument.h>

#include <casadi/casadi.hpp>
%}

%include <src/Function/Argument.h>
%template(ArgumentVector) std::vector< spline::Argument >;

%include <tensor.hpp>
%include <src/SharedObject/SharedObject.h>
%include <src/SharedObject/SharedObjectNode.h>

%include <src/Basis/utils/CommonBasis.h>
%include <src/Basis/SubBasis.h>
%include <src/Basis/SubUnivariateBasis.h>
%include <src/Basis/SubMonomialBasis.h>
%include <src/Basis/SubBSplineBasis.h>
%include <src/Basis/Basis.h>
%include <src/Basis/UnivariateBasis.h>
%include <src/Basis/MonomialBasis.h>
%include <src/Basis/BSplineBasis.h>
%include <src/Basis/utils/vectorUtilities.h> // Debug

%include <src/Coefficients/Coefficient.h>

%include <src/Function/Function.h>
%include <src/Function/Polynomial.h>

%template(BasisVector) std::vector< spline::Basis >;
%template(SubBasisVector) std::vector< spline::SubBasis >;

%template(DTensor) Tensor<DM>;
%template(STensor) Tensor<SX>;
%template(MTensor) Tensor<MX>;

