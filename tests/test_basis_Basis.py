#!/usr/bin/env python

import unittest
import os
import random

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

    def test_getDimenstion1(self):
        s1 = SubMonomialBasis(3)
        s2 = SubMonomialBasis(4)
        b = Basis()
        b.addBasis(s1)
        self.assertEqual(b.getDimension(), 1)

        b.addBasis(s2)
        b.addBasis(s1)
        self.assertEqual(b.getDimension(), 3)

    def test_getDimenstion2(self):
        s1 = SubMonomialBasis(3)
        b = Basis()
        b.addBasis(s1)
        self.assertEqual(len(b.getSubBasis()), 1)

    def test_getDimenstion3(self):
        s1 = SubMonomialBasis(3)
        s2 = SubMonomialBasis(4)
        b = Basis()
        b.addBasis(s1)
        b.addBasis(s2)
        b.addBasis(s1)
        self.assertEqual(len(b.getSubBasis()), 3)

    def test_getEvaluation1(self):
        s1 = SubMonomialBasis(3)
        b = Basis()
        b.addBasis(s1)
        x = [0.1]
        for _ in range(10):
            x = [random.random()]
            r = b(x)
            self.assertTrue(all((r.data() == s1(x).data()).full()))

    def test_getEvaluation2(self):
        s1 = SubMonomialBasis(2)
        s2 = SubMonomialBasis(3)
        b = Basis()
        b.addBasis(s1)
        b.addBasis(s2)
        x = [0.1]
        with self.assertRaises(Exception):
            b(x)

    def test_getEvaluation3(self):
        s1 = SubMonomialBasis(2)
        s2 = SubMonomialBasis(3)
        b = Basis()
        b.addBasis(s1)
        b.addBasis(s2)
        x = [0.1,2]
        r = b(x).data().full()
        self.assertTrue((np.array([[1,2,4,8],[0.1,0.2,0.4,0.8],[0.01,0.02,0.04,0.08]])==r).any())
        
    def test_hasArgument1(self):
        b = Basis()
        self.assertFalse(b.hasArguments())
        
    def test_hasArgument2(self):
        a = Argument()
        b = Basis()
        b.setArguments([a,a])
        self.assertTrue(b.hasArguments())
        
# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):

if __name__ == '__main__':        
    unittest.main()

