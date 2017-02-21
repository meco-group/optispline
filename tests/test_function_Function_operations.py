#!/usr/bin/env python

from helpers import *

from numpy.polynomial.polynomial import polyval

class Test_Function_Operations(BasisTestCase):

    def test_polynomial_operations(self):

      t = DTensor([0, 1, 1, 0, 1, 0, 0, 1], [2, 2, 2])

      x = Polynomial([0,1],'x')
      b = x.basis()
      f = Function(b, t)
      f([0.2])

      p1 = Polynomial([0,0,1],'x')
      p2 = Polynomial([0,1],'y')

      x = Polynomial([0,1],'x')
      y = p2
      # f = x + y
      # f = y*x
      # f = x + y*x + y*y

#
#      # for p,poly in [
#      #                 (p1+p2, lambda x,y: x**2+y),
#      #                 (p1*p2, lambda x,y: x**2*y),
#      #                 (f, lambda x,y: x + y*x + y*y),
#      #                 (f + p1+p2, lambda x,y: x + y*x + y*y+x**2+y),
#      #                 (f + p1*p2, lambda x,y: x + y*x + y*y+x**2*y)
#      #               ]:
#
#      #   for arg in [ [0,0], [1,0], [0,1], [2,2], [0.7,1.3]]:
#      #     self.assertEqualTensor(p(*arg), poly(*arg))
#
#
#      # p1 = Polynomial([0,0,1],'x')
#      # p2 = Polynomial([0,1])
#
#      # for p,poly in [
#      #                 (p1+p2, lambda x: x**2+x),
#      #                 (p1*p2, lambda x: x**3),
#      #               ]:
#
#      #   for arg in [0,1,2,1.3]:
#      #     self.assertEqualTensor(p(arg), poly(arg))
#        x = Polynomial([0,1],'x')
#        y = p2
#        f = x + y
#        # f = y*x
#        # f = x + y*x + y*y

    def test_const_function(self):
        knots = [0,0,0.4,1,1]
        degree = 1
        basis = Basis.BSplineBasis(knots,degree)
        basis2 = Basis.MonomialBasis(3)

        c = Function.Constant(basis,1)
        self.assertEqualT(c(0.5),DTensor(1))

#    def test_const_function(self):
#        knots = [0,0,0.4,1,1]
#        degree = 1
#        basis = Basis.BSplineBasis(knots,degree)
#
#        c = Function.Constant(basis,1)
#        self.assertEqualT(c(0.5),DTensor(1))
#
#        c2 = Function.Constant(basis,DTensor([2,2],[1,2]))
#        self.assertEqualT(c2(0.5),DTensor([2,2],[1,2]))
#
#        basis2 = Basis.MonomialBasis(3)
#        c3 = Function.Constant(basis2,1)
#        self.assertEqualT(c3(0.5),DTensor(1))

    def test_const_function(self):
        knots = [0,0,0.4,1,1]
        degree = 1
        basis = Basis.BSplineBasis(knots,degree)
        basis2 = Basis.MonomialBasis(3)

        c = Function.Constant(basis,1)
        self.assertEqualT(c(0.5),DTensor(1))

        c2 = Function.Constant(basis,DTensor([2,2],[1,2]))
        self.assertEqualT(c2(0.5),DTensor([2,2],[1,2]))

        basis2 = Basis.MonomialBasis(3)
        c3 = Function.Constant(basis2,1)
        self.assertEqualT(c3(0.5),DTensor(1))

        mbasis = TensorBasis([basis,basis2]);
        f = Function.Constant(mbasis,2);
        f2 = Function.Constant(mbasis,[2,2]);

        self.assertEqualT(f(0.1,0.1), 2)
        self.assertEqualT(f2(0.2,0.2), DTensor([2,2]))

    def test_bspline_operation(self):
        knots1 = [0,0,0.4,1,1]
        degree = 1
        basis1 = Basis.BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,1,1]
        degree = 1
        basis2 = Basis.BSplineBasis(knots2,degree)

        coeff1 = DTensor(numpy.random.randn(3,1),[3,1,1])
        coeff2 = DTensor(numpy.random.randn(3,1),[3,1,1])
        func1 = Function(basis1,coeff1)
        func2 = Function(basis2,coeff2)

        p1 = Polynomial([0,0,1],'x')

        # function evaluations
        x = [0.1,0.35,0.4,0.5,0.8,0.99] #range(-1.,2.1,10)
        func1_value = []
        func2_value = []
        p1_value = []
        for v in x:
            func1_value.append(func1(v))
            func2_value.append(func2(v))
            p1_value.append(p1(v))

        # function operations
        u1 = -func1
        s1 = func1 + 2.0
        s3 = func1 + func1
        s2 = func1 + func2
        s4 = p1 + 3.3
        d1 = func2 - 5.0
        d2 = func2 - func1
        d3 = p1 - 2.0
        m1 = func1*2.0
        m2 = func1*func2
        m3 = p1*2.0
        p = func1**3
        pp = p1**4

        # assertion over grid
        k = 0
        for k in range(0,len(x)):
            v = x[k]
            self.assertAlmostEqual(s1(v),func1_value[k]+2.)
            self.assertAlmostEqual(s2(v),func1_value[k]+func2_value[k])
            self.assertAlmostEqual(u1(v),-func1_value[k])
            self.assertAlmostEqual(d1(v),func2_value[k]-5.)
            self.assertAlmostEqual(d2(v),func2_value[k]-func1_value[k])
            self.assertAlmostEqual(m1(v),func1_value[k]*2.)
            self.assertAlmostEqual(m2(v),func1_value[k]*func2_value[k])
            self.assertAlmostEqual(p(v),func1_value[k]**3)
            self.assertAlmostEqual(s4(v),p1_value[k]+3.3)
            self.assertAlmostEqual(d3(v),p1_value[k]-2.0)
            self.assertAlmostEqual(m3(v),p1_value[k]*2.0)
            self.assertAlmostEqual(pp(v),p1_value[k]**4)

    def test_multivariate_operations(self):
        knots1 = [0,0,0.4,1,1]
        degree = 1
        basis1 = Basis.BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,1,1]
        degree = 1
        basis2 = Basis.BSplineBasis(knots2,degree)

        basis3 = MonomialBasis(3);

        mbasis1 = TensorBasis([basis1,basis2]);
        mbasis2 = TensorBasis([basis2,basis3]);

        coeff1 = DTensor(numpy.random.randn(9,1),[3,3,1,1])
        coeff2 = DTensor(numpy.random.randn(12,1),[3,4,1,1])
        func1 = Function(mbasis1,coeff1)
        func2 = Function(mbasis2,coeff2)

        s1 = func1 + func2
        s2 = func1 + 2.0
        d1 = func1 - func2
        d2 = func1 - 3.0
        m1 = func1*func2
        m2 = func1*2.0
        u1 = -func1
        p1 = func1**3

        # function evaluations
        x = [0.1,0.35,0.4,0.5,0.8,0.99] #range(-1.,2.1,10)
        y = [0.1,0.2,0.5,0.8,0.1,0.2]
        func1_value = []
        func2_value = []
        for k in range(0,len(x)):
            _x = x[k]
            _y = y[k]
            func1_value.append(func1(_x,_y))
            func2_value.append(func2(_x,_y))

        k = 0
        for k in range(0,len(x)):
            _x = x[k]
            _y = y[k]

            self.assertAlmostEqual(s1(_x,_y),func1_value[k] + func2_value[k])
            self.assertAlmostEqual(s2(_x,_y),func1_value[k] + 2.0)
            self.assertAlmostEqual(d1(_x,_y),func1_value[k] - func2_value[k])
            self.assertAlmostEqual(d2(_x,_y),func1_value[k] - 3.0)
            self.assertAlmostEqual(m1(_x,_y),func1_value[k]*func2_value[k])
            self.assertAlmostEqual(m2(_x,_y),func1_value[k]*2.0)
            self.assertAlmostEqual(u1(_x,_y),-func1_value[k])
            self.assertAlmostEqual(p1(_x,_y),func1_value[k]**3)


    def test_matrix_product(self):
        knots1 = [0,0,0.4,1,1]
        degree = 1
        basis1 = Basis.BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,1,1]
        degree = 1
        basis2 = Basis.BSplineBasis(knots2,degree)

        basis3 = MonomialBasis(3);

        mbasis1 = TensorBasis([basis1,basis2]);
        mbasis2 = TensorBasis([basis2,basis3]);

        coeff1 = DTensor(numpy.random.randn(9*6,1),[3,3,3,2])
        coeff2 = DTensor(numpy.random.randn(12*8,1),[3,4,2,4])
        func1 = Function(mbasis1,coeff1)
        func2 = Function(mbasis2,coeff2)

        fm = func1.mtimes(func2)

        c = Function.Constant(mbasis1,1)
        fm2 = func1.mtimes(c)
        fm3 = c.mtimes(func2)
        
        M = numpy.random.randn(3,2);
        N = numpy.random.randn(2,2);
        fm4 = func1*M;
        fm5 = func1.mtimes(N);
        #fm6 = M*func2;

        x = [0.1,0.35,0.4,0.5,0.8,0.99]
        y = [0.1,0.2,0.5,0.8,0.1,0.2]
        func1_value = []
        func2_value = []
        for k in range(0,len(x)):
            _x = x[k]
            _y = y[k]
            func1_value.append(func1(_x,_y))
            func2_value.append(func2(_x,_y))

        k = 0
        for k in range(0,len(x)):
            _x = x[k]
            _y = y[k]

            self.assertEqualT(fm(_x,_y),numpy.dot(func1_value[k], func2_value[k]))
            self.assertEqualT(fm2(_x,_y),func1_value[k])
            self.assertEqualT(fm3(_x,_y),func2_value[k])
            self.assertEqualT(fm4(_x,_y),func1_value[k]*M)
            self.assertEqualT(fm5(_x,_y),numpy.dot(func1_value[k],N))
            
    def test_trace(self):
        knots1 = [0,0,0.4,1,1]
        degree = 1
        basis1 = Basis.BSplineBasis(knots1,degree)

        basis2 = MonomialBasis(2);

        mbasis = TensorBasis([basis1,basis2]);

        coeff1 = DTensor(numpy.random.randn(81,1),[3,3,3,3])
        coeff2 = DTensor(numpy.random.randn(9*12,1),[3,3,3,4])
        func1 = Function(mbasis,coeff1)
        func2 = Function(mbasis,coeff2)
        tr = func1.trace()

        with self.assertRaises(Exception):
            DTensor.concat(func2.trace())

        N = 10
        x = numpy.random.random(N)
        y = numpy.random.random(N)
        for k in range(0,N):
            _x = x[k]
            _y = y[k]
            self.assertAlmostEqual(numpy.trace(func1(_x,_y)), tr(_x,_y))
        self.assertEqualT(tr.shape, [1,1])

    def test_slice(self):
        knots1 = [0,0,0.4,1,1]
        degree = 1
        basis1 = Basis.BSplineBasis(knots1,degree)

        basis2 = MonomialBasis(2);

        mbasis = TensorBasis([basis1,basis2]);
        
        numpy.random.seed(0)

        coeff2 = DTensor(numpy.random.randn(9*12,1),[3,3,3,4])
        func2 = Function(mbasis,coeff2)
        
        slices = ([0,1],[2,1])
        
        f = func2.slice(*slices)
        
        xy = numpy.random.random((2,1))
        x = float(xy[0])
        y = float(xy[1])
        
        self.assertEqualT(func2(x,y)[np.ix_(slices[0],slices[1])], f(x,y))

if __name__ == '__main__':
    unittest.main()
