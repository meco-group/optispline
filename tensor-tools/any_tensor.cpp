#include "any_tensor.hpp"
#include <memory>

//#include <casadi/core/function/function_internal.hpp>

std::vector<int> invert_order(const std::vector<int>& order) {
  std::vector<int> ret(order.size());
  for (int i=0;i<order.size();++i) {
    int j=order[i];
    ret[j] = i;
  }
  return ret;
}

bool AnyScalar::is_double() const {
  return t == TENSOR_DOUBLE;
}
bool AnyScalar::is_SX() const {
  return t == TENSOR_SX;
}
bool AnyScalar::is_MX() const {
  return t == TENSOR_MX;
}

bool AnyTensor::is_DT() const {
  return t == TENSOR_DOUBLE;
}
bool AnyTensor::is_ST() const {
  return t == TENSOR_SX;
}
bool AnyTensor::is_MT() const {
  return t == TENSOR_MX;
}

AnyTensor AnyTensor::unity() {
  return DT(1, {});
}

TensorType AnyScalar::merge(TensorType a, TensorType b) {
  if (a == TENSOR_SX && b == TENSOR_MX) tensor_assert(0);
  if (a == TENSOR_MX && b == TENSOR_SX) tensor_assert(0);
  if (a == TENSOR_SX || b == TENSOR_SX) return TENSOR_SX;
  if (a == TENSOR_MX || b == TENSOR_MX) return TENSOR_MX;

  return TENSOR_DOUBLE;
}

TensorType AnyScalar::type(const std::vector<AnyScalar>& v) {
  TensorType t = TENSOR_DOUBLE;
  for (auto & i : v) t = AnyScalar::merge(t, i.t);
  return t;
}

TensorType AnyTensor::type(const std::vector<AnyTensor>& v) {
  TensorType t = TENSOR_DOUBLE;
  for (auto & i : v) t = AnyScalar::merge(t, i.t);
  return t;
}

AnyScalar& AnyScalar::operator+=(const AnyScalar& rhs) {
  AnyScalar ret;
  switch (AnyScalar::merge(t, rhs.t)) {
    case TENSOR_DOUBLE:
      ret = as_double()+rhs.as_double();
      break;
    case TENSOR_SX:
      ret = as_SX()+rhs.as_SX();
      break;
    case TENSOR_MX:
      ret = as_MX()+rhs.as_MX();
      break;
    default: tensor_assert(false);
  }

  return this->operator=(ret);
}

/**bool AnyTensor::equals(const AnyTensor& rhs) const {
  if (dims()==rhs.dims()) {
    if (TENSOR_DOUBLE) {
      return static_cast<double>(norm_inf((*this)-rhs))==0;
    }
  }
  return false;
}*/

AnyScalar pow(const AnyScalar&x, int i) {
  if (x.is_double()) return pow(x.as_double(), static_cast<double>(i));
  if (x.is_SX()) return pow(x.as_SX(), casadi::SX(i));
  if (x.is_MX()) return pow(x.as_MX(), casadi::MX(i));
  tensor_assert(false);
  return 0;
}

AnyScalar::AnyScalar(const AnyScalar& s) {
  t = s.t;
  data_double = s.data_double;
  data_sx = s.data_sx;
  data_mx = s.data_mx;
}

AnyScalar& AnyScalar::operator=(const AnyScalar& s) {
  t = s.t;
  data_double = s.data_double;
  data_sx = s.data_sx;
  data_mx = s.data_mx;
  return *this;
}

AnyScalar::AnyScalar(double s) {
  t = TENSOR_DOUBLE;
  data_double = s;
}

AnyScalar::AnyScalar(const casadi::SX& s) {
  t = TENSOR_SX;
  data_sx = s;
  tensor_assert(s.is_scalar());
}

AnyScalar::AnyScalar(const casadi::DM& s) {
  t = TENSOR_DOUBLE;
  tensor_assert(s.is_scalar());
  data_double = static_cast<double>(s);
}

AnyScalar::AnyScalar(const casadi::MX& s) {
  t = TENSOR_MX;
  data_mx = s;
  tensor_assert(s.is_scalar());
}

AnyScalar::AnyScalar() {
  t = TENSOR_DOUBLE;
  data_double = 0;
}

AnyScalar::operator double() const {
  tensor_assert(t==TENSOR_DOUBLE);
  return data_double;
}

AnyScalar::operator casadi::SX() const {
  if (t==TENSOR_DOUBLE) return casadi::SX(data_double);
  tensor_assert(t==TENSOR_SX);
  return data_sx;
}

AnyScalar::operator casadi::MX() const {
  if (t==TENSOR_DOUBLE) return casadi::MX(data_double);
  tensor_assert(t==TENSOR_MX);
  return data_mx;
}

AnyTensor& AnyTensor::operator=(const AnyTensor& s) {
  t = s.t;
  data_double = s.data_double;
  data_sx = s.data_sx;
  data_mx = s.data_mx;
  return *this;
}

AnyScalar AnyScalar::min(const AnyScalar& a, const AnyScalar& b) {
  switch (AnyScalar::merge(a.type(), b.type())) {
    case TENSOR_DOUBLE:
      return std::fmin(a.as_double(), b.as_double());
    case TENSOR_SX:
      return fmin(a.as_SX(), b.as_SX());
    case TENSOR_MX:
      return fmin(a.as_MX(), b.as_MX());
    default:
      tensor_assert(false);
      return AnyScalar();
  }
}

AnyScalar AnyScalar::max(const AnyScalar& a, const AnyScalar& b) {
  switch (AnyScalar::merge(a.type(), b.type())) {
    case TENSOR_DOUBLE:
      return std::fmax(a.as_double(), b.as_double());
    case TENSOR_SX:
      return fmax(a.as_SX(), b.as_SX());
    case TENSOR_MX:
      return fmax(a.as_MX(), b.as_MX());
    default:
      tensor_assert(false);
      return AnyScalar();
  }
}

AnyTensor::AnyTensor(const AnyScalar& s) : data_double(0), data_sx(0), data_mx(0) {
  t = s.type();
  switch (t) {
    case TENSOR_DOUBLE:
      data_double = DT(s.as_double(), {1});
      break;
    case TENSOR_SX:
      data_sx = ST(s.as_SX(), {1});
      break;
    case TENSOR_MX:
      data_mx = MT(s.as_MX(), {1});
      break;
    default:
      tensor_assert(false);
  }
}

AnyTensor::AnyTensor(const AnyTensor& s) : data_double(0), data_sx(0), data_mx(0) {
  t = s.t;
  data_double = s.data_double;
  data_sx = s.data_sx;
  data_mx = s.data_mx;
}

AnyTensor::AnyTensor(const DT & s) : data_double(s), data_sx(0), data_mx(0) {
  t = TENSOR_DOUBLE;
}

AnyTensor::AnyTensor(const ST & s) : data_double(0), data_sx(s), data_mx(0) {
  t = TENSOR_SX;
}

AnyTensor::AnyTensor(const MT & s) : data_double(0), data_sx(0), data_mx(s) {
  t = TENSOR_MX;
}

//AnyTensor::AnyTensor(const AnyScalar& s) {
//  if (s.t == TENSOR_DOUBLE) {
//
//  }
//}

AnyTensor::AnyTensor() : data_double(0), data_sx(0), data_mx(0) {
  t = TENSOR_NULL;
}

AnyTensor::operator DT() const {
  tensor_assert(t==TENSOR_DOUBLE);
  return data_double;
}

AnyTensor::operator ST() const {
  if (t==TENSOR_DOUBLE) return ST(data_double);
  tensor_assert(t==TENSOR_SX);
  return data_sx;
}

AnyTensor::operator MT() const {
  if (t==TENSOR_DOUBLE) return MT(data_double);
  tensor_assert(t==TENSOR_MX);
  return data_mx;
}


AnyTensor AnyTensor::concat(const std::vector<AnyTensor>& v, int axis) {
  switch (AnyTensor::type(v)) {
    case TENSOR_DOUBLE: return DT::concat(AnyTensor::as_DT(v), axis);
    case TENSOR_SX: return ST::concat(AnyTensor::as_ST(v), axis);
    case TENSOR_MX: return MT::concat(AnyTensor::as_MT(v), axis);
    default: tensor_assert(false); return DT();
  }
}

std::vector<double> AnyScalar::as_double(const std::vector<AnyScalar>& v) {
  std::vector<double> ret;
  ret.reserve(v.size());
  for (auto & i : v)
    ret.push_back(i.as_double());
  return ret;
}

std::vector<casadi::SX> AnyScalar::as_SX(const std::vector<AnyScalar>& v) {
  std::vector<casadi::SX> ret;
  ret.reserve(v.size());
  for (auto & i : v)
    ret.push_back(i.as_SX());
  return ret;
}

std::vector<casadi::MX> AnyScalar::as_MX(const std::vector<AnyScalar>& v) {
  std::vector<casadi::MX> ret;
  ret.reserve(v.size());
  for (auto & i : v)
    ret.push_back(i.as_MX());
  return ret;
}

std::vector<DT> AnyTensor::as_DT(const std::vector<AnyTensor>& v) {
  std::vector<DT> ret;
  ret.reserve(v.size());
  for (auto & i : v)
    ret.push_back(i.as_DT());
  return ret;
}

std::vector<ST> AnyTensor::as_ST(const std::vector<AnyTensor>& v) {
  std::vector<ST> ret;
  ret.reserve(v.size());
  for (auto & i : v)
    ret.push_back(i.as_ST());
  return ret;
}

std::vector<MT> AnyTensor::as_MT(const std::vector<AnyTensor>& v) {
  std::vector<MT> ret;
  ret.reserve(v.size());
  for (auto & i : v)
    ret.push_back(i.as_MT());
  return ret;
}

AnyTensor AnyTensor::pack(const std::vector<AnyTensor>& v, int axis) {
  switch (AnyTensor::type(v)) {
    case TENSOR_DOUBLE: return DT::pack(AnyTensor::as_DT(v), axis);
    case TENSOR_SX: return ST::pack(AnyTensor::as_ST(v), axis);
    case TENSOR_MX: return MT::pack(AnyTensor::as_MT(v), axis);
    default: tensor_assert(false); return DT();
  }
}

AnyTensor AnyTensor::repeat(const AnyTensor&e, const std::vector<int>& factors) {
    if (e.is_DT()) return DT::repeat(e.as_DT(), factors);
    if (e.is_ST()) return ST::repeat(e.as_ST(), factors);
    if (e.is_MT()) return MT::repeat(e.as_MT(), factors);
    return {DT()};
}

std::vector<AnyTensor> AnyTensor::unpack(const AnyTensor& v, int axis) {
    spline_assert(axis<v.n_dims());
    int n = v.dims()[axis];
    int N = v.n_dims();
    std::vector<AnyTensor> ret;
    std::vector<int> a_e = mrange(N);
    std::vector<int> c_e = a_e;
    c_e.erase(c_e.begin()+axis);

    for (int i=0;i<n;++i) {
        casadi::DM ind = casadi::DM::zeros(n, 1);
        ind.nz(i) = 1;
        ret.push_back(v.einstein(DT(ind, {n}), a_e, {-axis-1}, c_e));
    }

  return ret;
}


AnyTensor AnyTensor::vertcat(const std::vector<AnyScalar>& v) {
  switch (AnyScalar::type(v)) {
    case TENSOR_DOUBLE: return DT(AnyScalar::as_double(v), {static_cast<int>(v.size())});
    case TENSOR_SX: return ST(casadi::SX::vertcat(AnyScalar::as_SX(v)), {static_cast<int>(v.size())});
    case TENSOR_MX: return MT(casadi::MX::vertcat(AnyScalar::as_MX(v)), {static_cast<int>(v.size())});
    default: tensor_assert(false); return DT();
  }
}

/**
AnyTensor::AnyTensor(const std::vector<AnyScalar>&v, const std::vector<int>& dim) {
  std::vector<>
}
*/

AnyTensor vertcat(const std::vector<AnyScalar> & v) {
  return AnyTensor::vertcat(v);
}

AnyTensor concat(const std::vector<AnyTensor> & v, int axis) {
  return AnyTensor::concat(v, axis);
}


AnyTensor vertcat(const std::vector<double>& v) {
  return DT(v, {static_cast<int>(v.size())});
}

std::vector<AnyScalar> AnyScalar::from_vector(const std::vector<double>& v) {
  std::vector<AnyScalar> ret(v.size());
  std::copy(v.begin(), v.end(), ret.begin());
  return ret;
}
std::vector<AnyScalar> AnyScalar::from_vector(const std::vector<casadi::SX>& v) {
  std::vector<AnyScalar> ret(v.size());
  std::copy(v.begin(), v.end(), ret.begin());
  return ret;
}
std::vector<AnyScalar> AnyScalar::from_vector(const std::vector<casadi::MX>& v) {
  std::vector<AnyScalar> ret(v.size());
  std::copy(v.begin(), v.end(), ret.begin());
  return ret;
}

bool AnyScalar::is_equal(const AnyScalar & rhs) const {
  if (type()!=rhs.type()) return false;
  switch (type()) {
    case TENSOR_DOUBLE:
      return as_double()==rhs.as_double();
    case TENSOR_SX:
      return casadi::SX::is_equal(as_SX(), rhs.as_SX());
    case TENSOR_MX:
      return casadi::MX::is_equal(as_MX(), rhs.as_MX());
    default:
      return false;
  }
}


AnyVector::AnyVector(const AnyTensor& s) : AnyTensor(s.as_vector()) {
  tensor_assert_message(n_dims()<=1,
    "AnyVector can have only one dimension. Got " << s.dims() << ".")
}

AnyVector::AnyVector(const std::vector<AnyScalar>& s) : AnyTensor(vertcat(s)) { }

AnyVector::AnyVector(const DT & s) : AnyTensor(s.as_vector()) {
  tensor_assert_message(n_dims()<=1, "AnyVector can have only one dimension.")
}

AnyVector::AnyVector(const ST & s) : AnyTensor(s.as_vector()) {
  tensor_assert_message(n_dims()<=1, "AnyVector can have only one dimension.")
}

AnyVector::AnyVector(const MT & s) : AnyTensor(s.as_vector()) {
  tensor_assert_message(n_dims()<=1, "AnyVector can have only one dimension.")
}

AnyVector AnyVector::perturbation() const {
  if(numel() < 2) return *this;
  std::vector<AnyScalar> r = to_scalar_vector();
  std::vector<AnyScalar> ret;
  ret.push_back((3*r[0]+r[1])/4);
  ret.insert(ret.end(),r.begin()+1,r.begin()+r.size()-1);
  ret.push_back((3*r[r.size()-1]+r[r.size()-2])/4);
  return ret;
}

/**AnyVector& AnyVector::operator=(const AnyTensor& s)  {
  AnyTensor::operator=(s);
  return *this;
}*/

AnyVector::AnyVector() : AnyTensor() { }

AnyScalar AnyVector::operator[](int index) const {
  if (is_DT()) {
    return static_cast<double>(as_DT().data().nz(index));
  }
  if (is_ST()) {
    return as_ST().data().nz(index);
  }
  if (is_MT()) {
    return as_MT().data().nz(index);
  }
  assert(false);
  return 0;
}


std::vector<AnyScalar> AnyVector::to_scalar_vector() const {
  std::vector<AnyScalar> ret(size());
  for (int i=0; i<size(); i++) {
    ret[i] = (*this)[i];
  }
  return ret;
}

namespace casadi {
  class Sorter : public casadi::Callback {
  public:
    Sorter(int size, int ascending) : size_(size), ascending_(ascending) {
      construct("sorter");
    };

    /** \brief  Destructor */
    ~Sorter() override {};

    ///@{
    /** \brief Number of function inputs and outputs */
    int get_n_in() override { return 1; };
    int get_n_out() override { return 1; };
    ///@}

    /// @{
    /** \brief Sparsities of function inputs and outputs */
    Sparsity get_sparsity_in(int i) override { return Sparsity::dense(size_, 1); }
    Sparsity get_sparsity_out(int i) override { return Sparsity::dense(size_, 1); }
    /// @}

    ///@{
    /** \brief Return Jacobian of all input elements with respect to all output elements */
    bool has_jacobian() const override { return true; }
    casadi::Function get_jacobian(const std::string& name,
                                  const std::vector<std::string>& inames,
                                  const std::vector<std::string>& onames,
                                  const Dict& opts) const override {
      std::vector<MX> arg = join(mx_in(), mx_out());
      return casadi::Function(name, arg, {DM(size_, size_) }, opts);
    }
    ///@}

    /** \brief  Evaluate numerically, work vectors given */
    int eval(const double** arg, double** res, int* iw, double* w, void* mem) const override {
      if (!res[0]) return 0;
      std::copy(arg[0], arg[0]+size_, res[0]);

      if (ascending_) {
        std::qsort(res[0], size_, sizeof(double), [](const void* a, const void* b) {
            double arg1 = *static_cast<const double*>(a);
            double arg2 = *static_cast<const double*>(b);

            if (arg1 < arg2) return -1;
            if (arg1 > arg2) return 1;
            return 0;
        });
      } else {
        std::qsort(res[0], size_, sizeof(double), [](const void* a, const void* b) {
            double arg1 = *static_cast<const double*>(a);
            double arg2 = *static_cast<const double*>(b);

            if (arg1 < arg2) return 1;
            if (arg1 > arg2) return -1;
            return 0;
        });
      }
      return 0;
    }

    int size_;
    bool ascending_;
    
    static Sorter& construct_cached(int size, bool ascending) {
      static std::map<std::pair<int, bool>, std::unique_ptr<casadi::Sorter> > cache;
      std::pair<int, bool> key = {size, ascending};
      auto it = cache.find(key);
      if (it==cache.end()) cache[key] = std::unique_ptr<casadi::Sorter>(new casadi::Sorter(key.first, key.second));
      it = cache.find(key);
      return *it->second;
    }
  };

class Uniquifier : public casadi::Callback {
  public:

    Uniquifier(int size) : size_(size) { construct("uniquifier"); };

    /** \brief  Destructor */
    ~Uniquifier() override {};

    ///@{
    /** \brief Number of function inputs and outputs */
    int get_n_in() override { return 1; };
    int get_n_out() override { return 1; };
    ///@}

    /// @{
    /** \brief Sparsities of function inputs and outputs */
    Sparsity get_sparsity_in(int i) override { return Sparsity::dense(size_, 1); }
    Sparsity get_sparsity_out(int i) override { return Sparsity::dense(size_, 1); }
    /// @}

    ///@{
    /** \brief Return Jacobian of all input elements with respect to all output elements */
    bool has_jacobian() const override { return true; }
    casadi::Function get_jacobian(const std::string& name,
                                  const std::vector<std::string>& inames,
                                  const std::vector<std::string>& onames,
                                  const Dict& opts) const override {
      std::vector<MX> arg = join(mx_in(), mx_out());
      return casadi::Function(name, arg, {DM(size_, size_) }, opts);
    }
    ///@}

    /** \brief  Evaluate numerically, work vectors given */
    int eval(const double** arg, double** res, int* iw, double* w, void* mem) const override {
      if (!res[0]) return 0;
      std::copy(arg[0], arg[0]+size_, res[0]);

      for (int i=1;i<size_-2;++i) {
        if (arg[0][i]==arg[0][i+1]) {
          res[0][i]   = (res[0][i-1]+res[0][i])/2;
          res[0][i+1] = (res[0][i+1]+res[0][i+2])/2;
        }
      }
      return 0;
    }

    int size_;

    static Uniquifier& construct_cached(int size) {
      static std::map<int, std::unique_ptr<casadi::Uniquifier> > cache;
      int key = size;
      auto it = cache.find(key);
      if (it==cache.end()) cache[key] = std::unique_ptr<casadi::Uniquifier>(new casadi::Uniquifier(key));
      it = cache.find(key);
      return *it->second;
    }    

  };
  
} // namespace casadi

AnyVector AnyVector::sort(bool ascending) const {
  tensor_assert(!is_ST());
  
  casadi::Sorter& sorter = casadi::Sorter::construct_cached(dims()[0], ascending); 
  if (is_DT()) {
    return DT(sorter(std::vector<casadi::DM>{as_DT().data()})[0], {dims()[0]});
  } else {
    return MT(sorter(std::vector<casadi::MX>{as_MT().data()})[0], {dims()[0]});
  }
}

AnyVector AnyVector::uniquify() const {
  tensor_assert(!is_ST());

  casadi::Uniquifier& uniquifier = casadi::Uniquifier::construct_cached(dims()[0]); 
  
  if (is_DT()) {
    return DT(uniquifier(std::vector<casadi::DM>{as_DT().data()})[0], {dims()[0]});
  } else {
    return MT(uniquifier(std::vector<casadi::MX>{as_MT().data()})[0], {dims()[0]});
  }
}

bool AnyVector::is_equal(const AnyVector& rhs) const {
  std::vector< AnyScalar > a = to_scalar_vector();
  std::vector< AnyScalar > b = rhs.to_scalar_vector();
  if (a.size()!=b.size()) return false;
  for (int i=0;i<a.size();++i) {
    if (!a[i].is_equal(b[i])) return false;
  }
  return true;
}


std::vector< AnyScalar > AnyTensor::unpack_1() const {
    /* tensor_assert(n_dims()==1); */
    return AnyVector((*this)).to_scalar_vector();
}

std::vector< std::vector<AnyScalar> > AnyTensor::unpack_2() const {
    tensor_assert(n_dims()==2);
    std::vector<AnyTensor> u = AnyTensor::unpack(*this, 0);

    std::vector< std::vector<AnyScalar> > ret;
    for (const auto& a : u) ret.push_back(a.unpack_1());
    return ret;
}


namespace casadi {
  bool casadi_limits<AnyScalar>::is_zero(const AnyScalar& val) {
    TensorType t = val.type();
    switch (t) {
      case TENSOR_DOUBLE:
        return val.as_double()==0;
        break;
      case TENSOR_SX:
        return val.as_SX().is_zero();
        break;
      case TENSOR_MX:
        return val.as_MX().is_zero();
        break;
      default:
        tensor_assert(false);
    }
    return false;
  }

  bool casadi_limits<AnyScalar>::is_one(const AnyScalar& val) {
    TensorType t = val.type();
    switch (t) {
      case TENSOR_DOUBLE:
        return val.as_double()==1;
        break;
      case TENSOR_SX:
        return val.as_SX().is_one();
        break;
      case TENSOR_MX:
        return val.as_MX().is_one();
        break;
      default:
        tensor_assert(false);
    }
    return false;
  }

template <>
Tensor<casadi::SX> Tensor<casadi::SX>::solve(const Tensor<casadi::SX>& B) const {
  return casadi::SX::solve(matrix(), B.matrix());
}



} //namespace casadi
