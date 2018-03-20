#!/usr/bin/env python

from helpers import *


class Test_Misc(BasisTestCase):
  def test_abc(self):
    np.random.seed(0)
    d1 = 3
    nki1 = 8
    k1 = np.r_[np.zeros(d1),np.linspace(0., 1., nki1), np.ones(d1)]
    b1 = BSplineBasis(k1, d1)
    C = casadi.MX(np.random.rand(b1.dimension()))
    f = Function(b1,C)
        
    f **2

  def test_jac_test(self):
    degree = 1
    knots = np.r_[np.zeros(degree), np.linspace(0, 1,2), np.ones(degree)]
    basis = BSplineBasis(knots, degree)

    X = MX.sym('X', basis.dimension())
    x = Function(basis, X)

    expr = x.derivative()
    expr = expr.data()

    jac = jacobian(expr, X)

    self.assertTrue(len(symvar(jac))==0)

    x = Function(basis, X**2)

    expr = x.derivative()
    expr = expr.data()

    jac = jacobian(expr, X)

    self.assertFalse(len(symvar(jac))==0)

  def test_numpy(self):
    np.random.seed(0)
    d1 = 3
    nki1 = 8
    k1 = np.r_[np.zeros(d1),np.linspace(0., 1., nki1), np.ones(d1)]
    b1 = BSplineBasis(k1, d1)
    C = casadi.MX(np.random.rand(b1.dimension()))
    f = Function(b1,C)
    np.array([f,f])<0
    pass
         
if __name__ == '__main__':        
    unittest.main()

