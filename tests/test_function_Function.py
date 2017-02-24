#!/usr/bin/env python

from helpers import *
from Basis import vertcat

class Test_Function_Function(BasisTestCase):

    def test_construct_function(self):
        a = MX.sym('a',3 + 1,1)
        a = MTensor(a, [3 + 1,1,1])
        a = Coefficient(a)
        m = MonomialBasis(3)
        b = TensorBasis([m])
        f = Function(b,a)

        with self.assertRaises(Exception):
          f(np.array([2.0,4.0]))

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
        self.assertEqualT(a,f.coeff_tensor())

    def test_insert_knots_univariate(self):
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
        b1 = BSplineBasis(knots1, degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1, c1)
        knots_add = np.random.rand(2)
        # knots_add = 0.1
        s2 = s1.insert_knots(knots_add)
        b2 = s2.basis()
        knots2 = np.sort(np.r_[knots_add, knots1])
        self.assertEqualT(knots2, b2.knots(), 1e-6)
        self.assertEqualT(degree, b2.degree(), 1e-6)
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

    def test_insert_knots_multivariate(self):
        np.random.seed(0)
        d1 = 3
        nki1 = 8
        k1 = np.r_[np.zeros(d1),np.linspace(0., 1., nki1), np.ones(d1)]
        b1 = BSplineBasis(k1, d1)
        ka1 = np.random.rand(2)
        k1a = np.sort(np.r_[k1, ka1])
        d2 = 2
        nki2 = 5
        k2 = np.r_[np.zeros(d2),np.linspace(0., 1., nki2), np.ones(d2)]
        b2 = BSplineBasis(k2, d2)
        ka2 = np.random.rand(3)
        k2a = np.sort(np.r_[k2, ka2])
        d3 = 6
        b3 = MonomialBasis(d3)
        B = TensorBasis([b1,b2,b3], ['x', 'y', 'z'])
        dims = B.dimension()
        C = np.random.rand(dims[0], dims[1], dims[2])
        f = Function(B,C)
        f_a1 = f.insert_knots(ka1, 0)
        f_a12 = f.insert_knots([ka1,ka2], ['x', 'y'])
        b1_2 = f_a12.basis(0)
        b2_2 = f_a12.basis(1)
        self.assertEqualT(k1a, b1_2.knots(), 1e-6)
        self.assertEqualT(d1, b1_2.degree(), 1e-6)
        self.assertEqualT(k2a, b2_2.knots(), 1e-6)
        self.assertEqualT(d2, b2_2.degree(), 1e-6)
        g1_2 = b1_2.greville()
        g2_2 = b2_2.greville()
        for i in g1_2:
            for j in g2_2:
                for k in np.linspace(0, 1, d3+1):
                    self.assertEqualT(f(i,j,k), f_a12(i,j,k), 1e-6)

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
        b2 = s2.basis()
        knots2 = np.r_[np.zeros(degree),np.linspace(0., 1., 2*ref*(knotsint-1)+1), np.ones(degree)]
        self.assertEqualT(knots2, b2.knots(), 1e-6)
        self.assertEqualT(degree, b2.degree(), 1e-6)
        g2 = b2.greville()
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)

    def test_degree_elevation(self):
        return
        np.random.seed(0)
        d1 = 3
        nki1 = 8
        k1 = np.r_[np.zeros(d1),np.linspace(0., 1., nki1), np.ones(d1)]
        b1 = BSplineBasis(k1, d1)
        elev1 = 2
        d2 = 2
        nki2 = 5
        k2 = np.r_[np.zeros(d2),np.linspace(0., 1., nki2), np.ones(d2)]
        b2 = BSplineBasis(k2, d2)
        elev2 = 1
        d3 = 6
        b3 = MonomialBasis(d3)
        elev3 = 4
        B = TensorBasis([b1,b2,b3], ['x', 'y', 'z'])
        dims = B.dimension()
        C = np.random.rand(dims[0], dims[1], dims[2])
        f = Function(B,C)
        f_e123 = f.degree_elevation([elev1, elev3, elev2], ['x', 'z', 'y'])
        be_1 = f_e123.basis(0)
        be_2 = f_e123.basis(1)
        be_3 = f_e123.basis(2)
        self.assertEqualT(be_1.degree(), d1+elev1)
        self.assertEqualT(be_2.degree(), d2+elev2)
        self.assertEqualT(be_3.degree(), d3+elev3)
        self.assertEqualT(be_1.knots(), np.r_[np.zeros(d1), sorted(np.linspace(0, 1, nki1).tolist()*elev1), np.ones(d1)])
        self.assertEqualT(be_2.knots(), np.r_[np.zeros(d2), sorted(np.linspace(0, 1, nki2).tolist()*elev2), np.ones(d2)])
        g1 = be_1.greville()
        g2 = be_2.greville()
        for i in g1:
            for j in g2:
                for k in range(d3+1):
                    self.assertEqualT(f(i,j,k), f_e123(i,j,k), 1e-6)
        f.degree_elevation(3)
        f.degree_elevation(3, [1])

    def test_kick_boundary(self):
        return
        np.random.seed(0)
        d1 = 3
        nki1 = 8
        k1 = np.r_[np.zeros(d1),np.linspace(0., 1., nki1), np.ones(d1)]
        b1 = BSplineBasis(k1, d1)
        dom1 = [0.05, 1.1]
        d2 = 2
        nki2 = 5
        k2 = np.r_[np.zeros(d2),np.linspace(0., 1., nki2), np.ones(d2)]
        b2 = BSplineBasis(k2, d2)
        dom2 = [-0.1, 0.95]
        d3 = 6
        b3 = MonomialBasis(d3)
        B = TensorBasis([b1,b2,b3], ['x', 'y', 'z'])
        dims = B.dimension()
        C = np.random.rand(dims[0], dims[1], dims[2])
        f = Function(B,C)
        f_e123 = f.kick_boundary([dom2, dom1], ['y', 'x'])
        be_1 = f_e123.basis(0)
        be_2 = f_e123.basis(1)
        be_3 = f_e123.basis(2)
        self.assertEqualT(be_1.degree(), d1)
        self.assertEqualT(be_2.degree(), d2)
        self.assertEqualT(be_3.degree(), d3)
        self.assertEqualT(be_1.knots(), np.r_[dom1[0]*np.ones(d1), np.linspace(dom1[0], dom1[1], nki1), dom1[1]*np.ones(d1)])
        self.assertEqualT(be_2.knots(), np.r_[dom2[0]*np.ones(d2), np.linspace(dom2[0], dom1[1], nki2), dom2[1]*np.ones(d2)])
        g1 = be_1.greville()
        g2 = be_2.greville()
        for i in g1:
            for j in g2:
                for k in range(d3+1):
                    self.assertEqualT(f(i,j,k), f_e123(i,j,k), 1e-6)
        f.kick_boundary(dom1, [1])
        with self.assertRaises(Exception):
            f.insert_knots(dom1)

    def test_derivative_multivariate(self):
        d0 = 4
        k0 = np.r_[np.zeros(d0),np.linspace(0.,1.,8),np.ones(d0)]
        b0 = BSplineBasis(k0,d0)
        n_der0 = 2
        db0,T0 = b0.derivative(n_der0)
        g0 = db0.greville()
        x0 = casadi.SX.sym('x0')
        db0_c = b0([x0])
        for i in range(0,n_der0):
            db0_c = casadi.jacobian(db0_c, x0)
        db0_c = casadi.Function('db0_c', [x0], [db0_c])

        d1 = 3
        b1 = MonomialBasis(d1)
        n_der1 = 1
        db1 = b1.derivative(n_der1)
        g1 = db0.greville()
        x1 = casadi.SX.sym('x1')
        db1_c = b1([x1])
        for i in range(0,n_der1):
            db1_c = casadi.jacobian(db1_c, x1)
        db1_c = casadi.Function('db1_c', [x1], [db1_c])

        c = np.random.rand(b0.dimension(),b1.dimension())
        f = Function(TensorBasis([b0,b1], ['x', 'y']), c)
        df = f.derivative([n_der0, n_der1], [0,1])

        for i0 in g0:
            for i1 in g1:
                self.assertEqualT(np.array(db0_c(i0)).T.dot(c).dot(db1_c(i1))[0][0], df(i0,i1))

        f.derivative(n_der0, 0)
        f.derivative(n_der0, 'x')
        f.derivative([n_der0, n_der1], ['x', 'y'])
        f.derivative(n_der0)

    def test_derivative_polynomial(self):
        deg = 4
        p = Polynomial([1,2,3,4])
        dp = p.derivative([1],[0])
        ddp = p.derivative([2],[0])
        dddp = p.derivative([3],[0])
        ddddp = p.derivative([deg],[0])

        self.assertEqualT(p.coeff().data().reshape(p.basis().degree()+1,), [1,2,3,4])
        self.assertEqualT(dp.coeff().data().reshape(dp.basis().degree()+1,), [2,6,12])
        self.assertEqualT(ddp.coeff().data().reshape(ddp.basis().degree()+1,), [6,24])
        self.assertEqualT(dddp.coeff().data().reshape(dddp.basis().degree()+1,), [24])
        self.assertEqualT(ddddp.coeff().data().reshape(dddp.basis().degree()+1,), [0])

    def test_transform_to(self):
        b = BSplineBasis([0, 1], 3, 2)
        p = Polynomial(np.random.randn(4))
        f = p.transform_to(TensorBasis(b))
        for x in np.random.random(10):
            self.assertAlmostEqual(f(x), p(x))

    def test_function_constant(self):
        f = Function(2)
        for x in np.random.random(10):
            self.assertAlmostEqual(f(x), 2)

    def test_antiderivative(self):
        d0 = 4
        k0 = np.r_[np.zeros(d0),np.linspace(0.,1.,7),np.ones(d0)]
        b0 = BSplineBasis(k0,d0)
        g0 = b0.greville()
        n0 = 2
        init0 = np.random.rand(n0);

        d1 = 3
        b1 = MonomialBasis(d1)
        g1 = range(d1+1)
        n1 = 1
        init1 = np.random.rand(n1);

        c = np.random.rand(b0.dimension(),b1.dimension())
        f = Function(TensorBasis([b0,b1], ['x', 'y']), c)
        ff = f.antiderivative([n0, n1], [0,1]).derivative([n0, n1], [0,1])
        for i0 in g0:
            for i1 in g1:
                self.assertEqualT(f(i0,i1), ff(i0,i1), 1e-6)
        f.antiderivative(n0, 0)
        f.antiderivative(n0, 'x')
        f.antiderivative([n0, n1], ['x', 'y'])
        f.derivative(n0)

    def test_jacobian(self):
       if valgrind: return
       d0 = 4
       k0 = np.r_[np.zeros(d0),np.linspace(0.,1.,7),np.ones(d0)]
       b0 = BSplineBasis(k0,d0)
       g0 = np.sort(np.r_[b0.greville(), b0.derivative()[0].greville()])

       d1 = 6
       k1 = np.r_[np.zeros(d1),np.linspace(0.,1.,7),np.ones(d1)]
       b1 = BSplineBasis(k1,d1)
       g1 = np.sort(np.r_[b1.greville(), b1.derivative()[0].greville()])

       d2 = 3
       b2 = MonomialBasis(d2)
       g2 = range(d2+1)

       c = np.random.rand(b0.dimension(),b1.dimension(),b2.dimension())
       f = Function(TensorBasis([b0,b1,b2]), c)
       Jf = f.jacobian()

       x = casadi.SX.sym('x',3)
       f_c = f(x[0], x[1], x[2])
       Jf_c = casadi.jacobian(f_c, x)
       Jf_c = casadi.Function('Jf_c', [x], [Jf_c])
       for i0 in g0:
           for i1 in g1:
               for i2 in g2:
                    for idx in range(len(Jf)):
                        self.assertEqualT(Jf[idx](i0,i1,i2), float(Jf_c([i0,i1,i2])[idx]), 1e-6)

    def test_integral(self):
        np.random.seed(0)
        d1 = 3
        nki1 = 8
        k1 = np.r_[np.zeros(d1), np.linspace(0., 1., nki1), np.ones(d1)]
        b1 = BSplineBasis(k1, d1)
        ka1 = np.random.rand(2)
        k1a = np.sort(np.r_[k1, ka1])
        d2 = 2
        nki2 = 5
        k2 = np.r_[np.zeros(d2), np.linspace(0., 1., nki2), np.ones(d2)]
        b2 = BSplineBasis(k2, d2)
        ka2 = np.random.rand(3)
        k2a = np.sort(np.r_[k2, ka2])
        d3 = 6
        b3 = MonomialBasis(d3)
        B = TensorBasis([b1, b2, b3], ['x', 'y', 'z'])
        dims = B.dimension()
        C = np.random.rand(dims[0], dims[1], dims[2])
        f = Function(B,C)
        f1 = Function(b1, np.random.rand(b1.dimension()))
        f2 = Function(b2, np.random.rand(b2.dimension()))
        f3 = Function(b3, np.random.rand(b3.dimension()))
        B2 = TensorBasis([b1, b2], ['x', 'y'])
        B3 = TensorBasis([b1, b3], ['x', 'y'])
        C2 = np.random.rand(dims[0], dims[1])
        C3 = np.random.rand(dims[0], dims[2])
        f12 = Function(B2, C2)
        f13 = Function(B3, C3)

        f1_antider = f1.antiderivative()
        f2_antider = f2.antiderivative()
        f3_antider = f3.antiderivative()
        f12_antider = f12.antiderivative()
        f13_antider = f13.antiderivative()

        self.assertEqualT(f2.integral(), f2_antider(1.) - f2_antider(0.), 1e-6)
        self.assertEqualT(f3.integral(), inf, 1e-6)
        self.assertEqualT(f12.integral(), f12_antider(1., 1.) - f12_antider(0., 0.), 1e-6)
        self.assertEqualT(f13.integral(), inf, 1e-6)

        self.assertEqualT(f1.integral([0.1, 0.9]), f1_antider(0.9) - f1_antider(0.1), 1e-6)
        self.assertEqualT(f2.integral([0.1, 0.8]), f2_antider(0.8) - f2_antider(0.1), 1e-6)
        self.assertEqualT(f3.integral([-2.1, 0.6]), f3_antider(0.6) - f3_antider(-2.1), 1e-6)
        f12_int1 = f12.integral([[0.1, 0.8], [0.2, 0.9]])
        f12_int1b = f12.integral(TensorDomain([[0.2, 0.9], [0.1, 0.8]], ['y', 'x']))
        f12_int2 = f12_antider(0.8, 0.9) - f12_antider(0.8, 0.2) - f12_antider(0.1, 0.9) + f12_antider(0.1, 0.2)
        self.assertEqualT(f12_int1, f12_int2, 1e-6)
        self.assertEqualT(f12_int1b, f12_int2, 1e-6)
        f13_int1 = f13.integral([[0.1, 0.8], [0.2, 0.9]])
        f13_int2 = f13_antider(0.8, 0.9) - f13_antider(0.8, 0.2) - f13_antider(0.1, 0.9) + f13_antider(0.1, 0.2)
        self.assertEqualT(f13_int1, f13_int2, 1e-6)
        fv = vertcat(f12, f13)
        self.assertEqualT(fv.integral([[0.1, 0.8], [0.2, 0.9]]), np.vstack((f12_int2, f13_int2)), 1e-6)

    def test_partial_integral(self):
        np.random.seed(0)
        d1 = 3
        nki1 = 8
        k1 = np.r_[np.zeros(d1), np.linspace(0., 1., nki1), np.ones(d1)]
        b1 = BSplineBasis(k1, d1)
        ka1 = np.random.rand(2)
        k1a = np.sort(np.r_[k1, ka1])
        d2 = 2
        nki2 = 5
        k2 = np.r_[np.zeros(d2), np.linspace(0., 1., nki2), np.ones(d2)]
        b2 = BSplineBasis(k2, d2)
        ka2 = np.random.rand(3)
        k2a = np.sort(np.r_[k2, ka2])
        d3 = 6
        b3 = MonomialBasis(d3)
        B = TensorBasis([b1, b2, b3], ['x', 'y', 'z'])
        dims = B.dimension()
        C = np.random.rand(dims[0], dims[1], dims[2])
        f = Function(B,C)
        f1 = Function(b1, np.random.rand(b1.dimension()))
        f2 = Function(b2, np.random.rand(b2.dimension()))
        f3 = Function(b3, np.random.rand(b3.dimension()))
        B2 = TensorBasis([b1, b2], ['x', 'y'])
        B3 = TensorBasis([b1, b3], ['x', 'y'])
        C2 = np.random.rand(dims[0], dims[1])
        C3 = np.random.rand(dims[0], dims[2])
        f12 = Function(B2, C2)
        f13 = Function(B3, C3)
        f123 = Function(TensorBasis([b1, b2, b3], ['x', 'y', 'z']), np.random.rand(dims[0], dims[1], dims[2]))

        f12_antider_y = f12.antiderivative([1], ['y'])
        fx = f12.partial_integral([0.5, 0.8], ['y'])
        gx = fx.basis().greville()
        for g in gx:
            self.assertEqualT(fx(g), f12_antider_y(g, 0.8) - f12_antider_y(g, 0.5), 1e-6)

        f123_antider_yz = f123.antiderivative([1, 1], ['y', 'z'])
        dom1 = TensorDomain([[-0.1, 0.9], [0.5, 0.6]], ['z', 'y'])
        dom2 = TensorDomain([[0.5, 0.6], [-0.1, 0.9]])
        fx = f123.partial_integral(dom1, ['y', 'z'])
        fx2 = f123.partial_integral(dom2, ['y', 'z'])
        fx3 = f123.partial_integral(dom2, [1, 2])
        gx = fx.basis().greville()
        self.assertEqual(fx.tensor_basis().arguments()[0], 'x')
        for g in gx:
            self.assertEqualT(fx(g), f123_antider_yz(g, 0.6, 0.9) - f123_antider_yz(g, 0.6, -0.1) - f123_antider_yz(g, 0.5, 0.9) + f123_antider_yz(g, 0.5, -0.1))
            self.assertEqualT(fx2(g), f123_antider_yz(g, 0.6, 0.9) - f123_antider_yz(g, 0.6, -0.1) - f123_antider_yz(g, 0.5, 0.9) + f123_antider_yz(g, 0.5, -0.1))
            self.assertEqualT(fx3(g), f123_antider_yz(g, 0.6, 0.9) - f123_antider_yz(g, 0.6, -0.1) - f123_antider_yz(g, 0.5, 0.9) + f123_antider_yz(g, 0.5, -0.1))


if __name__ == '__main__':
    unittest.main()
