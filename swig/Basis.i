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
//%remame(__eval__) TensorBasis::evalBasis
//%}
//{% endif %}

// Renameing MATLAB
//{% if false %}
//%{
//%remame(f) TensorBasis::evalBasis
//%}
//{% endif %}



%include "std_vector.i"


%include "exception.i"


%{
#include <tensor.hpp>
#include <src/SharedObject/SharedObject.h>
#include <src/SharedObject/SharedObjectNode.h>

#include <src/Basis/utils/CommonBasis.h>
#include <src/Basis/Basis.h>
#include <src/Basis/UnivariateBasis.h>
#include <src/Basis/MonomialBasis.h>
#include <src/Basis/BSplineBasis.h>
#include <src/Basis/TensorBasis.h>
#include <src/Basis/UnivariateBasis.h>
#include <src/Basis/MonomialBasis.h>
#include <src/Basis/BSplineBasis.h>
#include <src/Basis/utils/vectorUtilities.h> // Debug
#include <src/Basis/utils/EvaluationGrid.h> // Debug

#include <src/Coefficients/Coefficient.h>

#include <src/Function/Function.h>
#include <src/Function/Polynomial.h>
#include <src/Function/Argument.h>
#include <src/Optistack/optistack.h>

#include <casadi/casadi.hpp>
%}

#ifdef SWIGPYTHON
%header %{
  namespace casadi {
    bool to_ptr(PyObject *p, AnyScalar** m);
    bool to_ptr(PyObject *p, AnyTensor** m);
    bool to_ptr(PyObject *p, DT** m);
    bool to_ptr(PyObject *p, ST** m);
    bool to_ptr(PyObject *p, MT** m);
    bool to_ptr(PyObject *p, spline::TensorBasis** m);
    bool to_ptr(PyObject *p, spline::Basis** m);
    PyObject * from_ptr(const AnyScalar *a);
    PyObject * from_ptr(const AnyTensor *a);
    PyObject * from_ptr(const spline::TensorBasis *a);
    PyObject * from_ptr(const spline::Basis *a);
    PyObject *from_ptr(const DT *a);
    PyObject *from_ptr(const ST *a);
    PyObject *from_ptr(const MT *a);
  }
%}
#elif defined(SWIGMATLAB)
%header %{
  namespace casadi {
    bool to_ptr(mxArray *p, AnyScalar** m);
    bool to_ptr(mxArray *p, AnyTensor** m);
    bool to_ptr(mxArray *p, DT** m);
    bool to_ptr(mxArray *p, ST** m);
    bool to_ptr(mxArray *p, MT** m);
    bool to_ptr(mxArray *p, spline::TensorBasis** m);
    bool to_ptr(mxArray *p, spline::Basis** m);
    mxArray * from_ptr(const AnyScalar *a);
    mxArray * from_ptr(const AnyTensor *a);
    mxArray * from_ptr(const spline::TensorBasis *a);
    mxArray * from_ptr(const spline::Basis *a);
    mxArray *from_ptr(const DT *a);
    mxArray *from_ptr(const ST *a);
    mxArray *from_ptr(const MT *a);
  }
%}
#endif

%import "casadi.i"
%exception {
  try {
    $action
   } catch(const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}


#ifdef SWIGPYTHON
%define %tensor_helpers()
%pythoncode %{
    def __getitem__(self, s):
          ind = []
          for i in s:
            if isinstance(i, slice):
              assert i.step is None and i.stop is None and i.start is None
              ind.append(-1)
            else:
              ind.append(i)
          return self.index(ind)
%}
%enddef
#else
%define %tensor_helpers()
%matlabcode %{
   function varargout = subsref(self,s)
      if numel(s)==1 & s.type=='()'
        [varargout{1:nargout}]= index_helper(self, s.subs{:});
      else
        [varargout{1:nargout}] = builtin('subsref',self,s);
      end
   end
   function out = index_helper(self, varargin)
      args = [];
      for i=1:numel(varargin)
        if strcmp(varargin{i},':')
          args = [args -1];
        else
          args = [args varargin{i}-1];
        end
      end
      out = index(self,args);
   end
%}
%enddef
#endif

%include <tensor.hpp>


%template(DTensor) Tensor<DM>;
%template(STensor) Tensor<SX>;
%template(MTensor) Tensor<MX>;

%template(DTensorVector) std::vector< Tensor<DM> >;
%template(STensorVector) std::vector< Tensor<SX> >;
%template(MTensorVector) std::vector< Tensor<MX> >;

%fragment("tensortools_anyscalar", "header", fragment="casadi_aux") {
  namespace casadi {
    bool to_ptr(GUESTOBJECT *p, spline::Basis** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::Basis*), 0))) {
        return true;
      }
      return false;
    }
    bool to_ptr(GUESTOBJECT *p, spline::TensorBasis** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::TensorBasis*), 0))) {
        return true;
      }
      return false;
    }
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

    bool to_ptr(GUESTOBJECT *p, DT** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::DT*), 0))) {
        return true;
      }
      
      // Try first converting to a temporary DM
      {
        DM tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }

      return false;
    }
    bool to_ptr(GUESTOBJECT *p, ST** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::ST*), 0))) {
        return true;
      }
      
      // Try first converting to a temporary SX
      {
        SX tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }

      return false;
    }
    bool to_ptr(GUESTOBJECT *p, MT** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::MT*), 0))) {
        return true;
      }
      
      // Try first converting to a temporary MM
      {
        MT tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }

      return false;
    }
    bool to_ptr(GUESTOBJECT *p, AnyTensor** m) {
      {
        DT *m2;
        if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(&m2),
                                      $descriptor(DT*), 0))) {
          if (m) **m=*m2;
          return true;
        }
      }

      {
        ST *m2;
        if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(&m2),
                                      $descriptor(ST*), 0))) {
          if (m) **m=*m2;
          return true;
        }
      }

      {
        MT *m2;
        if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(&m2),
                                      $descriptor(MT*), 0))) {
          if (m) **m=*m2;
          return true;
        }
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
      if (a->is_DT()) return from_ref(static_cast<DT>(*a));
      if (a->is_ST()) return from_ref(static_cast<ST>(*a));
      if (a->is_MT()) return from_ref(static_cast<MT>(*a));
#ifdef SWIGPYTHON
      return Py_None;
#endif // SWIGPYTHON
      return 0;
    }

    GUESTOBJECT* from_ptr(const DT *a) {
      if (a->n_dims()<=2) {
        DM r = a->matrix();
        return from_ref(r);
      }
      return SWIG_NewPointerObj(new DT(*a), $descriptor(Tensor< casadi::Matrix<double> > *), SWIG_POINTER_OWN);
    }

    GUESTOBJECT* from_ptr(const ST *a) {
      if (a->n_dims()<=2) {
        SX r = a->matrix();
        return from_ref(r);
      }
      return SWIG_NewPointerObj(new ST(*a), $descriptor(Tensor< casadi::Matrix<casadi::SXElem> > *), SWIG_POINTER_OWN);
    }

    GUESTOBJECT* from_ptr(const MT *a) {
      if (a->n_dims()<=2) {
        MX r = a->matrix();
        return from_ref(r);
      }
      return SWIG_NewPointerObj(new MT(*a), $descriptor(Tensor< casadi::MX > *), SWIG_POINTER_OWN);
    }
    
    GUESTOBJECT* from_ptr(const spline::Basis *a) {
      return SWIG_NewPointerObj(new spline::Basis(*a), $descriptor(spline::Basis *), SWIG_POINTER_OWN);
    }
    GUESTOBJECT* from_ptr(const spline::TensorBasis *a) {
      return SWIG_NewPointerObj(new spline::TensorBasis(*a), $descriptor(spline::TensorBasis *), SWIG_POINTER_OWN);
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
%tensortools_typemaps("STensor", PREC_MX, Tensor<casadi::SX>)
%tensortools_typemaps("DTensor", PREC_MX, Tensor<casadi::DM>)
%tensortools_typemaps("MTensor", PREC_MX, Tensor<casadi::MX>)
%tensortools_template("[TensorBasis]", PREC_MXVector, std::vector< spline::TensorBasis >)
%tensortools_template("[Basis]", PREC_MXVector, std::vector< spline::Basis >)

%tensortools_template("[int]", PREC_IVector, std::vector<int>)
%tensortools_template("[double]", SWIG_TYPECHECK_DOUBLE, std::vector<double>)

%include <src/SharedObject/SharedObject.h>
%include <src/SharedObject/SharedObjectNode.h>

%include <src/Function/Argument.h>
%template(ArgumentVector) std::vector< spline::Argument >;

%include <tensor.hpp>

%include <src/Basis/utils/CommonBasis.h>
%include <src/Basis/Basis.h>
%include <src/Basis/UnivariateBasis.h>
%include <src/Basis/MonomialBasis.h>
%include <src/Basis/BSplineBasis.h>

%template(AnyScalarVectorVector) std::vector< std::vector< AnyScalar > >; // Debug

%include <src/Basis/TensorBasis.h>
%include <src/Basis/UnivariateBasis.h>
%include <src/Basis/MonomialBasis.h>
%include <src/Basis/BSplineBasis.h>
%include <src/Basis/utils/vectorUtilities.h> // Debug
%include <src/Basis/utils/EvaluationGrid.h> // Debug

%include <src/Coefficients/Coefficient.h>

%include <src/Function/Function.h>
%include <src/Function/Polynomial.h>
%include <src/Optistack/optistack.h>

#ifdef SWIGMATLAB
namespace spline {
%extend SharedObject {
  %matlabcode %{
    function [] = disp(self)
      disp(self.getRepresentation())
    end
  %}
 }
}
#endif

%extend Tensor<DM> {
  %tensor_helpers()
}
%extend Tensor<SX> {
  %tensor_helpers()
}
%extend Tensor<MX> {
  %tensor_helpers()
}
