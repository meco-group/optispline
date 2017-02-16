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
    AnyScalar(const SX& s);
    AnyScalar(const MX& s);
    AnyScalar();

#ifndef SWIG
    explicit operator double() const;
    explicit operator SX() const;
    explicit operator MX() const;
#endif

    double as_double() const { return this->operator double();}
    SX as_SX() const { return this->operator SX();}
    MX as_MX() const { return this->operator MX();}

    bool is_double() const;
    bool is_SX() const;
    bool is_MX() const;

    static std::vector<double> as_double(const std::vector<AnyScalar>& v);
    static std::vector<SX> as_SX(const std::vector<AnyScalar>& v);
    static std::vector<MX> as_MX(const std::vector<AnyScalar>& v);
    static TensorType merge(TensorType a, TensorType b);



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

    AnyScalar& operator+=(const AnyScalar& rhs);

    TensorType type() const {return t;}
    static TensorType type(const std::vector<AnyScalar>& v);
    static bool is_double(const std::vector<AnyScalar>& v) {return type(v)==TENSOR_DOUBLE;}
    static bool is_SX(const std::vector<AnyScalar>& v) {return type(v)==TENSOR_SX;}
    static bool is_MX(const std::vector<AnyScalar>& v) {return type(v)==TENSOR_MX;}

    static std::vector<AnyScalar> from_vector(const std::vector<double>& v);
    static std::vector<AnyScalar> from_vector(const std::vector<SX>& v);
    static std::vector<AnyScalar> from_vector(const std::vector<MX>& v);

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
    SX data_sx;
    MX data_mx;
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

    AnyTensor reorder_dims(const std::vector<int>& order) const {
      ANYTENSOR_METHOD(reorder_dims(order));
      return DT();
    }
    AnyTensor shape(const std::vector<int>& dims) const {
      ANYTENSOR_METHOD(shape(dims));
      return DT();
    }
    AnyTensor operator-() const {
      ANYTENSOR_METHOD(operator-());
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
    AnyTensor solve(const AnyTensor&b) const {
      ANYTENSOR_BINARY((*this), b, solve);
    }
    AnyTensor operator+(const AnyTensor&b) const {
      ANYTENSOR_BINARY((*this), b, operator+);
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

    inline friend AnyTensor mtimes(const AnyTensor &a, const AnyTensor &b) {
      return a.mtimes(b);
    }

    #ifndef SWIG
    /// Print a representation of the object to a stream (shorthand)
    inline friend
        std::ostream& operator<<(std::ostream &stream, const AnyTensor& obj) {
            return stream << obj.getRepresentation();
        }
    #endif // SWIG

    std::string getRepresentation() const {
      if (is_DT()) return "AnyTensor:" + as_DT().getRepresentation();
      if (is_ST()) return "AnyTensor:" + as_ST().getRepresentation();
      if (is_MT()) return "AnyTensor:" + as_MT().getRepresentation();
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
