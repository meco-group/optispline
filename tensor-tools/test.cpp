#include <any_tensor.hpp>




template<class T, class S>
void assert_equal(T a, S b) {
  assert(static_cast<double>(norm_inf(a-b))==0);
}

template<>
void assert_equal(double a, double b) {
  assert(a-b==0);
}

template<>
void assert_equal(const std::vector<double>& a, const std::vector<double>& b) {
  assert(a==b);
}

template<>
void assert_equal(std::vector<double> a, std::vector<double> b) {
  assert(a==b);
}

template<>
void assert_equal(AnyTensor a, AnyTensor b) {
  //assert(a.equals(b));
  assert(a.dims()==b.dims());
  //assert(a.data()==b.data());
}

int main() {

  ST t = ST::sym("t", {2, 4, 5});

  ST t2 = ST::sym("t2", {2, 4 , 5});

  ST t3 = (t+t2)*t;

  std::cout << "t3" << t3 << endl;

  DT t4 = DT(DM({{1, 2, 5, 6}, {3, 4, 7, 8}}), {2, 2, 2});
  DT t5 = t4+t4;

  std::cout << "t" << t5 << endl;

  // test sub2ind, ind2sub
  std::vector<int> dims = {3, 2, 4};
  for (int i=0;i<3*2*4;i++) {
    std::vector<int> ind = Tensor<DM>::sub2ind(dims, i);
    int j = Tensor<DM>::ind2sub(dims, ind);
    assert(i==j);
  }

  DM expected = DM({{2, 10, 4, 12}, {6, 14, 8, 16}});
  DM got = t5.reorder_dims({0, 2, 1}).data();


  assert_equal(got, expected);

  expected = DM({{2, 10, 6, 14}, {4, 12, 8, 16}});
  got = t5.reorder_dims({1, 2, 0}).data();

  assert_equal(got, expected);

  DT t6 = DT(DM(std::vector<std::vector<double> >{{3, 4}, {1, 7}}), {2, 2});

  DT t7 = t6.partial_product(t5);

  expected = DM({{30, 44, 86, 100}, {44, 60, 108, 124}});
  got = t7.data();

  assert_equal(got, expected);

  DT t8 = t7.partial_product(t5);
  expected = DM({{324, 472, 2260, 2632}, {448, 656, 2816, 3280}});
  got = t8.data();

  assert_equal(got, expected);

  std::cout << t6.outer_product(t5) << std::endl;

  // Outer product
  t8 = DT(DM(std::vector<std::vector<double> >{{3, 4}, {1, 7}}), {2, 2});
  DT t9 = DT(DM(std::vector<double>{1, 2}), {2, 1});

  DT t10 = t8.outer_product(t9);
  assert((t10.dims()==std::vector<int>{2, 2, 2, 1}));

  expected = DM({{3, 4, 6, 8}, {1, 7, 2, 14}});
  got = t10.data();


  assert_equal(got, expected);

  t9 = DT(DM(std::vector<double>{1, 2}), {1, 2});

  t10 = t8.outer_product(t9);
  assert((t10.dims()==std::vector<int>{2, 2, 1, 2}));

  expected = DM({{3, 4, 6, 8}, {1, 7, 2, 14}});
  got = t10.data();

  assert_equal(got, expected);

  t9 = DT(DM(std::vector<double>{1, 2}), {2});

  t10 = t8.outer_product(t9);
  assert((t10.dims()==std::vector<int>{2, 2, 2}));

  expected = DM({{3, 4, 6, 8}, {1, 7, 2, 14}});
  got = t10.data();

  assert_equal(got, expected);

  // Slice tests
  t10 = t5({0, -1, -1});
  expected = DM(std::vector<std::vector<double> >{{2, 10}, {4, 12}});
  got = t10.data();

  assert_equal(got, expected);

  t10 = t5({1, -1, -1});
  expected = DM(std::vector<std::vector<double> >{{6, 14}, {8, 16}});
  got = t10.data();

  assert_equal(got, expected);

  t10 = t5({-1, 0, -1});
  expected = DM(std::vector<std::vector<double> >{{2, 10}, {6, 14}});
  got = t10.data();

  assert_equal(got, expected);

  t10 = t5({-1, 1, -1});
  expected = DM(std::vector<std::vector<double> >{{4, 12}, {8, 16}});
  got = t10.data();

  assert_equal(got, expected);

  t10 = t5({-1, -1, 0});
  expected = DM(std::vector<std::vector<double> >{{2, 4}, {6, 8}});
  got = t10.data();

  assert_equal(got, expected);

  t10 = t5({-1, -1, 1});
  expected = DM(std::vector<std::vector<double> >{{10, 12}, {14, 16}});
  got = t10.data();

  assert_equal(got, expected);

  t8 = DT(DM(std::vector<std::vector<double> >{{3, 4}, {1, 7}}), {2, 2});
  t9 = DT(DM(std::vector<double>{1, 2}), {2, 1});

  t8.einstein(t9, {-1, -2}, {-2, -3}, {-1, -3});

  expected = DM(3);
  got = t8({0, 0}).data();
  assert_equal(got, expected);

  expected = DM(4);
  got = t8({0, 1}).data();
  assert_equal(got, expected);

  // Scalar
  expected = DM(5);
  got = DT(5.0).data();
  assert_equal(got, expected);


  // Inner product
  DT v1 = DT(DM(std::vector<double>{0, 1, 2}), {3});
  DT v2 = DT(DM(std::vector<double>{1, 2, 3}), {3});

  expected = DM(std::vector<double>{8});

  got = v1.inner(v2).data();
  assert_equal(got, expected);

  got = v2.inner(v1).data();
  assert_equal(got, expected);

  DT m1 = DT(DM(std::vector<std::vector<double>>{{0, 1}, {2, 3}, {4, 5}}), {3, 2});

  expected = DM(std::vector<double>{10, 13});

  got = v1.inner(m1).data();
  assert_equal(got, expected);

  got = m1.inner(v1).data();
  assert_equal(got, expected);

  DT s1 = DT(5.0);
  DT s2 = DT(7.0);

  expected = DM(std::vector<double>{35});
  got = s1.inner(s2).data();
  assert_equal(got, expected);

  expected = DM(std::vector<double>{0, 5, 10});
  got = s1.inner(v1).data();
  assert_equal(got, expected);

  expected = DM(std::vector<double>{0, 5, 10});
  got = v1.inner(s1).data();
  assert_equal(got, expected);

  AnyScalar a = 1.5;

  double w = a;
  assert_equal(1.5, w);

  {
    AnyScalar a = SX(1.5);
    SX w = a;
    assert_equal(1.5, w);
  }
  
  {

    std::vector<AnyScalar> v = {2, 3};
    AnyTensor t = vertcat(v);

    //assert_equal(t, AnyTensor(DT(DM({{2, 3}}),{2})));

    std::vector<double> d = AnyScalar::vector_double(v);

    assert_equal(d, std::vector<double>{2, 3});
  }

  {
    AnyTensor a = AnyTensor::unity();

  }

  return 0;
}
