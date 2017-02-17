#!/usr/bin/env python

from helpers import *

class Test_Function_Function(BasisTestCase):

    def test_construct_function(self):
        a = MX.sym('a',3 + 1,1)
        a = MTensor(a, [3 + 1,1,1])
        a = Coefficient(a)
        m = MonomialBasis(3)
        b = TensorBasis([m])
        f = Function(b,a)

    def test_function_n_inputs1(self):
        a = MX.sym('a',3 + 1,1)
        a = MTensor(a, [3 + 1,1,1])
        a = Coefficient(a)
        m = MonomialBasis(3)
        b = TensorBasis([m])
        f = Function(b,a)
        self.assertEqual(f.n_inputs(), 1)

    def test_function_n_inputs2(self):
        a = MX.sym('a', (3+1)*(4+1)*(1+1))
        a = MTensor(a, [3+1,4+1,1+1,1,1])
        m1 = MonomialBasis(3)
        m2 = MonomialBasis(4)
        m3 = MonomialBasis(1)
        a = Coefficient(a)
        b = TensorBasis([m1,m2,m3])
        f = Function(b,a)
        self.assertEqual(f.n_inputs(), 3)

    def test_function_shape1(self):
        a = MX.sym('a',3 + 1,1)
        a = MTensor(a, [3 + 1,1,1])
        a = Coefficient(a)
        m = MonomialBasis(3)
        b = TensorBasis([m])
        f = Function(b,a)
        self.assertEqual(f.shape, [1,1])

    def test_function_shape2(self):
        a = MX.sym('a', (3+1)*(4+1)*(1+1)*3*2)
        a = MTensor(a, [3+1,4+1,1+1,3,2])
        m1 = MonomialBasis(3)
        m2 = MonomialBasis(4)
        m3 = MonomialBasis(1)
        a = Coefficient(a)
        b = TensorBasis([m1,m2,m3])
        f = Function(b,a)
        self.assertEqual(f.shape, [3,2])

    def test_function_evaluation_type1(self):
        a_ = [2,0,0,1]
        a_ = DTensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = MonomialBasis(3)
        b = TensorBasis([m])
        f = Function(b,a_)
        x = 1.0
        self.assertEqual(type(f(x)), float)

    def test_function_evaluation_type2(self):
        a_ = [2,0,0,1]
        a_ = DTensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = MonomialBasis(3)
        b = TensorBasis([m])
        f = Function(b,a_)
        x = SX.sym('x',1,1)
        self.assertEqual(type(f(x)), casadi.SX)

    def test_function_evaluation_type3(self):
        a_ = SX.sym('a',3 + 1,1)
        a_ = STensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = MonomialBasis(3)
        b = TensorBasis([m])
        f = Function(b,a_)
        x = 1.0
        self.assertEqual(type(f(x)), casadi.SX)

    def test_function_evaluation_type4(self):
        a_ = SX.sym('a',3 + 1,1)
        a_ = STensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = MonomialBasis(3)
        b = TensorBasis([m])
        f = Function(b,a_)
        x = SX.sym('x',1,1)
        self.assertEqual(type(f(x)), casadi.SX)

    def test_function_evaluation_1(self):
        a_ = [2,0,0,1]
        a_ = DTensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = MonomialBasis(3)
        b = TensorBasis([m])
        f = Function(b,a_)
        self.assertEqual( f(0.0), 2 )
        self.assertEqual( f(1.0), 3 )
        self.assertEqual( f(2.0), 10 )

    def test_function_evaluation_2(self):
        a = DM([[1,1],[0,2]]) # 1 + y + 2xy
        a_ = DTensor(a, [2,2,1,1])
        a_ = Coefficient(a_)
        m = MonomialBasis(1)
        b = TensorBasis([m,m])
        f = Function(b,a_)
        for x in range(-5, 4):
            for y in range(-5, 4):
                self.assertEqual( f(x,y) , 1 + y + 2*x*y)

    def test_get_coefficient_tensor(self):
        a = DM([[1,1],[0,2]]) # 1 + y + 2xy
        a = DTensor(a, [2,2,1,1])
        a_ = Coefficient(a)
        m = MonomialBasis(1)
        b = TensorBasis([m,m])
        f = Function(b,a_)

        # print f.getCoeffTensor()
        # print a

        self.assertEqualT(a,f.getCoeffTensor())

    def test_insert_knots_univariate(self):
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
        b1 = BSplineBasis(knots1, degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1, c1)
        knots_add = np.random.rand(2)
        s2 = s1.insert_knots(knots_add)
        b2 = s2.getBasis()
        knots2 = np.sort(np.r_[knots_add, knots1])
        self.assertEqualT(knots2, b2.getKnots(), 1e-6)
        self.assertEqualT(degree, b2.getDegree(), 1e-6)
        g2 = b2.greville()
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)
        b = MonomialBasis(degree)
        with self.assertRaises(Exception):
            b.insert_knots(knots_add)
        # symbolic knot insertion
        knots_add_sym = MX.sym('knots_add', 2)
        eval_sym = MX.sym('eval')
        s2_sym = s1.insert_knots(knots_add_sym)
        s2_ev = casadi.Function('s2', [knots_add_sym, eval_sym], [s2_sym(eval_sym)])
        for i in g2:
            self.assertEqualT(s1(i), float(s2_ev(knots_add, i)), 1e-6)

#    def test_insert_knots_multivariate(self):
#        np.random.seed(0)
#        d1 = 3
#        nki1 = 8
#        k1 = np.r_[np.zeros(d1),np.linspace(0., 1., nki1), np.ones(d1)]
#        b1 = BSplineBasis(k1, d1)
#        ka1 = np.random.rand(2)
#        k1a = np.sort(np.r_[k1, ka1])
#        d2 = 2
#        nki2 = 5
#        k2 = np.r_[np.zeros(d2),np.linspace(0., 1., nki2), np.ones(d2)]
#        b2 = BSplineBasis(k2, d2)
#        ka2 = np.random.rand(3)
#        k2a = np.sort(np.r_[k2, ka2])
#        d3 = 6
#        b3 = MonomialBasis(d3)
#        B = TensorBasis([b1,b2,b3])
#        dims = B.dimension()
#        C = np.random.rand(dim[0], dim[1], dim[2])
#        f = Function(B,C)
#        f_a1 = f.insert_knots(ka1, 0)
#        f_a12 = f.insert_knots([ka1,ka2], [0,1])



#        f_a12 = f.insert_knots([ka1,ka2], [1,2])


    def test_midpoint_refinement(self):
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = np.r_[np.zeros(degree),np.linspace(0.,1.,knotsint),np.ones(degree)]
        b1 = BSplineBasis(knots1,degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1,c1)
        ref = 2
        s2 = s1.midpoint_refinement(ref)
        b2 = s2.getBasis()
        knots2 = np.r_[np.zeros(degree),np.linspace(0., 1., 2*ref*(knotsint-1)+1), np.ones(degree)]
        self.assertEqualT(knots2, b2.getKnots(), 1e-6)
        self.assertEqualT(degree, b2.getDegree(), 1e-6)
        g2 = b2.greville()
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)

    # def test_insert_knots_multivariate(self):
    #     return
    #     np.random.seed(0)
    #     degree = 3
    #     knotsint = 8
    #     knots = np.r_[np.zeros(degree),np.linspace(0.,1.,knotsint),np.ones(degree)]
    #     bs = BSplineBasis(knots,degree)
    #     bm = MonomialBasis(degree)
    #     b = TensorBasis([m])


if __name__ == '__main__':
    unittest.main()
