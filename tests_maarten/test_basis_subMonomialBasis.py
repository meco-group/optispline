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
        s = SubMonomialBasis(3)
        self.assertEqual(s.getLenght(), 4)

    def test_getDimension(self):
        s = SubMonomialBasis(3)
        self.assertEqual(s.getDimension(), 1)

    def test_getDegree(self):
        s = SubMonomialBasis(3)
        self.assertEqual(s.getDegree(), 3)

    def test_evaluation1(self):
        s = SubMonomialBasis(3)
        r = s([2.0]).data()
        self.assertEqual(r.size(1),s.getLenght())

    def test_evaluation2(self):
        s = SubMonomialBasis(3)
        r = s([2.0])
        self.assertEqualTensor(r,[1,2,4,8])

    def test_evaluation3(self):
        s = SubMonomialBasis(3)
        r = s([2.0])
        self.assertNotEqualTensor(r,[1,2,4,7])

    def test_evaluationMX1(self):
        s = SubMonomialBasis(3)
        x = MX.sym("x")
        r = s([x])
        self.assertTrue(isinstance(r.data(),MX))

    def test_evaluationSX1(self):
        s = SubMonomialBasis(3)
        x = SX.sym("x")
        r = s([x])
        self.assertTrue(isinstance(r.data(),SX))

if __name__ == '__main__':
    unittest.main()
