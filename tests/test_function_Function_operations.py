#!/usr/bin/env python

from helpers import *

from numpy.polynomial.polynomial import polyval

class Test_Function_Operations(BasisTestCase):

    def test_polynomial_operations(self):

      p1 = Polynomial([0,0,1],'x')
      p2 = Polynomial([0,1],'y')

      x = Polynomial([0,1],'x')
      y = p2
      f = x + y
      f = y*x
      f = x + y*x + y*y


      for p,poly in [
                      (p1+p2, lambda x,y: x**2+y),
                      (p1*p2, lambda x,y: x**2*y),
                      (f, lambda x,y: x + y*x + y*y),
                      (f + p1+p2, lambda x,y: x + y*x + y*y+x**2+y),
                      (f + p1*p2, lambda x,y: x + y*x + y*y+x**2*y)
                    ]:

        for arg in [ [0,0], [1,0], [0,1], [2,2], [0.7,1.3]]:
          self.assertEqualTensor(p(*arg), poly(*arg))


      p1 = Polynomial([0,0,1],'x')
      p2 = Polynomial([0,1])

      for p,poly in [
                      (p1+p2, lambda x: x**2+x),
                      (p1*p2, lambda x: x**3),
                    ]:

        for arg in [0,1,2,1.3]:
          self.assertEqualTensor(p(arg), poly(arg))

if __name__ == '__main__':
    unittest.main()
