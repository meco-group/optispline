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

      a = 0.1
      b = 0.2
      c = 1.2
      self.assertEqualTensor(x([a,a],["x","y"]),x([a],["x"]))
      print((x([a],["x"])))
      print((x([a,b,c],["x","y", "z"])))
      print((x([b,a],["y","x"])))
      self.assertEqualTensor(x([a,b],["x","y"]),x([a],["x"]))
      self.assertEqualTensor(x([b,a],["y","x"]),x([a],["x"]))

      with self.assertRaises(Exception):
        x([a],["y"])
        x([a,b],["x", "x"])
        x([a],["x", "y"])


      a = [0.1, 0.2, 0.3]
      self.assertEqualTensor(x.list_eval(a), np.array(a)+2*np.array(a)**2)

      a = np.array([[0.1, 0.2, 0.3]]).T

      self.assertEqualTensor(x.list_eval(a), a+2*a**2)

      a = np.array([[0.1, 0.2, 0.3]])

      self.assertEqualTensor(x.list_eval(a), a+2*a**2)

      a = np.array([0.1, 0.2, 0.3])

      self.assertEqualTensor(x.list_eval(a), a+2*a**2)


    def test_two_bases(self):
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


      self.assertEqualTensor(f.list_eval(a,b), x.list_eval(a)*y.list_eval(b))
      self.assertEqualTensor(f.list_eval(b,a), x.list_eval(b)*y.list_eval(a))

      ab = np.array([a,b]).T
      ba = np.array([b,a]).T

      self.assertEqualTensor(f.list_eval(ab), x.list_eval(a)*y.list_eval(b))
      self.assertEqualTensor(f.list_eval(ba), x.list_eval(b)*y.list_eval(a))

      self.assertEqualTensor(f.list_eval(ab,["x","y"]), x.list_eval(a)*y.list_eval(b))
      self.assertEqualTensor(f.list_eval(ab,["y","x"]), x.list_eval(b)*y.list_eval(a))

      self.assertEqualTensor(f.list_eval(ba,["x","y"]), x.list_eval(b)*y.list_eval(a))
      self.assertEqualTensor(f.list_eval(ba,["y","x"]), x.list_eval(a)*y.list_eval(b))

      with self.assertRaises(Exception):
        f.list_eval([b,a],["y","z"])

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

        f_part_z = func1.partial_eval(x_, 'z')
        self.assertEqualT( func1([x_, y_], ['x','y']), f_part_z([x_, y_], ['x','y']))


    def test_partial_eval2(self):

        knots1 = [0,0,0,0,0.2,0.4,0.6,0.8,1,1,1,1]
        degree = 3
        basis1 = splines.BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,1,1]
        degree = 1
        basis2 = splines.BSplineBasis(knots2,degree)

        mbasis1 = TensorBasis([basis1,basis2], ['x','y']);
        coeff1 = DTensor(numpy.random.randn(*mbasis1.dimension()), mbasis1.dimension()+[1,1])
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
