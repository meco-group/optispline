#!/usr/bin/env python

import os
import random

from helpers import *

from Basis import *
from casadi import *

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
        
    def test_Index(self):
        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis([s1,s2])
        with self.assertRaises(Exception):
          p = b.getBasis(-1)
        with self.assertRaises(Exception):
          p = b.getBasis(2)

        p = b.getBasis(0)
        self.assertEqual( p.getDegree(), 2)
        p = b.getBasis(1)
        self.assertEqual( p.getDegree(), 3)
        
        p = b.getBasis("x")

        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis([s1,s2])
        b.setArguments(["x","y"])
        
        p = b.getBasis(0)
        self.assertEqual( p.getDegree(), 2)
        p = b.getBasis(1)
        self.assertEqual( p.getDegree(), 3)
        
        p = b.getBasis("x")
        self.assertEqual( p.getDegree(), 2)
        p = b.getBasis("y")
        self.assertEqual( p.getDegree(), 3)
        
# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':        
    unittest.main()

