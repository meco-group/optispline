%module(package="splines") splines

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
#ifdef WITH_STACKTRACE
%exception {
  $action
}
#else
%exception {
  try {
    $action
  } catch(const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}
#endif


// Renameing PYTHON
//{% if false %}
//%{
//%remame(__eval__) TensorBasis::evalBasis
//%}
//{% endif %}

// Renameing MATLAB
#ifdef SWIGMATLAB
%rename(eval) spline::Function::operator();
%feature("varargin","1") spline::Function::operator();
#endif //SWIGMATLAB


%include "std_vector.i"


%include "exception.i"


%feature("classAttributes","(InferiorClasses = {?casadi.DM,?casadi.SX,?casadi.MX,?DTensor,?STensor,?MTensor})") spline::Function;


//%feature("classAttributes","(InferiorClasses = {?casadi.DM,?casadi.SX,?casadi.MX})") ::Tensor<DM>;
%feature("classAttributes","(InferiorClasses = {?casadi.DM,?casadi.SX,?casadi.MX})") Tensor;


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

#include <src/Domain/Interval.h>
#include <src/Domain/Domain.h>
#include <src/Domain/TensorDomain.h>

#include <src/Function/Function.h>
#include <src/Function/Polynomial.h>
#include <src/Function/Argument.h>
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

    void interpret_NumericIndex(spline::NumericIndex& a) {
      #ifdef SWIGMATLAB
        a--;
      #endif
    }
    void interpret_NumericIndex(std::vector<spline::NumericIndex>& a) {
      #ifdef SWIGMATLAB
        for (auto& e: a) e--;
      #endif
    }

    bool to_ptr(GUESTOBJECT *p, AnyScalar** m);
    bool to_ptr(GUESTOBJECT *p, std::vector<AnyScalar>** m);
    bool to_ptr(GUESTOBJECT *p, AnyTensor** m);
    bool to_ptr(GUESTOBJECT *p, AnyVector** m);
    bool to_ptr(GUESTOBJECT *p, AnySlice** m);
    bool to_ptr(GUESTOBJECT *p, DT** m);
    bool to_ptr(GUESTOBJECT *p, ST** m);
    bool to_ptr(GUESTOBJECT *p, MT** m);
    bool to_ptr(GUESTOBJECT *p, spline::Argument** m);
    bool to_ptr(GUESTOBJECT *p, spline::TensorBasis** m);
    bool to_ptr(GUESTOBJECT *p, spline::Basis** m);

    bool to_ptr(GUESTOBJECT *p, spline::TensorDomain** m);
    bool to_ptr(GUESTOBJECT *p, spline::Domain** m);
    bool to_ptr(GUESTOBJECT *p, spline::Interval** m);

    bool to_ptr(GUESTOBJECT *p, spline::Coefficient** m);
    bool to_ptr(GUESTOBJECT *p, spline::Function** m);

    GUESTOBJECT * from_ptr(const std::vector<AnyScalar>* a);
    GUESTOBJECT * from_ptr(const AnyScalar *a);
    GUESTOBJECT * from_ptr(const AnyTensor *a);
    GUESTOBJECT * from_ptr(const AnyVector *a);
    GUESTOBJECT * from_ptr(const AnySlice *a);

    GUESTOBJECT * from_ptr(const spline::TensorBasis *a);
    GUESTOBJECT * from_ptr(const spline::Basis *a);

    GUESTOBJECT * from_ptr(const spline::TensorDomain *a);
    GUESTOBJECT * from_ptr(const spline::Domain *a);
    GUESTOBJECT * from_ptr(const spline::Interval *a);

    GUESTOBJECT * from_ptr(const spline::Coefficient *a);
    GUESTOBJECT * from_ptr(const spline::Function *a);
    GUESTOBJECT *from_ptr(const DT *a);
    GUESTOBJECT *from_ptr(const ST *a);
    GUESTOBJECT *from_ptr(const MT *a);
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

      {
        std::vector<Basis> tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }
      return false;
    }

    bool to_ptr(GUESTOBJECT *p, spline::Interval** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::Interval*), 0))) {
        return true;
      }
      {
        std::vector<AnyScalar> tmp, *mt=&tmp;
        if(casadi::to_ptr(p, &mt) && (*mt).size()==2) {
          if (m) **m = tmp;
          return true;
        }
      }
      return false;
    }
    bool to_ptr(GUESTOBJECT *p, spline::Domain** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::Domain*), 0))) {
        return true;
      }
      {
        Interval tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = tmp;
          return true;
        }
      }
      return false;
    }
    bool to_ptr(GUESTOBJECT *p, spline::TensorDomain** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::TensorDomain*), 0))) {
        return true;
      }

      {
        std::vector<Domain> tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
      }
      {
        Domain tmp, *mt=&tmp;
        if(casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = *mt;
          return true;
        }
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
        if(casadi::to_ptr(p, &mt) && (*mt).is_scalar()) {
          if (m) **m = *mt;
          return true;
        }
      }

      // Try first converting to a temporary MX
      {
        MX tmp, *mt=&tmp;
        if(casadi::to_ptr(p, &mt) && (*mt).is_scalar()) {
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
        if(casadi::to_ptr(p, &mt)) {
            for (auto& e : *mt) {
              if (!e.is_scalar()) return false;
            }
            **m = AnyScalar::from_vector(*mt);
          return true;
        }
      }
      {
        std::vector<MX> tmp, *mt=&tmp;
        if(casadi::to_ptr(p, &mt)) {
          for (auto& e : *mt) {
            if (!e.is_scalar()) return false;
          }
          if (m) **m = AnyScalar::from_vector(*mt);
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
      {
        DT tmp, *mt=&tmp;
        if(casadi::to_ptr(p, &mt) && ((*mt).is_vector())) {
          if (m) **m = *mt;
          return true;
        }
      }
      {
        ST tmp, *mt=&tmp;
        if(casadi::to_ptr(p, &mt) && ((*mt).is_vector())) {
          if (m) **m = *mt;
          return true;
        }
      }

      {
        MT tmp, *mt=&tmp;
        if(casadi::to_ptr(p, &mt) && ((*mt).is_vector())) {
          if (m) **m = *mt;
          return true;
        }
      }
      {
        std::vector<AnyScalar> tmp, *mt = &tmp;
        if (casadi::to_ptr(p, m ? &mt : 0)) {
          if (m) **m = vertcat(*mt);
          return true;
        }
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

    bool to_ptr(GUESTOBJECT *p, AnySlice** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(AnySlice*), 0))) {
        return true;
      }
      // Integer
      {
        NumericIndex tmp;
        if (to_val(p, &tmp)) {
          if (m) {
            interpret_NumericIndex(tmp);
            **m = AnySlice(tmp);
          }
          return true;
        }
      }
      // Integer vector
      {
        std::vector<NumericIndex> tmp;
        if (to_val(p, &tmp)) {
          if (m) {
            interpret_NumericIndex(tmp);
            **m = AnySlice(tmp);
          }
          return true;
        }
      }
#ifdef SWIGPYTHON
      // Python slice
      if (PySlice_Check(p)) {
        PySliceObject *r = (PySliceObject*)(p);
        if (m) {
          int start = (r->start == Py_None || PyNumber_AsSsize_t(r->start, NULL) <= std::numeric_limits<int>::min())
            ? std::numeric_limits<int>::min() : PyInt_AsLong(r->start);
          int stop = (r->stop ==Py_None || PyNumber_AsSsize_t(r->stop, NULL)>= std::numeric_limits<int>::max())
            ? std::numeric_limits<int>::max() : PyInt_AsLong(r->stop);
          int step = (r->step !=Py_None)? PyInt_AsLong(r->step) : 1;

          **m = AnySlice(start, stop, step);
        }
        return true;
      }
#endif // SWIGPYTHON

      // string
      {
        std::string tmp;
        if (to_val(p, &tmp)) {
          if (tmp!=":") return false;
          if (m) **m = AnySlice();
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

    bool to_ptr(GUESTOBJECT *p, spline::Function** m) {
      // Treat Null
      if (is_null(p)) return false;

      if (SWIG_IsOK(SWIG_ConvertPtr(p, reinterpret_cast<void**>(m),
                                    $descriptor(spline::Function*), 0))) {
        return true;
      }

      {
        AnyTensor tmp;
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

    GUESTOBJECT * from_ptr(const spline::Argument *a) {
#ifdef SWIGPYTHON
      return Py_None;
#endif // SWIGPYTHON
      return 0;
    }

    GUESTOBJECT * from_ptr(const AnySlice *a) {
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
    GUESTOBJECT* from_ptr(const spline::Function *a) {
      return SWIG_NewPointerObj(new spline::Function(*a), $descriptor(spline::Function *), SWIG_POINTER_OWN);
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

    GUESTOBJECT* from_ptr(const spline::Domain *a) {
      if (dynamic_cast<const spline::IntervalNode*>(a->get())) {;
        const spline::IntervalNode * b = dynamic_cast<const spline::IntervalNode*>(a->get());
        return SWIG_NewPointerObj(new spline::Interval(b->shared_from_this<spline::Interval>()), $descriptor(spline::Interval *), SWIG_POINTER_OWN);
      } else {
        return SWIG_NewPointerObj(new spline::Domain(*a), $descriptor(spline::Domain *), SWIG_POINTER_OWN);
      }
    }
    GUESTOBJECT* from_ptr(const spline::TensorDomain *a) {
      return SWIG_NewPointerObj(new spline::TensorDomain(*a), $descriptor(spline::TensorDomain *), SWIG_POINTER_OWN);
    }


  } // namespace casadi
 }

%import "casadi/casadi.i"

#ifdef WITH_STACKTRACE
%exception {
  $action
}
#else
%exception {
  try {
    $action
  } catch(const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}
#endif


#ifdef SWIGPYTHON
%define %tensor_helpers(arraypriority)
%pythoncode %{

    __array_priority__ = arraypriority

    def __add__(self, a) : return spline_plus(self, a)
    def __radd__(self, a) : return spline_plus(self, a)
    def __pow__(x, n): return spline_power(x, n)
    def __sub__(self, a) : return spline_minus(self, a)
    def __rsub__(self, a) : return spline_plus(-self, a)
    def __mul__(self, a) : return spline_times(self, a)
    def __rmul__(self, a) : return spline_times(self, a)

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
%define %tensor_like_helpers(arraypriority)
   %tensor_helpers(arraypriority)
%enddef


#else
%define %tensor_like_helpers(arraypriority)
%matlabcode %{
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

%define %tensor_helpers(arraypriority)
%matlabcode %{
   function varargout = subsref(self,s)
      if numel(s)==1 & s.type=='()'
        [varargout{1:nargout}]= index_helper(self, s.subs{:});
      else
        [varargout{1:nargout}] = builtin('subsref',self,s);
      end
   end
%}
%tensor_like_helpers(arraypriority)
%enddef
#endif




%casadi_template("[AnyScalar]", PREC_SXVector, std::vector< AnyScalar >)
%casadi_template("[[AnyScalar]]", PREC_SXVector, std::vector< std::vector< AnyScalar > >)
%casadi_typemaps("AnyScalar", PREC_MX, AnyScalar)
%casadi_typemaps("AnyTensor", PREC_MX, AnyTensor)
%casadi_typemaps("AnySlice", PREC_MX, AnySlice)
%casadi_template("[AnyTensor]", PREC_MX, std::vector<AnyTensor>)
%casadi_typemaps("AnyVector", PREC_MX, AnyVector)
%casadi_template("[AnyVector]", PREC_MX, std::vector<AnyVector>)
%casadi_template("[AnyVector]", PREC_MX, std::vector<AnyVector>)
%casadi_typemaps("index", PREC_MX, spline::Argument)
%casadi_template("[index]", PREC_MX, std::vector<spline::Argument >)
%casadi_typemaps("STensor", PREC_MX, Tensor<casadi::SX>)
%casadi_typemaps("DTensor", PREC_MX, Tensor<casadi::DM>)
%casadi_typemaps("MTensor", PREC_MX, Tensor<casadi::MX>)
%casadi_typemaps("Basis", PREC_MX, spline::Basis)

%casadi_typemaps("Domain", PREC_MX, spline::Domain)
%casadi_typemaps("interval", PREC_MX, spline::Interval)
%casadi_typemaps("TensorDomain", PREC_MX, spline::TensorDomain)
%casadi_template("[TensorDomain]", PREC_MXVector, std::vector< spline::TensorDomain >)
%casadi_template("[Domain]", PREC_MXVector, std::vector< spline::Domain >)
%casadi_template("[interval]", PREC_MXVector, std::vector< spline::Interval >)

%casadi_typemaps("Coefficient", PREC_MX, spline::Coefficient)
%casadi_typemaps("TensorBasis", PREC_MX, spline::TensorBasis)
%casadi_template("[TensorBasis]", PREC_MXVector, std::vector< spline::TensorBasis >)
%casadi_template("[DTensor]", PREC_MXVector, std::vector< Tensor<casadi::DM> >)
%casadi_template("[STensor]", PREC_MXVector, std::vector< Tensor<casadi::SX> >)
%casadi_template("[MTensor]", PREC_MXVector, std::vector< Tensor<casadi::MX> >)
%casadi_template("[Basis]", PREC_MXVector, std::vector< spline::Basis >)
%casadi_template("[Function]", PREC_FUNCTION, std::vector< spline::Function >)
%casadi_typemaps("Function", PREC_FUNCTION, spline::Function)
%casadi_typemaps("[Coefficient]", PREC_MX, std::vector<spline::Coefficient>)

%casadi_template("[index]", PREC_IVector, std::vector< spline::Argument >)
%casadi_template("[double]", SWIG_TYPECHECK_DOUBLE, std::vector<double>)


%typemap(in, doc="index", noblock=1, fragment="casadi_all") spline::NumericIndex {
  if (!casadi::to_val($input, &$1)) SWIG_exception_fail(SWIG_TypeError,"Failed to convert input $argnum to type ' index '.");
  interpret_NumericIndex($1);
  }

%typemap(in, doc="index", noblock=1, fragment="casadi_all") const spline::NumericIndex & (spline::NumericIndex m) {
  $1 = &m;
  if (!casadi::to_ptr($input, &$1)) SWIG_exception_fail(SWIG_TypeError,"Failed to convert input $argnum to type ' index '.");
  interpret_NumericIndex(*$1);
 }


%typemap(in, doc="[index]", noblock=1, fragment="casadi_all") const spline::NumericIndexVector & (spline::NumericIndexVector m) {
  $1 = &m;
  if (!casadi::to_ptr($input, &$1)) SWIG_exception_fail(SWIG_TypeError,"Failed to convert input $argnum to type ' [index] '.");
  interpret_NumericIndex(m);
 }

%include <src/SharedObject/SharedObject.h>
%include <src/SharedObject/SharedObjectNode.h>

%include <src/Function/Argument.h>
%include <src/Function/NumericIndex.h>

%include <tensor.hpp>
%include <slice.hpp>

%template(DTensor) Tensor<casadi::DM>;
%template(STensor) Tensor<casadi::SX>;
%template(MTensor) Tensor<casadi::MX>;

%include <src/Domain/Domain.h>
%include <src/Domain/Interval.h>
%include <src/Domain/TensorDomain.h>

%include <src/Basis/utils/CommonBasis.h>
%include <src/Basis/Basis.h>
%include <src/Basis/UnivariateBasis.h>
%include <src/Basis/MonomialBasis.h>
%include <src/Basis/BSplineBasis.h>

%include <src/Basis/TensorBasis.h>
%include <src/Basis/UnivariateBasis.h>
%include <src/Basis/MonomialBasis.h>
%include <src/Basis/BSplineBasis.h>
%include <src/Basis/utils/vectorUtilities.h> // Debug
%include <src/Basis/utils/EvaluationGrid.h> // Debug

%include <src/Coefficients/Coefficient.h>

#ifdef SWIGPYTHON
%rename(call) spline::Function::operator();
%rename(_shape) spline::Function::shape;
%rename(_vertcat) spline::spline_vertcat;
%rename(_horzcat) spline::spline_horzcat;
%rename(_blkdiag) spline::spline_blkdiag;
%pythoncode %{
  def vertcat(*args):
    return _vertcat(args)
  def horzcat(*args):
    return _horzcat(args)
  def blkdiag(*args):
    return _blkdiag(args)
%}
#endif

#ifdef SWIGMATLAB
%rename(times) spline::Function::operator*;
%feature("nonstatic") spline::Function::vertcat;
%feature("nonstatic") spline::Function::horzcat;
%feature("nonstatic") spline::Function::blkdiag;

%feature("varargin","1") spline::Function::vertcat;
%feature("varargin","1") spline::Function::horzcat;
%feature("varargin","1") spline::Function::blkdiag;

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
      if numel(s)==1 && strcmp(s(1).type,'()')
        [varargout{1:nargout}] = paren(self, s(1).subs);
      elseif numel(s)>1 && strcmp(s(1).type,'()')
        r = paren(self, s(1).subs);
        [varargout{1:nargout}] = subsref(r,s(2:end));
      else
        [varargout{1:nargout}] = builtin('subsref',self,s);
      end
   end
  function r = size(self)
    r = shape(self);
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
    @property
    def shape(self):
      return self._shape()
  %}
}
}
#endif


%extend Tensor<DM> {
  %tensor_helpers(2000.0)
}
%extend Tensor<SX> {
  %tensor_helpers(2001.0)
}
%extend Tensor<MX> {
  %tensor_helpers(2002.0)
}

namespace spline {
%extend Function {
  %tensor_like_helpers(2003.0)

#ifdef SWIGPYTHON
  %pythoncode %{
  def __getitem__(self, s):
      if isinstance(s, tuple):
        return self.slice(*s)
      return self.slice(s)
  %}
#endif

#ifdef SWIGMATLAB
  %matlabcode %{
  function out = paren(self,args)
    out = self.slice(args{:});
  end

  %}
#endif

}
}

%inline {
  namespace spline {
    AnyTensor spline_plus(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs+rhs; }
    AnyTensor spline_minus(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs-rhs; }
    AnyTensor spline_times(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs*rhs; }
    AnyTensor spline_mtimes(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs); }
    AnyTensor spline_rmtimes(const AnyTensor& lhs, const AnyTensor& rhs) { return rhs.mtimes(lhs); }
    Function spline_power(const Function& lhs, int rhs) { return lhs.pow(rhs); }
    Function spline_plus(const Function& lhs, const Function& rhs) { return lhs+rhs; }
    Function spline_minus(const Function& lhs, const Function& rhs) { return lhs-rhs; }
    Function spline_times(const Function& lhs, const Function& rhs) { return lhs*rhs; }
    Function spline_mtimes(const Function& lhs, const Function& rhs) { return lhs.mtimes(rhs); }
    Function spline_rmtimes(const Function& lhs, const Function& rhs) { return rhs.mtimes(lhs); }
    Function spline_plus(const Function& lhs, const AnyTensor& rhs) { return lhs+rhs; }
    Function spline_minus(const Function& lhs, const AnyTensor& rhs) { return lhs-rhs; }
    Function spline_times(const Function& lhs, const AnyTensor& rhs) { return lhs*rhs; }
    Function spline_mtimes(const Function& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs); }
    Function spline_rmtimes(const Function& lhs, const AnyTensor& rhs) { return lhs.rmtimes(rhs); }
    Function spline_plus(const AnyTensor& lhs, const Function& rhs) { return rhs+lhs; }
    Function spline_minus(const AnyTensor& lhs, const Function& rhs) { return (-rhs)+lhs; }
    Function spline_times(const AnyTensor& lhs, const Function& rhs) { return rhs*lhs; }
    Function spline_mtimes(const AnyTensor& lhs, const Function& rhs) { return rhs.rmtimes(lhs); }
    Function spline_rmtimes(const AnyTensor& lhs, const Function& rhs) { return rhs.mtimes(rhs); }

    Function spline_vertcat(const std::vector<spline::Function>& args) { return Function::vertcat(args); }
    Function spline_horzcat(const std::vector<spline::Function>& args) { return Function::horzcat(args); }
    Function spline_blkdiag(const std::vector<spline::Function>& args) { return Function::blkdiag(args); }

  }
}


#ifdef SWIGMATLAB
// Wrap (static) member functions
%feature("nonstatic");
namespace spline {
  %extend Function {
    static Function plus(const Function& lhs, const Function& rhs) { return lhs+rhs; }
    static Function minus(const Function& lhs, const Function& rhs) { return lhs-rhs; }
    static Function times(const Function& lhs, const Function& rhs) { return lhs*rhs; }
    static Function mtimes(const Function& lhs, const Function& rhs) { return lhs.mtimes(rhs); }
    static Function rmtimes(const Function& lhs, const Function& rhs) { return rhs.mtimes(lhs); }
    static Function power(const Function& lhs, int rhs) { return lhs.pow(rhs); }
    static Function mpower(const Function& lhs, int rhs) { spline_assert(lhs.is_scalar()); return lhs.pow(rhs); }
    static Function plus(const Function& lhs, const AnyTensor& rhs) { return lhs+rhs; }
    static Function minus(const Function& lhs, const AnyTensor& rhs) { return lhs-rhs; }
    static Function times(const Function& lhs, const AnyTensor& rhs) { return lhs*rhs; }
    static Function mtimes(const Function& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs); }
    static Function rmtimes(const Function& lhs, const AnyTensor& rhs) { return lhs.rmtimes(rhs); }
    static Function plus(const AnyTensor& lhs, const Function& rhs) { return rhs+lhs; }
    static Function minus(const AnyTensor& lhs, const Function& rhs) { return (-rhs)+lhs; }
    static Function times(const AnyTensor& lhs, const Function& rhs) { return rhs*lhs; }
    static Function mtimes(const AnyTensor& lhs, const Function& rhs) { return rhs.rmtimes(lhs); }
    static Function rmtimes(const AnyTensor& lhs, const Function& rhs) { return rhs.mtimes(rhs); }
  }
} // namespace spline
  %extend Tensor<DM> {
    static AnyTensor plus(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs+rhs; }
    static AnyTensor minus(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs-rhs; }
    static AnyTensor times(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs*rhs; }
    static AnyTensor mtimes(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs); }
    static AnyTensor rmtimes(const AnyTensor& lhs, const AnyTensor& rhs) { return rhs.mtimes(lhs); }
  }
  %extend Tensor<SX> {
    static AnyTensor plus(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs+rhs; }
    static AnyTensor minus(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs-rhs; }
    static AnyTensor times(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs*rhs; }
    static AnyTensor mtimes(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs); }
    static AnyTensor rmtimes(const AnyTensor& lhs, const AnyTensor& rhs) { return rhs.mtimes(lhs); }
  }
  %extend Tensor<MX> {
    static AnyTensor plus(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs+rhs; }
    static AnyTensor minus(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs-rhs; }
    static AnyTensor times(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs*rhs; }
    static AnyTensor mtimes(const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs); }
    static AnyTensor rmtimes(const AnyTensor& lhs, const AnyTensor& rhs) { return rhs.mtimes(lhs); }
  }
%feature("nonstatic", "");
#endif // SWIGMATLAB

#ifdef WINMAT64
%begin %{
#define UINT64_T unsigned long long int
%}
#endif
