#!/usr/bin/env python

from helpers import *
from Basis import *

from numpy.polynomial.polynomial import polyval

class Test_Function_Operations(BasisTestCase):

    def test_polynomial_operations(self):

      t = DTensor([0, 1, 1, 0, 1, 0, 0, 1], [2, 2, 2])

      x = Polynomial([0,1],'x')
      b = x.getBasis()
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
if __name__ == '__main__':
    unittest.main()
