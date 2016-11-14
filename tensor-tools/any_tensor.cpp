#include "any_tensor.hpp"

int product(const std::vector<int>& a) {
  int r = 1;
  for (int i=0;i<a.size();++i) r*=a[i];
  return r;
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
  if (x.is_double()) return pow(x.as_double(), i);
  if (x.is_SX()) return pow(x.as_SX(), SX(i));
  if (x.is_MX()) return pow(x.as_MX(), MX(i));
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

AnyScalar::AnyScalar(const SX& s) {
  t = TENSOR_SX;
  data_sx = s;
}

AnyScalar::AnyScalar(const MX& s) {
  t = TENSOR_MX;
  data_mx = s;
}

AnyScalar::AnyScalar() {
  t = TENSOR_NULL;
  data_double = 0;
  data_sx = 0;
  data_mx = 0;
}

AnyScalar::operator double() const {
  tensor_assert(t==TENSOR_DOUBLE);
  return data_double;
}

AnyScalar::operator SX() const {
  if (t==TENSOR_DOUBLE) return SX(data_double);
  tensor_assert(t==TENSOR_SX);
  return data_sx;
}

AnyScalar::operator MX() const {
  if (t==TENSOR_DOUBLE) return MX(data_double);
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
  tensor_assert_message(false, "Not implemented");
  return DT();
}

std::vector<double> AnyScalar::as_double(const std::vector<AnyScalar>& v) {
  std::vector<double> ret;
  ret.reserve(v.size());
  for (auto & i : v)
    ret.push_back(i.as_double());
  return ret;
}

std::vector<SX> AnyScalar::as_SX(const std::vector<AnyScalar>& v) {
  std::vector<SX> ret;
  ret.reserve(v.size());
  for (auto & i : v)
    ret.push_back(i.as_SX());
  return ret;
}

std::vector<MX> AnyScalar::as_MX(const std::vector<AnyScalar>& v) {
  std::vector<MX> ret;
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

std::vector<AnyTensor> unpack(const AnyTensor& v, int axis) {
  tensor_assert_message(false, "Not implemented yet");
  return {DT()};
}


AnyTensor AnyTensor::vertcat(const std::vector<AnyScalar>& v) {
  switch (AnyScalar::type(v)) {
    case TENSOR_DOUBLE: return DT(AnyScalar::as_double(v), {static_cast<int>(v.size())});
    case TENSOR_SX: return ST(SX::vertcat(AnyScalar::as_SX(v)), {static_cast<int>(v.size())});
    case TENSOR_MX: return MT(MX::vertcat(AnyScalar::as_MX(v)), {static_cast<int>(v.size())});
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
  return DT(DM(v), {static_cast<int>(v.size())});
}
