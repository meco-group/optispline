#!/usr/bin/env python

from helpers import *

from numpy.polynomial.polynomial import polyval

class Test_Function_Operations(BasisTestCase):

    def test_eval(self):

      x = Polynomial([0,1],'x')
      r = [i**2*0.01 for i in range(11)]
      r2 = [[i*0.1,0.1*i] for i in range(11)]
      print r
      # print x(0.5)
      # print x(r)
      print "print x.call([0.5])"
      print x.call([0.5])
      print x(0.5)
      print x(0.5, ['x'])
      print "print x.call(r)"
      print x.call(r)
      print x(r)
      print x(r, ['x'])
      # self.assertEqualT(r, x(r), 1e-6)

      y = Polynomial([0,1],'y')
      f = x * y
      print "print r2"
      print r2

      # print f(0.5, 0.5)
      # print f(r2)
      print "print f.call([0.5, 0.5])"
      print f.call([0.5, 0.5])
      print f([0.5, 0.5])
      print f([0.5, 0.5], ['x'])
      print "print f.call(r2)"
      print f.call(r2)
      print f(r2)
      print f(r2, ['x'])

    def test_partial_eval(self):
        return
        numpy.random.seed(0)

        knots1 = [0,0,0,0,0.2,0.4,0.6,0.8,1,1,1,1]
        degree = 3
        basis1 = splines.BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,1,1]
        degree = 1
        basis2 = splines.BSplineBasis(knots2,degree)

        mbasis1 = TensorBasis([basis1,basis2], ['x','y']);
        coeff1 = DTensor(numpy.random.randn(18,1),[6,3,1,1])
        func1 = Function(mbasis1,coeff1)

        f_part1 = func1.f_partial(10, 'x')
        f_part2 = f_part1(4)

        f_part3 = func1.f_partial(4, 'y')
        f_part4 = f_part1(10)

        self.assertEqualT(f_part2, f_part4)
        self.assertEqualT(f_part4, func1([10, 4], ['x','y']))

if __name__ == '__main__':
    unittest.main()
