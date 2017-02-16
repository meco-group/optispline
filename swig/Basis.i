%module(package="Basis") Basis

// WORKAROUNDS BEGINS: Due to Python-related issues in casadi.i
#ifdef SWIGPYTHON
%pythonbegin %{
import casadi
%}

%{
#define SWIG_FILE_WITH_INIT
#include "casadi/casadi_numpy.hpp"
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

import casadi
swigtypeconvertor = casadi.swigtypeconvertor
swig_typename_convertor_python2cpp = casadi.swig_typename_convertor_python2cpp

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

#include <src/Coefficients/Coefficient.h>

#include <src/Function/Function.h>
#include <src/Function/Polynomial.h>
#include <src/Function/Argument.h>
#include <src/Function/Index.h>
#include <src/Function/NumericIndex.h>
#include <src/Optistack/optistack.h>

#include <src/Basis/utils/EvaluationGrid.h> // Debug

using namespace spline;
#include <casadi/casadi.hpp>
%}

#ifdef SWIGPYTHON
#define GUESTOBJECT PyObject
#elif defined(SWIGMATLAB)
#define GUESTOBJECT mxArray
#else
#define GUESTOBJECT void
#endif

%fragment("casadi_extra_decl", "header") {
  namespace casadi {
    bool to_ptr(GUESTOBJECT *p, AnyScalar** m);
    bool to_ptr(GUESTOBJECT *p, std::vector<AnyScalar>** m);
    bool to_ptr(GUESTOBJECT *p, AnyTensor** m);
    bool to_ptr(GUESTOBJECT *p, AnyVector** m);
    bool to_ptr(GUESTOBJECT *p, DT** m);
    bool to_ptr(GUESTOBJECT *p, ST** m);
    bool to_ptr(GUESTOBJECT *p, MT** m);
    bool to_ptr(GUESTOBJECT *p, spline::Index** m);
    bool to_ptr(GUESTOBJECT *p, spline::NumericIndex** m);
    bool to_ptr(GUESTOBJECT *p, spline::TensorBasis** m);
    bool to_ptr(GUESTOBJECT *p, spline::Basis** m);
    bool to_ptr(GUESTOBJECT *p, spline::Coefficient** m);
    bool to_ptr(GUESTOBJECT *p, spline::Argument** m);

    GUESTOBJECT * from_ptr(const std::vector<AnyScalar>* a);
    GUESTOBJECT * from_ptr(const AnyScalar *a);
    GUESTOBJECT * from_ptr(const AnyTensor *a);
    GUESTOBJECT * from_ptr(const AnyVector *a);
    GUESTOBJECT * from_ptr(const spline::TensorBasis *a);
    GUESTOBJECT * from_ptr(const spline::Basis *a);
    GUESTOBJECT * from_ptr(const spline::Coefficient *a);
    GUESTOBJECT *from_ptr(const DT *a);
    GUESTOBJECT *from_ptr(const ST *a);
    GUESTOBJECT *from_ptr(const MT *a);
    GUESTOBJECT *from_ptr(const spline::NumericIndex *a);
    GUESTOBJECT *from_ptr(const spline::Index *a);
    GUESTOBJECT *from_ptr(const spline::Argument *a);
  }
}


%fragment("casadi_extra", "header") {
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
    bool to_ptr(GUESTOBJECT *p, spline::Coefficient** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::Coefficient*), 0))) {
        return true;
      }

      {
        AnyTensor tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
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

    bool to_ptr(GUESTOBJECT *p, std::vector<AnyScalar>** m) {
      {
        std::vector<double> tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = AnyScalar::from_vector(*mt);
          return true;
        }
      }
      {
        std::vector<SX> tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = AnyScalar::from_vector(*mt);
          return true;
        }
      }
      {
        std::vector<MX> tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = AnyScalar::from_vector(*mt);
          return true;
        }
      }
    }

    bool to_ptr(GUESTOBJECT *p, DT** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::DT*), 0))) {
        return true;
      }

      #ifdef SWIGMATLAB
      if (mxIsDouble(p) && !mxIsSparse(p)) {
        if (m) {
          int n_dims = mxGetNumberOfDimensions(p);
          const size_t* p_dim = static_cast<const size_t*>(mxGetDimensions(p));
          std::vector<int> dim(p_dim, p_dim+n_dims);
          double* p_data = static_cast<double*>(mxGetData(p));
          std::vector<double> data(p_data, p_data+product(dim));
          **m = DT(data, dim);
        }
        return true;
      }
      #else
      #endif // SWIGMATLAB
      #ifdef SWIGPYTHON
      // 1D numpy array
      if (is_array(p) && array_is_native(p)) {

        // Make sure we have a contigous array
        int array_is_new_object;
        PyArrayObject* array;

        array = obj_to_array_contiguous_allow_conversion(p, NPY_DOUBLE, &array_is_new_object);
        if (array) {
            int n_dim = array_numdims(p);
            std::vector<int> dim(n_dim);
            for (int i=0;i<dim.size();++i) dim[i] = array_size(p,i);
            std::vector<int> dim_rev = dim;
            std::reverse(dim_rev.begin(), dim_rev.end());

            double* d = reinterpret_cast<double*>(array_data(array));
            std::vector<double> data(d, d+product(dim));
            std::vector<int> order_rev = range(n_dim);
            std::reverse(order_rev.begin(), order_rev.end());
            if (m) **m = DT(data, dim_rev).reorder_dims(order_rev);
            if (array_is_new_object) Py_DECREF(array);
            return true;
        }

        // No match
        return false;
      }
      #endif

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

      // Try first converting to a temporary DT
      {
        DT tmp, *mt=&tmp;
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

      // Try first converting to a temporary MX
      {
        MX tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }

      // Try first converting to a temporary DT
      {
        DT tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }

      return false;
    }

    bool to_ptr(GUESTOBJECT *p, AnyVector** m) {
      try {
        {
          DT tmp, *mt=&tmp;
          if(casadi::to_ptr(p, m ? &mt : 0)) {
            if (m) **m = *mt;
            return true;
          }
        }
        {
          ST tmp, *mt=&tmp;
          if(casadi::to_ptr(p, m ? &mt : 0)) {
            if (m) **m = *mt;
            return true;
          }
        }

        {
          MT tmp, *mt=&tmp;
          if(casadi::to_ptr(p, m ? &mt : 0)) {
            if (m) **m = *mt;
            return true;
          }
        }
        {
          std::vector<AnyScalar> tmp, *mt = &tmp;
          if (casadi::to_ptr(p, m ? &mt : 0)) {
            userOut() << "debug" << tmp.size() << std::endl;
            if (m) **m = vertcat(tmp);
            return true;
          }
        }
      } catch(const std::exception& e) {
        return false;
      }
      return false;
    }

    bool to_ptr(GUESTOBJECT *p, AnyTensor** m) {
      {
        DT tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }
      {
        ST tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }

      {
        MT tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }
      return false;
    }

    bool to_ptr(GUESTOBJECT *p, spline::NumericIndex** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::NumericIndex*), 0))) {
        return true;
      }
      // Integer scalar
      {
        int tmp;
        if (to_val(p, &tmp)) {
#ifdef SWIGMATLAB
          if (m) **m=tmp-1;
#else
          if (m) **m=tmp;
#endif
          return true;
        }
      }
      return false;
    }

    bool to_ptr(GUESTOBJECT *p, spline::Index** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::Index*), 0))) {
        return true;
      }
      // String scalar
      {
        std::string tmp;
        if (to_val(p, &tmp)) {
          if (m) **m=tmp;
          return true;
        }
      }
      // Integer scalar
      {
        int tmp;
        if (to_val(p, &tmp)) {
#ifdef SWIGMATLAB
          if (m) **m=tmp-1;
#else
          if (m) **m=tmp;
#endif
          return true;
        }
      }
      return false;
    }

    bool to_ptr(GUESTOBJECT *p, spline::Argument** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::Argument*), 0))) {
        return true;
      }

      // String scalar
      {
        std::string tmp;
        if (to_val(p, &tmp)) {
          if (m) **m=tmp;
          return true;
        }
      }
      return false;
    }

    GUESTOBJECT * from_ptr(const AnyScalar *a) {
      if (a->is_double()) return from_ref(static_cast<double>(*a));
      if (a->is_SX()) return from_ref(static_cast<SX>(*a));
      if (a->is_MX()) return from_ref(static_cast<MX>(*a));
#ifdef SWIGPYTHON
      return Py_None;
#endif // SWIGPYTHON
      return 0;
    }

    GUESTOBJECT * from_ptr(const std::vector<AnyScalar> *a) {
      if (AnyScalar::is_double(*a)) return from_ref(AnyScalar::as_double(*a));
      if (AnyScalar::is_SX(*a)) return from_ref(AnyScalar::as_SX(*a));
      if (AnyScalar::is_MX(*a)) return from_ref(AnyScalar::as_MX(*a));
#ifdef SWIGPYTHON
      return Py_None;
#endif // SWIGPYTHON
      return 0;
    }

    GUESTOBJECT * from_ptr(const spline::Index *a) {
#ifdef SWIGPYTHON
      return Py_None;
#endif // SWIGPYTHON
      return 0;
    }

    GUESTOBJECT * from_ptr(const spline::NumericIndex *a) {
#ifdef SWIGPYTHON
      return Py_None;
#endif // SWIGPYTHON
      return 0;
    }

    GUESTOBJECT * from_ptr(const AnyVector *a) {
      return from_ptr(static_cast<const AnyTensor*>(a));
    }

    GUESTOBJECT * from_ptr(const AnyTensor *a) {
      if (a->is_DT()) {
        DT temp = static_cast<DT>(*a);
#if SWIGMATLAB
        int n_dim = temp.n_dims();
        if (n_dim==0) return mxCreateDoubleScalar(static_cast<double>(temp.data()));
        std::vector<size_t> dim(n_dim);
        std::vector<int> dims = temp.dims();
        std::copy(dims.begin(), dims.end(), dim.begin());
        mxArray *p  = mxCreateNumericArray(n_dim, get_ptr(dim), mxDOUBLE_CLASS, mxREAL);
        spline_assert(p);
        double* d = static_cast<double*>(mxGetData(p));
        std::vector<double> nz = temp.data().nonzeros();
        std::copy(nz.begin(), nz.end(), d);
        return p;
#endif
#ifdef SWIGPYTHON
        int n_dim = temp.n_dims();
        if (n_dim==0 || n_dim==1 && temp.dims(0)==1 || n_dim==2 && temp.dims(0)==1 && temp.dims(1)==1) return PyFloat_FromDouble(static_cast<double>(temp.data()));
        std::vector<npy_intp> dim(n_dim);
        for (int i=0;i<n_dim;++i) dim[i] = temp.dims(i);
        PyObject* ret = PyArray_SimpleNew(n_dim, get_ptr(dim), NPY_DOUBLE);
        std::vector<int> order = range(n_dim);
        std::reverse(order.begin(), order.end());
        double* d = static_cast<double*>(array_data(ret));
        std::vector<double> nz = temp.reorder_dims(order).data().nonzeros();
        std::copy(nz.begin(), nz.end(), d);
        return ret;
#endif
        if (temp.n_dims()<=2) {
          DM r = temp.matrix();
          if (r.is_scalar()) return from_ref(static_cast<double>(r));
          return from_ref(r);
        }
        return from_ref(temp);
      }
      if (a->is_ST()) {
        ST temp = static_cast<ST>(*a);
        if (temp.n_dims()<=2) {
          SX r = temp.matrix();
          return from_ref(r);
        }
        return from_ref(temp);
      }
      if (a->is_MT()) {
        MT temp = static_cast<MT>(*a);
        if (temp.n_dims()<=2) {
          MX r = temp.matrix();
          return from_ref(r);
        }
        return from_ref(temp);
      }
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
    GUESTOBJECT* from_ptr(const spline::Coefficient *a) {
      return SWIG_NewPointerObj(new spline::Coefficient(*a), $descriptor(spline::Coefficient *), SWIG_POINTER_OWN);
    }

    GUESTOBJECT* from_ptr(const spline::Basis *a) {
      if (dynamic_cast<const spline::MonomialBasisNode*>(a->get())) {;
        const spline::MonomialBasisNode * b = dynamic_cast<const spline::MonomialBasisNode*>(a->get());
        return SWIG_NewPointerObj(new spline::MonomialBasis(b->shared_from_this<spline::MonomialBasis>()), $descriptor(spline::MonomialBasis *), SWIG_POINTER_OWN);
      } else if (dynamic_cast<const spline::BSplineBasisNode*>(a->get())) {
        const spline::BSplineBasisNode * b = dynamic_cast<const spline::BSplineBasisNode*>(a->get());
        return SWIG_NewPointerObj(new spline::BSplineBasis(b->shared_from_this<spline::BSplineBasis>()), $descriptor(spline::BSplineBasis *), SWIG_POINTER_OWN);
      } else {
        return SWIG_NewPointerObj(new spline::Basis(*a), $descriptor(spline::Basis *), SWIG_POINTER_OWN);
      }
    }
    GUESTOBJECT* from_ptr(const spline::TensorBasis *a) {
      return SWIG_NewPointerObj(new spline::TensorBasis(*a), $descriptor(spline::TensorBasis *), SWIG_POINTER_OWN);
    }
    GUESTOBJECT* from_ptr(const spline::Argument *a) {
      return SWIG_NewPointerObj(new spline::Argument(*a), $descriptor(spline::Argument *), SWIG_POINTER_OWN);
    }
  } // namespace casadi
 }

%import "casadi/casadi.i"
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


%casadi_template("[AnyScalar]", PREC_SXVector, std::vector< AnyScalar >)
%casadi_typemaps("AnyTensor", PREC_MX, AnyTensor)
%casadi_typemaps("AnyVector", PREC_MX, AnyVector)
%casadi_typemaps("argument", PREC_MX, spline::Argument)
%casadi_typemaps("index", PREC_MX, spline::Index)
%casadi_typemaps("index", PREC_MX, spline::NumericIndex)
%casadi_typemaps("STensor", PREC_MX, Tensor<casadi::SX>)
%casadi_typemaps("DTensor", PREC_MX, Tensor<casadi::DM>)
%casadi_typemaps("MTensor", PREC_MX, Tensor<casadi::MX>)
%casadi_typemaps("Basis", PREC_MX, spline::Basis)
%casadi_typemaps("Coefficient", PREC_MX, spline::Coefficient)
%casadi_template("[TensorBasis]", PREC_MXVector, std::vector< spline::TensorBasis >)
%casadi_template("[DTensor]", PREC_MXVector, std::vector< Tensor<casadi::DM> >)
%casadi_template("[Basis]", PREC_MXVector, std::vector< spline::Basis >)

%casadi_template("[int]", PREC_IVector, std::vector<int>)
%casadi_template("[index]", PREC_IVector, std::vector< spline::Index >)
%casadi_template("[argument]", PREC_IVector, std::vector< spline::Argument >)
%casadi_template("[double]", SWIG_TYPECHECK_DOUBLE, std::vector<double>)

%include <src/SharedObject/SharedObject.h>
%include <src/SharedObject/SharedObjectNode.h>

%include <src/Function/Argument.h>
%include <src/Function/Index.h>
%include <src/Function/NumericIndex.h>
%include <tensor.hpp>


%template(DTensor) Tensor<DM>;
%template(STensor) Tensor<SX>;
%template(MTensor) Tensor<MX>;

%template(STensorVector) std::vector< Tensor<SX> >;
%template(MTensorVector) std::vector< Tensor<MX> >;

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

#ifdef SWIGPYTHON
%rename(call) spline::Function::operator();
#endif

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

%extend Function {
  %matlabcode %{
   function varargout = subsref(self,s)
      if numel(s)==1 && strcmp(s.type,'()')
        [varargout{1:nargout}] = paren(self, s.subs);
      else
        [varargout{1:nargout}] = builtin('subsref',self,s);
      end
   end
  %}
}
}
#endif

#ifdef SWIGPYTHON
namespace spline {
%extend Function {
  %pythoncode %{
    def __call__(self, *args):
      return self.call(args)
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

#ifdef WINMAT64
%begin %{
#define UINT64_T unsigned long long int
%}
#endif
