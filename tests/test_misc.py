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

     
if __name__ == '__main__':        
    unittest.main()

