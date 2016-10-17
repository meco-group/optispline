#!/usr/bin/env python

import unittest
from Basis import *
from casadi import *
from helpers import BasisTestCase

class Test_Function_Polynomial(BasisTestCase):

    def test_trivial(self):
      a = [0,2,1]
      p = Polynomial(a, 'x')
      for e in [1,0.5,3]:
        self.assertEqualTensor(p([e]), self.polyval(a,e))

if __name__ == '__main__':
    unittest.main()
