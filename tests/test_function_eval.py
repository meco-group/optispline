#!/usr/bin/env python

from helpers import *

from numpy.polynomial.polynomial import polyval

class Test_Function_Operations(BasisTestCase):


    def test_ones_base(self):
      a = 0.1
      x = Polynomial([0,1, 2],'x')


      self.assertEqualTensor(x(a), a+2*a**2)
      self.assertEqualTensor(x([a]), a+2*a**2)
      self.assertEqualTensor(x([a],["x"]), a+2*a**2)

      with self.assertRaises(Exception):
        x([a],["y"])

      a = [0.1, 0.2, 0.3]
      self.assertEqualTensor(x(a), np.array(a)+2*np.array(a)**2)

      a = np.array([[0.1, 0.2, 0.3]]).T

      self.assertEqualTensor(x(a), a+2*a**2)

      a = np.array([[0.1, 0.2, 0.3]])

      self.assertEqualTensor(x(a), a+2*a**2)

      a = np.array([0.1, 0.2, 0.3])

      self.assertEqualTensor(x(a), a+2*a**2)


    def test_two_bases(self):
      print "here"
      a = 0.1
      b = 0.13
      x = Polynomial([0,1, 2],'x')
      y = Polynomial([0,0.1, -3],'y')

      f = x*y

      with self.assertRaises(Exception):
        self.assertEqualTensor(f(a,b), f(b,a))

      self.assertEqualTensor(f(a,b), x(a)*y(b))
      self.assertEqualTensor(f(b,a), x(b)*y(a))



      self.assertEqualTensor(f([a,b]), x(a)*y(b))
      self.assertEqualTensor(f([b,a]), x(b)*y(a))


      self.assertEqualTensor(f([a,b],["x","y"]), x(a)*y(b))
      self.assertEqualTensor(f([a,b],["y","x"]), x(b)*y(a))

      self.assertEqualTensor(f([b,a],["x","y"]), x(b)*y(a))
      self.assertEqualTensor(f([b,a],["y","x"]), x(a)*y(b))

      with self.assertRaises(Exception):
        f([b,a],["y","z"])
      a = [0.1, 0.2, 0.3]
      b = [0.13, 0.17, 0.19]


      self.assertEqualTensor(f(a,b), x(a)*y(b))
      self.assertEqualTensor(f(b,a), x(b)*y(a))

      ab = np.array([a,b]).T
      ba = np.array([b,a]).T

      self.assertEqualTensor(f(ab), x(a)*y(b))
      self.assertEqualTensor(f(ba), x(b)*y(a))

      self.assertEqualTensor(f(ab,["x","y"]), x(a)*y(b))
      self.assertEqualTensor(f(ab,["y","x"]), x(b)*y(a))

      self.assertEqualTensor(f(ba,["x","y"]), x(b)*y(a))
      self.assertEqualTensor(f(ba,["y","x"]), x(a)*y(b))

    def test_partial_eval(self):
        x = Polynomial([0,1],'x')
        y2 = Polynomial([0,0,1],'y')
        func1 = x + y2

        x_ = 0.4
        y_ = 0.2

        f_part1 = func1.partial_eval(x_, 'x')
        f_part2 = f_part1(y_)

        f_part3 = func1.partial_eval(y_, 'y')
        f_party_ = f_part3(x_)

        self.assertEqualT(f_part2, f_party_)
        self.assertEqualT(f_party_, func1([x_, y_], ['x','y']))

    def test_partial_eval2(self):

        knots1 = [0,0,0,0,0.2,0.4,0.6,0.8,1,1,1,1]
        degree = 3
        basis1 = splines.BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,1,1]
        degree = 1
        basis2 = splines.BSplineBasis(knots2,degree)

        mbasis1 = TensorBasis([basis1,basis2], ['x','y']);
        coeff1 = DTensor(numpy.random.randn(*mbasis1.dimension()), mbasis1.dimension()+[1,1])
        print mbasis1
        func1 = Function(mbasis1,coeff1)

        x_ = 0.4
        y_ = 0.2

        f_part_x = func1.partial_eval(x_, 'x')
        f_part_x_ = f_part_x(y_)

        f_part_y = func1.partial_eval(y_, 'y')
        f_part_y_ = f_part_y(x_)

        self.assertEqualT(f_part_x_, f_part_y_)
        self.assertEqualT(f_part_y_, func1([x_, y_], ['x','y']))

if __name__ == '__main__':
    unittest.main()
