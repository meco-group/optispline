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

#ifdef SWIGPYTHON
#define GUESTOBJECT PyObject
#elif defined(SWIGMATLAB)
#define GUESTOBJECT mxArray
#else
#define GUESTOBJECT void
#endif
%header %{
  namespace casadi {
    bool to_ptr(PyObject *p, AnyScalar** m);
    bool to_ptr(PyObject *p, AnyTensor** m);
    PyObject * from_ptr(const AnyScalar *a);
    PyObject * from_ptr(const AnyTensor *a);
    PyObject *from_ptr(const DT *a);
    PyObject *from_ptr(const ST *a);
    PyObject *from_ptr(const MT *a);
  }
%}
%import "casadi.i"
%exception {
  try {
    $action
   } catch(const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

%include <tensor.hpp>
%include <any_tensor.hpp>


%fragment("tensortools_anyscalar", "header", fragment="casadi_aux") {
  namespace casadi {
    bool to_ptr(GUESTOBJECT *p, AnyScalar** m) {
      // Treat Null
      if (is_null(p)) return false;

      // Double scalar
      {
        double tmp;
        if (to_val(p, &tmp)) {
          if (m) **m=tmp;
          return true;
        }
      }

      // Integer scalar
      {
        int tmp;
        if (to_val(p, &tmp)) {
          if (m) **m=tmp;
          return true;
        }
      }

      // Try first converting to a temporary SX
      {
        SX tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }

      // Try first converting to a temporary MX
      {
        MX tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }
      return false;
    }

    bool to_ptr(GUESTOBJECT *p, AnyTensor** m) {
      // MX already?
      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(DT*), 0))) {
        return true;
      }

      // Try converting to a temporary ST
      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(ST*), 0))) {
        return true;
      }

      // Try converting to a temporary MT
      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(MT*), 0))) {
        return true;
      }
      return false;
    }
    
    GUESTOBJECT * from_ptr(const AnyScalar *a) {
      //if (a->is_double()) return from_ref(static_cast<double>(*a));
      //if (a->is_SX()) return from_ref(static_cast<SX>(*a));
      //if (a->is_MX()) return from_ref(static_cast<MX>(*a));
#ifdef SWIGPYTHON
      return Py_None;
#endif // SWIGPYTHON
      return 0;
    }

    GUESTOBJECT * from_ptr(const AnyTensor *a) {
      if (a->is_double()) return from_ref(static_cast<DT>(*a));
      if (a->is_SX()) return from_ref(static_cast<ST>(*a));
      if (a->is_MX()) return from_ref(static_cast<MT>(*a));
#ifdef SWIGPYTHON
      return Py_None;
#endif // SWIGPYTHON
      return 0;
    }
    
    GUESTOBJECT* from_ptr(const DT *a) {
      return SWIG_NewPointerObj(new DT(*a), $descriptor(Tensor< casadi::Matrix<double> > *), SWIG_POINTER_OWN);
    }

    GUESTOBJECT* from_ptr(const ST *a) {
      return SWIG_NewPointerObj(new ST(*a), $descriptor(Tensor< casadi::Matrix<casadi::SXElem> > *), SWIG_POINTER_OWN);
    }

    GUESTOBJECT* from_ptr(const MT *a) {
      return SWIG_NewPointerObj(new MT(*a), $descriptor(Tensor< casadi::MX > *), SWIG_POINTER_OWN);
    }
    
  } // namespace casadi
 }

%fragment("tensortools_all", "header", fragment="tensortools_anyscalar") { }


 // Define all output typemaps
%define %tensortools_output_typemaps(xName, xType...)

 // Return-by-value
%typemap(out, noblock=1, fragment="tensortools_all,casadi_all") xType, const xType {
  if(!($result = casadi::from_ref($1))) SWIG_exception_fail(SWIG_TypeError,"Failed to convert output to " xName ".");
}

// Return a const-ref behaves like return-by-value
%typemap(out, noblock=1, fragment="tensortools_all,casadi_all") const xType& {
  if(!($result = casadi::from_ptr($1))) SWIG_exception_fail(SWIG_TypeError,"Failed to convert output to " xName ".");
}

// Inputs marked OUTPUT are also returned by the function, ...
%typemap(argout,noblock=1,fragment="tensortools_all,casadi_all") xType &OUTPUT {
  %append_output(casadi::from_ptr($1));
 }

// ... and the corresponding inputs are ignored
%typemap(in, noblock=1, numinputs=0) xType &OUTPUT (xType m) {
 $1 = &m;
}

 // Enable dynamic dispatch
%typemap(typecheck, noblock=1, fragment="tensortools_all,casadi_all") xType &OUTPUT {
  $1 = casadi::to_ptr($input, static_cast< xType **>(0));
 }

// Alternative names
%apply xType &OUTPUT {xType &OUTPUT1};
%apply xType &OUTPUT {xType &OUTPUT2};
%apply xType &OUTPUT {xType &OUTPUT3};
%apply xType &OUTPUT {xType &OUTPUT4};
%apply xType &OUTPUT {xType &OUTPUT5};
%apply xType &OUTPUT {xType &OUTPUT6};

// Inputs marked INOUT are also returned by the function, ...
%typemap(argout,noblock=1,fragment="tensortools_all,casadi_all") xType &INOUT {
  %append_output(casadi::from_ptr($1));
 }

// ... but kept as inputs
%typemap(in, noblock=1, fragment="tensortools_all,casadi_all") xType &INOUT (xType m) {
  $1 = &m;
  if (!casadi::to_ptr($input, &$1)) SWIG_exception_fail(SWIG_TypeError,"Failed to convert input to " xName ".");
 }

 // ... also for dynamic dispatch
%typemap(typecheck, noblock=1, fragment="tensortools_all,casadi_all") xType& INOUT {
  $1 = casadi::to_ptr($input, static_cast< xType **>(0));
 }

// No arguments need to be freed
%typemap(freearg, noblock=1) xType& INOUT {}

// Alternative names
%apply xType &INOUT {xType &INOUT1};
%apply xType &INOUT {xType &INOUT2};
%apply xType &INOUT {xType &INOUT3};
%apply xType &INOUT {xType &INOUT4};
%apply xType &INOUT {xType &INOUT5};
%apply xType &INOUT {xType &INOUT6};

%enddef

%define %tensortools_input_typemaps(xName, xPrec, xType...)
 // Pass input by value, check if matches
%typemap(typecheck, noblock=1, precedence=xPrec, fragment="tensortools_all,casadi_all") xType {
  $1 = casadi::to_ptr($input, static_cast< xType **>(0));
 }

 // Directorout typemap; as input by value
%typemap(directorout, noblock=1, fragment="tensortools_all,casadi_all") xType {
    if (!casadi::to_val($input, &$result)) {
      %dirout_fail(SWIG_TypeError,"$type");
    }
 }

 // Pass input by value, convert argument
%typemap(in, noblock=1, fragment="tensortools_all,casadi_all") xType {
  if (!casadi::to_val($input, &$1)) SWIG_exception_fail(SWIG_TypeError,"Cannot convert input to " xName ".");
 }

 // Pass input by value, cleanup
%typemap(freearg, noblock=1) xType {}

 // Pass input by reference, check if matches
%typemap(typecheck, noblock=1, precedence=xPrec, fragment="tensortools_all,casadi_all") const xType& {
  $1 = casadi::to_ptr($input, static_cast< xType **>(0));
 }

 // Pass input by reference, convert argument
%typemap(in, noblock=1, fragment="tensortools_all,casadi_all") const xType & (xType m) {
  $1 = &m;
  if (!casadi::to_ptr($input, &$1)) SWIG_exception_fail(SWIG_TypeError,"Failed to convert input to " xName ".");
 }

 // Pass input by reference, cleanup
%typemap(freearg, noblock=1) const xType & {}
%enddef

 // Define all typemaps for a template instantiation without proxy classes
%define %tensortools_template(xName, xPrec, xType...)
%template() xType;
%tensortools_input_typemaps(xName, xPrec, xType)
%tensortools_output_typemaps(xName, %arg(xType))
%enddef

 // Define all input and ouput typemaps
%define %tensortools_typemaps(xName, xPrec, xType...)
%tensortools_input_typemaps(xName, xPrec, xType)
%tensortools_output_typemaps(xName, xType)
%enddef

%tensortools_template("[AnyScalar]", PREC_SXVector, std::vector< AnyScalar >)
%tensortools_typemaps("AnyTensor", PREC_MX, AnyTensor)

%include <src/SharedObject/SharedObject.h>
%include <src/SharedObject/SharedObjectNode.h>

%include <src/Function/Argument.h>
%template(ArgumentVector) std::vector< spline::Argument >;

%include <tensor.hpp>

%include <src/Basis/utils/CommonBasis.h>
%include <src/Basis/SubBasis.h>
%include <src/Basis/SubUnivariateBasis.h>
%include <src/Basis/SubMonomialBasis.h>
%include <src/Basis/SubBSplineBasis.h>

%template(SubBasisVector) std::vector< spline::SubBasis >;

%include <src/Basis/Basis.h>
%include <src/Basis/UnivariateBasis.h>
%include <src/Basis/MonomialBasis.h>
%include <src/Basis/BSplineBasis.h>
%include <src/Basis/utils/vectorUtilities.h> // Debug

%include <src/Coefficients/Coefficient.h>

%include <src/Function/Function.h>
%include <src/Function/Polynomial.h>

%template(BasisVector) std::vector< spline::Basis >;

%template(DTensor) Tensor<DM>;
%template(STensor) Tensor<SX>;
%template(MTensor) Tensor<MX>;
