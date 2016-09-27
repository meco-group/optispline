#!/usr/bin/env python

import unittest
import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

from Basis import *
from casadi import *

class Test_Basis_SubBasis(unittest.TestCase):

    def assertEqualTensor(self, a, b):
        self.assertTrue(list(a.data().full())==b)

    def assertNotEqualTensor(self, a, b):
        self.assertFalse(list(a.data().full())==b)

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

    def test_evaluation4(self):
        s = SubMonomialBasis(3)
        x = MX()
        r = s([2.0])
        self.assertNotEqualTensor(r,[1,2,4,7])

if __name__ == '__main__':
    unittest.main()
