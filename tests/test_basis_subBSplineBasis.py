#!/usr/bin/env python

from helpers import *
import pdb

def wrap(s, casadiType):
    def eval(a):
        x = [casadiType.sym('x') for i in a]
        temp = C.Function('temp',x,[s(*x)])
        return DTensor(temp(a),[s.dimension()])
    return eval

class Test_Basis_SubBSpline(BasisTestCase):

    def test_BSpline_constructor(self):
        x1L = -0.65;    x1U = 0.7;
        x1 = Polynomial([0, 1], 'x1')
        x1 = x1.transform_to(BSplineBasis([x1L,x1U],1,2))
        self.assertEqual((x1*x1).coeff_tensor().size, 3)

        x2L = -0.7;    x2U = 0.7;
        x2 = Polynomial([0, 1], 'x2')
        x2 = x2.transform_to(BSplineBasis([x2L,x2U],1,2))
        self.assertEqual((x2*x2).coeff_tensor().size, 3)

        x2L = -0.8;    x2U = 0.7;
        x2 = Polynomial([0, 1], 'x2')
        x2_2 = x2.transform_to(BSplineBasis([x2L,x2U],1,2))
        x2_3 = x2.transform_to(BSplineBasis([x2L,x2U],1,3))
        x2_5 = x2.transform_to(BSplineBasis([x2L,x2U],1,5))
        x2_21 = x2.transform_to(BSplineBasis([x2L,x2U],1,21))
        self.assertEqual((x2_3+x2_3).coeff_tensor().size, 3)
        self.assertEqual((x2_5+x2_5).coeff_tensor().size, 5)
        self.assertEqual((x2_3+x2_5).coeff_tensor().size, 5)
        self.assertEqual((x2_21+x2_3).coeff_tensor().size, 21)
        self.assertEqual((x2_21+x2_5).coeff_tensor().size, 21)

        self.assertEqual((x2_3*x2_3).coeff_tensor().size, 5)
        self.assertEqual((x2_5*x2_5).coeff_tensor().size, 9)
        self.assertEqual((x2_3*x2_5).coeff_tensor().size, 9)
        self.assertEqual((x2_21*x2_3).coeff_tensor().size, 41)
        self.assertEqual((x2_21*x2_5).coeff_tensor().size, 41)

        x = BSplineBasis([0,1.2],1,5)
        y = BSplineBasis([0,0,0.3,0.6,0.9,1.2,1.2],1)
        print((x+y).knots())

    def test_dimension1(self):
        s = BSplineBasis([0,0,0,0.5,1,1,1], 2)
        # number of basis functions = number of knots - degree - 1
        self.assertEqual(s.dimension(), 4)

    def test_dimension2(self):
        s = BSplineBasis([0,0,0,0.25,0.5,0.75,1,1,1], 2)
        # number of basis functions = number of knots - degree - 1
        self.assertEqual(s.dimension(), 6)

    def test_dimension2(self):
        s = BSplineBasis([0,0,0,0,0,0.25,0.5,0.75,1,1,1,1,1], 4)
        # number of basis functions = number of knots - degree - 1
        self.assertEqual(s.dimension(), 8)

    def test_n_inputs(self):
        s = BSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.n_inputs(), 1)

    def test_degree(self):
        s = BSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.degree(), 2)

    def test_evaluation1(self):
        s = BSplineBasis([0,0,0,0.5,1,1,1], 2)
        r = s([0.1])
        self.assertEqual(r.shape[0], s.dimension())

    def test_evaluation2(self):
        S = BSplineBasis([0,0,0,0.2,0.5,0.7,1,1,1], 2)

        for s in [S, wrap(S,SX), wrap(S,MX)]:
            for arg, check in [
                ([0.0], [1,0,0,0,0,0]),
                ([0.5], [0,0,0.4,0.6,0,0]),
                ([1.0], [0,0,0,0,0,1])
                ]:
                self.assertEqualTensor(s(arg),check)

        S = BSplineBasis([0,0,0.2,0.5,0.7,1,1], 2)

        # Not possible to do with SX: requires conditional division
        for s in [S, wrap(S,MX)]:
            for arg, check in [
                ([0.0], [0,0,0,0]),
                ([0.5], [0,0.4,0.6,0]),
                ([1.0], [0,0,0,0])
                ]:
                self.assertEqualTensor(s(arg),check)
    def test_derivative(self):
      np.random.seed(0)
      x = C.MX.sym("x")

      N = 15

      B = BSplineBasis([0,0,0] + list(np.linspace(0,1,N)) + [1,1,1],3)
      t = TensorBasis([B])

      c = np.random.random(*t.dimension())
      print((np.shape(c)))
      c = DTensor(c)
      print((c.dims()))
      # c = c.shape(c.dims()+[ 1])
      c = Coefficient(c)

      print("____")
      print((c.shape()))
      print((c.dimension()))

      f = Function(B,c)
      # print(f(x).dims())
      F = C.Function("F",[x],[f(x)])

      J = C.Function("F",[x],[C.jacobian(f(x),x)])
      ts = np.linspace(0,1,1000)

#       def FunDerivative(self):
#         degree = self.basis().degree()
#         coeffs = self.coeff().data()
#         knots = np.array(self.basis().knots())
#         n = coeffs.shape[0]
#         delta_knots = knots[1+degree:-1] - knots[1:-degree-1]
#         T = np.zeros((n - 1, n))
#         j = np.arange(n - 1)
#         T[j, j] = -1. / delta_knots
#         T[j, j + 1] = 1. / delta_knots
#         P = degree * T
#         m = BSplineBasis(knots[1:-1], degree-1)
#         basis = TensorBasis([m])
#         return Function(basis, C.mtimes(P, coeffs))

#       Function.derivative = FunDerivative


#       j = f.derivative()

#       s = [j(i) for i in ts]

#       S = [float(J(i)) for i in ts]

#       self.assertEqualArray(s,S,tol=1e-12)

if __name__ == '__main__':
    unittest.main()
