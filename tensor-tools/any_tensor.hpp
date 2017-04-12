#ifndef ANY_TENSOR_HPP_INCLUDE
#define ANY_TENSOR_HPP_INCLUDE

#include "tensor.hpp"

enum TensorType {TENSOR_NULL, TENSOR_DOUBLE, TENSOR_SX, TENSOR_MX};



#define ANYSCALAR_BINARY_OP(OP) \
switch (AnyScalar::merge(x.t, y.t)) { \
  case TENSOR_DOUBLE: \
    return x.as_double() OP y.as_double();break; \
  case TENSOR_SX: \
    return x.as_SX() OP y.as_SX();break; \
  case TENSOR_MX: \
    return x.as_MX() OP y.as_MX();break; \
  default:\
     assert(false); return 0;\
}

#define ANYTENSOR_BINARY(X, Y, FUN) \
switch (AnyScalar::merge(X.t, Y.t)) { \
  case TENSOR_DOUBLE: \
    return X.as_DT().FUN(Y.as_DT()); \
  case TENSOR_SX: \
    return X.as_ST().FUN(Y.as_ST()); \
  case TENSOR_MX: \
    return X.as_MT().FUN(Y.as_MT()); \
  default:\
     assert(false); return DT();\
}

#define ANYTENSOR_METHOD(METHOD) \
if (is_DT()) return as_DT().METHOD; \
if (is_ST()) return as_ST().METHOD; \
if (is_MT()) return as_MT().METHOD; \
tensor_assert(false);

class AnyScalar {

  public:
#ifndef SWIG
    AnyScalar& operator=(const AnyScalar&);
#endif
    AnyScalar(const AnyScalar& s);
    AnyScalar(double s);
    AnyScalar(const casadi::DM& s);
    AnyScalar(const casadi::SX& s);
    AnyScalar(const casadi::MX& s);
    AnyScalar();

#ifndef SWIG
    explicit operator double() const;
    explicit operator casadi::SX() const;
    explicit operator casadi::MX() const;
#endif

    double as_double() const { return this->operator double();}
    casadi::SX as_SX() const { return this->operator casadi::SX();}
    casadi::MX as_MX() const { return this->operator casadi::MX();}

    bool is_double() const;
    bool is_SX() const;
    bool is_MX() const;

    static std::vector<double> as_double(const std::vector<AnyScalar>& v);
    static std::vector<casadi::SX> as_SX(const std::vector<AnyScalar>& v);
    static std::vector<casadi::MX> as_MX(const std::vector<AnyScalar>& v);
    static TensorType merge(TensorType a, TensorType b);

    static AnyScalar min(const AnyScalar& a, const AnyScalar& b);
    static AnyScalar max(const AnyScalar& a, const AnyScalar& b);

    /// Logic greater or equal to
    friend inline AnyScalar operator>=(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(>=)
    }

    friend inline AnyScalar operator<(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(<)
    }
    friend inline AnyScalar operator>(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(>)
    }
    friend inline AnyScalar operator<=(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(<=)
    }
    friend inline AnyScalar operator-(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(-)
    }
    friend inline AnyScalar operator*(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(*)
    }
    friend inline AnyScalar operator/(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(/)
    }
    friend inline AnyScalar operator==(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(==)
    }
    friend inline AnyScalar operator&&(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(&&)
    }
    friend inline AnyScalar operator+(const AnyScalar &x, const AnyScalar &y) {
      ANYSCALAR_BINARY_OP(+)
    }

    AnyScalar operator-() const { return operator*(*this,-1); }
    AnyScalar& operator+=(const AnyScalar& rhs);

    TensorType type() const {return t;}
    static TensorType type(const std::vector<AnyScalar>& v);
    static bool is_double(const std::vector<AnyScalar>& v) {return type(v)==TENSOR_DOUBLE;}
    static bool is_SX(const std::vector<AnyScalar>& v) {return type(v)==TENSOR_SX;}
    static bool is_MX(const std::vector<AnyScalar>& v) {return type(v)==TENSOR_MX;}

    static std::vector<AnyScalar> from_vector(const std::vector<double>& v);
    static std::vector<AnyScalar> from_vector(const std::vector<casadi::SX>& v);
    static std::vector<AnyScalar> from_vector(const std::vector<casadi::MX>& v);
    
    bool is_equal(const AnyScalar & rhs) const;

    #ifndef SWIG
    /// Print a representation of the object to a stream (shorthand)
    inline friend
        std::ostream& operator<<(std::ostream &stream, const AnyScalar& obj) {
            if (obj.is_double()) {
              stream << "AnyScalar(double):" << obj.as_double();
            } else if (obj.is_SX()) {
              stream << "AnyScalar(SX):" << obj.as_SX();
            } else if (obj.is_MX()) {
              stream << "AnyScalar(MX):" << obj.as_MX();
            } else {
              stream << "AnyScalar(null)";
            }
            return stream;
        }
    #endif // SWIG

  private:
    TensorType t;
    double data_double;
    casadi::SX data_sx;
    casadi::MX data_mx;
};

AnyScalar pow(const AnyScalar&x, int i);

class AnyTensor {
  public:
#ifndef SWIG
    AnyTensor& operator=(const AnyTensor&);
#endif
    AnyTensor(const AnyScalar& s);
    AnyTensor(const AnyTensor& s);
    //AnyTensor(const AnyTensor&, const std::vector<int>& dim);
    AnyTensor(const DT & t);
    AnyTensor(const ST & t);
    AnyTensor(const MT & t);
    static AnyTensor unity();
    AnyTensor();
    bool is_DT() const;
    bool is_ST() const;
    bool is_MT() const;
    DT as_DT() const { return this->operator DT();}
    ST as_ST() const { return this->operator ST();}
    MT as_MT() const { return this->operator MT();}
    static std::vector<DT> as_DT(const std::vector<AnyTensor>& v);
    static std::vector<ST> as_ST(const std::vector<AnyTensor>& v);
    static std::vector<MT> as_MT(const std::vector<AnyTensor>& v);
    static bool is_DT(const std::vector<AnyTensor>& v) {return type(v)==TENSOR_DOUBLE;}
    static bool is_ST(const std::vector<AnyTensor>& v) {return type(v)==TENSOR_SX;}
    static bool is_MT(const std::vector<AnyTensor>& v) {return type(v)==TENSOR_MX;}

    //bool equals(const AnyTensor&rhs) const;

    static TensorType type(const std::vector<AnyTensor>& v);
    TensorType type() const {return t; } ;

#ifndef SWIG
    explicit operator DT() const;
    explicit operator ST() const;
    explicit operator MT() const;
#endif
    static AnyTensor vertcat(const std::vector<AnyScalar>& v);
    static AnyTensor concat(const std::vector<AnyTensor>& v, int axis);

    static AnyTensor repeat(const AnyTensor&e, const std::vector<int>& factors);

    static AnyTensor pack(const std::vector<AnyTensor>& v, int axis);
    static std::vector<AnyTensor> unpack(const AnyTensor& v, int axis);
    std::vector< std::vector<AnyScalar> > unpack_2() const;
    std::vector< AnyScalar > unpack_1() const;


    AnyTensor get_slice(const AnySlice& i, const AnySlice& j) const {
      ANYTENSOR_METHOD(get_slice(i, j));
      return DT();
    }

    AnyTensor get_slice(const AnySlice& i) const {
      ANYTENSOR_METHOD(get_slice(i));
      return DT();
    }

    AnyTensor reorder_dims(const std::vector<int>& order) const {
      ANYTENSOR_METHOD(reorder_dims(order));
      return DT();
    }
    AnyTensor shape(const std::vector<int>& dims) const {
      ANYTENSOR_METHOD(shape(dims));
      return DT();
    }
    bool is_scalar() const {
      if (is_DT()) return as_DT().is_scalar();
      if (is_ST()) return as_ST().is_scalar();
      if (is_MT()) return as_MT().is_scalar();
      return true;
    }
    AnyScalar as_scalar() const {
      if (is_DT()) return as_DT().as_scalar();
      if (is_ST()) return as_ST().as_scalar();
      if (is_MT()) return as_MT().as_scalar();
      return 0;
    }

  AnyTensor flatten_first(int n) const {
    tensor_assert(n<=n_dims());
    std::vector<int> new_shape;
    std::vector<int> old_dims = dims();
    int prod = 1;
    for (int i=0;i<n;++i) prod*=old_dims[i];
    new_shape.push_back(prod);
    new_shape.insert(new_shape.end(), old_dims.begin()+n, old_dims.end());
    return shape(new_shape);
  }
  AnyTensor flatten_last(int n) const {
    tensor_assert(n<=n_dims());
    std::vector<int> new_shape;
    std::vector<int> old_dims = dims();
    int prod = 1;
    for (int i=0;i<n;++i) prod*=old_dims[old_dims.size()-i-1];
    new_shape.insert(new_shape.end(), old_dims.begin(), old_dims.begin()+old_dims.size()-n);
    new_shape.push_back(prod);
    return shape(new_shape);
  }
    AnyTensor operator-() const {
      ANYTENSOR_METHOD(operator-());
      return DT();
    }
    bool is_vector() const {
      ANYTENSOR_METHOD(is_vector());
      return false;
    }
    AnyTensor as_vector() const {
      ANYTENSOR_METHOD(as_vector());
      return DT();
    }
    AnyTensor squeeze() const {
      std::vector<int> squeeze_dims;
      for (int i=0;i<n_dims();++i) {
        if (dims()[i]!=1) squeeze_dims.push_back(dims()[i]);
      }
      return shape(squeeze_dims);
    }
    std::vector<int> dims() const {
      ANYTENSOR_METHOD(dims());
      return std::vector<int>();
    }
    int numel() const {
      ANYTENSOR_METHOD(numel());
      return 0;
    }
    int n_dims() const {
      ANYTENSOR_METHOD(n_dims());
      return 0;
    }

    AnyTensor operator>=(const AnyTensor &b) const {
      ANYTENSOR_BINARY((*this), b, operator>=);
    }
    AnyTensor operator<=(const AnyTensor &b) const {
      ANYTENSOR_BINARY((*this), b, operator<=);
    }
    AnyTensor outer_product(const AnyTensor &b) const {
      ANYTENSOR_BINARY((*this), b, outer_product);
    }
    AnyTensor inner(const AnyTensor&b) const {
      ANYTENSOR_BINARY((*this), b, inner);
    }
    AnyTensor transform(const AnyTensor &b, int axis) const {
      switch (AnyScalar::merge(t, b.t)) {
        case TENSOR_DOUBLE:
          return data_double.transform(b.data_double, axis);
          break;
        case TENSOR_SX:
          return as_ST().transform(b.as_ST(), axis);
          break;
        case TENSOR_MX:
          return as_MT().transform(b.as_MT(), axis);
          break;
        default:
          assert(false); return DT();
      }
    }
    AnyTensor solve(const AnyTensor&b) const {
      if (is_DT() && !b.is_DT()) {
         casadi::DM A = as_DT().matrix();
         casadi::DM Ainv = casadi::DM::solve(A, casadi::DM::eye(A.size1()), "lapackqr", casadi::Dict());
         return AnyTensor(DT(Ainv)).mtimes(b);
      }
      ANYTENSOR_BINARY((*this), b, solve);
    }
    AnyTensor operator+(const AnyTensor&b) const {
      ANYTENSOR_BINARY((*this), b, operator+);
    }
    AnyTensor operator-(const AnyTensor&b) const {
      ANYTENSOR_BINARY((*this), b, operator-);
    }
    AnyTensor operator*(const AnyTensor&b) const {
      ANYTENSOR_BINARY((*this), b, operator*);
    }
    AnyTensor& operator+=(const AnyTensor&b) {
      return this->operator=((*this) + b);
    }

    AnyTensor mtimes(const AnyTensor& rhs) const {
      ANYTENSOR_BINARY((*this), rhs, mtimes);
    }

    AnyTensor trailing_mtimes(const AnyTensor& rhs) const {
      ANYTENSOR_BINARY((*this), rhs, trailing_mtimes);
    }

    AnyTensor trailing_rmtimes(const AnyTensor& rhs) const {
      ANYTENSOR_BINARY((*this), rhs, trailing_rmtimes);
    }

    AnyTensor einstein(const AnyTensor &B, const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& c) const {
      const AnyTensor& X = *this;
      const AnyTensor& Y = B;
      switch (AnyScalar::merge(X.t, Y.t)) {
        case TENSOR_DOUBLE:
          return X.as_DT().einstein(Y.as_DT(), a, b, c);
        case TENSOR_SX:
          return X.as_ST().einstein(Y.as_ST(), a, b, c);
        case TENSOR_MX:
          return X.as_MT().einstein(Y.as_MT(), a, b, c);
        default:
           assert(false); return DT();
      }
    }

    AnyTensor einstein(const std::vector<int>& a, const std::vector<int>& c) const {
      const AnyTensor& X = *this;
      switch (X.type()) {
        case TENSOR_DOUBLE:
          return X.as_DT().einstein(a, c);
        case TENSOR_SX:
          return X.as_ST().einstein(a, c);
        case TENSOR_MX:
          return X.as_MT().einstein(a, c);
        default:
           assert(false); return DT();
      }
    }

    inline friend AnyTensor mtimes(const AnyTensor &a, const AnyTensor &b) {
      return a.mtimes(b);
    }

    #ifndef SWIG
    /// Print a representation of the object to a stream (shorthand)
    inline friend
        std::ostream& operator<<(std::ostream &stream, const AnyTensor& obj) {
            return stream << obj.to_string();
        }
    #endif // SWIG

    std::string to_string() const {
      if (is_DT()) return "AnyTensor:" + as_DT().to_string();
      if (is_ST()) return "AnyTensor:" + as_ST().to_string();
      if (is_MT()) return "AnyTensor:" + as_MT().to_string();
      return "";
    }


  private:
    TensorType t;
    DT data_double;
    ST data_sx;
    MT data_mx;
};

class AnyVector : public AnyTensor {
  public:
    AnyVector(const AnyTensor& s);
    AnyVector(const std::vector<AnyScalar>& s);
    AnyVector(const DT & t);
    AnyVector(const ST & t);
    AnyVector(const MT & t);
   // AnyVector& operator=(const AnyTensor& s);
    AnyVector();
    AnyScalar operator[](int index) const;
    int size() const {
      return dims()[0];
    }
    std::vector<AnyScalar> to_scalar_vector() const;
    AnyVector sort(bool ascending=true) const;
    AnyVector uniquify() const;
    bool is_equal(const AnyVector& rhs) const;
};

namespace casadi {
  template<>
  class casadi_limits<AnyScalar>{
  public:
    static bool is_zero(const AnyScalar& val);
    //static bool is_equal(const AnyScalar& x, const AnyScalar& y, int depth);
    static bool is_one(const AnyScalar& val);
  };
}

AnyTensor vertcat(const std::vector<AnyScalar> & v);
AnyTensor vertcat(const std::vector<double> & v);


/*class AnyVector : public AnyTensor {
    AnyVector(const std::vector<AnyScalar>& s) : AnyTensor(vertcat(s)) {
    }
};*/

#undef ANYSCALAR_BINARY_OP
#undef ANYTENSOR_BINARY
#undef ANYTENSOR_METHOD

#endif
