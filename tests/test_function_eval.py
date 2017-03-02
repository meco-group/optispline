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


if __name__ == '__main__':
    unittest.main()
