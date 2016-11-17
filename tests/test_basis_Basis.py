#!/usr/bin/env python
import meco_binaries;meco_binaries(cpp_splines='fill_in_the_branch_you_want')

import unittest
import os
import random

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

from Basis import *
from casadi import *
from helpers import BasisTestCase

class Test_Basis_Basis(BasisTestCase):

    def test_getDimenstion1(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        b = TensorBasis()
        b.addBasis(s1)
        self.assertEqual(b.getNumberOfSubBasis(), 1)

        b.addBasis(s2)
        b.addBasis(s1)
        self.assertEqual(b.getNumberOfSubBasis(), 3)

    def test_getDimenstion2(self):
        s1 = MonomialBasis(3)
        b = TensorBasis()
        b.addBasis(s1)
        self.assertEqual(len(b.getSubBasis()), 1)

    def test_getDimenstion3(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        b = TensorBasis()
        b.addBasis(s1)
        b.addBasis(s2)
        b.addBasis(s1)
        self.assertEqual(len(b.getSubBasis()), 3)

    def test_getEvaluation1(self):
        s1 = MonomialBasis(3)
        b = TensorBasis()
        b.addBasis(s1)
        x = [0.1]
        for _ in range(10):
            x = [random.random()]
            self.assertEqualTensor(b(x),s1(x))

    def test_getEvaluation2(self):
        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis()
        b.addBasis(s1)
        b.addBasis(s2)
        x = [0.1]
        with self.assertRaises(Exception):
            b(x)

    def test_getEvaluation3(self):
        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis()
        b.addBasis(s1)
        b.addBasis(s2)
        x = [0.1,2]
        self.assertEqualTensor(b(x),np.array([[1,2,4,8],[0.1,0.2,0.4,0.8],[0.01,0.02,0.04,0.08]]))
        
    def test_hasArgument1(self):
        b = TensorBasis()
        self.assertFalse(b.hasArguments())
        
    def test_hasArgument2(self):
        a = Argument()
        b = TensorBasis()
        b.setArguments([a,a])
        self.assertTrue(b.hasArguments())
        
# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':        
    unittest.main()

