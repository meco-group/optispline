#!/usr/bin/env python

import unittest
import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

from Basis import *
from casadi import *

import pdb

class Test_Basis_SubBasis(unittest.TestCase):

    def assertEqualTensor(self, a, b):
        try:
          a = a.data()
        except:
          pass
        self.assertTrue(list(a.full())==b)

    def assertNotEqualTensor(self, a, b):
        try:
          a = a.data()
        except:
          pass
        self.assertFalse(list(a.full())==b)

    def test_getLenght(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.getLenght(), 4)

    def test_getDimension(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.getDimension(), 1)

    def test_getDegree(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.getDegree(), 2)

    def test_evaluation1(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        r = s([0.1]).data()
        self.assertEqual(r.size(1),s.getLenght())

    def test_evaluation2(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        r = s([0.0])
        self.assertEqualTensor(r,[1,0,0,0])
        r = s([0.5])
        self.assertEqualTensor(r,[0,0.5,0.5,0])
        r = s([1.0])
        self.assertEqualTensor(r,[0,0,0,1])

    def test_evaluationMX1(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        x = MX.sym("x")
        r = s([x])
        self.assertTrue(isinstance(r.data(),MX))

    def test_evaluationSX1(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        x = SX.sym("x")
        r = s([x])
        self.assertTrue(isinstance(r.data(),SX))

if __name__ == '__main__':
    unittest.main()
