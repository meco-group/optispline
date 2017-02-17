#!/usr/bin/env python

import os
import random

from helpers import *

from Basis import *
from casadi import *

class Test_Basis_Basis(BasisTestCase):

    def test_ubasis_n_basis(self):
        s1 = UnivariateBasis()
        s2 = UnivariateBasis()
        s3 = UnivariateBasis()
        b = TensorBasis()
        b = b.addBasis(s1)
        self.assertEqual(b.n_basis(), 1)

        b = b.addBasis(s2)
        b = b.addBasis(s3)
        self.assertEqual(b.n_basis(), 3)

    def test_tbasis_n_basis(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        s3 = MonomialBasis(1)
        b = TensorBasis()
        b = b.addBasis(s1)
        self.assertEqual(b.n_basis(), 1)

        b = b.addBasis(s2)
        b = b.addBasis(s3)
        self.assertEqual(b.n_basis(), 3)

    def test_tbasis_dimension(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        s3 = MonomialBasis(1)
        b = TensorBasis()
        b = b.addBasis(s1)
        self.assertEqual(b.dimension(), [4])

        b = b.addBasis(s2)
        b = b.addBasis(s3)
        self.assertEqual(b.dimension(), [4,5,2])

    def test_tbasis_n_inputs1(self):
        s1 = MonomialBasis(3)
        b = TensorBasis()
        b = b.addBasis(s1)
        self.assertEqual(b.n_inputs(), 1)

    def test_tbasis_n_inputs2(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        s3 = MonomialBasis(1)
        b = TensorBasis()
        b = b.addBasis(s1)
        b = b.addBasis(s2)
        b = b.addBasis(s3)
        self.assertEqual(b.n_inputs(), 3)

    def test_tbasis_n_inputs2(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        s3 = MonomialBasis(1)
        b = TensorBasis()
        b = b.addBasis(s1)
        b = b.addBasis(s2)
        b = b.addBasis(s3)
        self.assertEqual(b.n_inputs(), 3)

    def test_getSubBasis1(self):
        s1 = MonomialBasis(3)
        b = TensorBasis()
        b = b.addBasis(s1)
        self.assertEqual(len(b.getSubBasis()), 1)

    def test_getSubBasis2(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        b = TensorBasis()
        b = b.addBasis(s1)
        b = b.addBasis(s2)
        b = b.addBasis(s1)
        self.assertEqual(len(b.getSubBasis()), 3)

    def test_getEvaluation1(self):
        s1 = MonomialBasis(3)
        b = TensorBasis()
        b = b.addBasis(s1)
        x = [0.1]
        for _ in range(10):
            x = [random.random()]
            self.assertEqualTensor(b(x),s1(x))

    def test_getEvaluation2(self):
        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis()
        b = b.addBasis(s1)
        b = b.addBasis(s2)
        x = [0.1]
        with self.assertRaises(Exception):
            b(x)

    def test_getEvaluation3(self):
        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis()
        b = b.addBasis(s1)
        b = b.addBasis(s2)
        x = [0.1,2]
        self.assertEqualTensor(b(x),np.array([[1,2,4,8],[0.1,0.2,0.4,0.8],[0.01,0.02,0.04,0.08]]))

    def test_hasArgument1(self):
        b = TensorBasis()
        self.assertFalse(b.hasArguments())

    def test_hasArgument2(self):
        a = Argument()
        b = TensorBasis()
        s1 = MonomialBasis(2)
        b = b.addBasis(s1)
        b = b.addBasis(s1)
        b = TensorBasis(b, [a,a])
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
        b = TensorBasis(b, ["x","y"])

        p = b.getBasis(0)
        self.assertEqual( p.getDegree(), 2)
        p = b.getBasis(1)
        self.assertEqual( p.getDegree(), 3)

        p = b.getBasis("x")
        self.assertEqual( p.getDegree(), 2)
        p = b.getBasis("y")
        self.assertEqual( p.getDegree(), 3)

    # def test_derivative(self):
    #     # Test derivative of TensorBasis

    #     d0 = 4
    #     b0 = MonomialBasis(d0)
    #     d1 = 3
    #     k1 = np.r_[np.zeros(d1), np.linspace(0.,1.,4.), np.ones(d1)]
    #     b1 = BSplineBasis(k1,d1)
        
    #     # Derivative of bases
    #     nd0 = 1
    #     #db0,T0 = b0.derivative(nd0)
    #     nd1=2
    #     #db1,T1 = b1.derivative(nd1)
        
    #     # Derivative of TensorBasis
    #     B = TensorBasis([b0,b1], ['x', 'y'])
    #     dB, T = B.derivative([nd0], ['x'])
    #     dB, T = B.derivative([nd0], [0])
    #     # dB,T = B.derivative([nd0,nd1],[0,1])

    #     # Same T's and same Bases
    #     self.assertEqualT(T[0], T0)
    #     self.assertEqualT(T[1], T1)
    #     db00 = B.getBasis(0)
    #     db11 = B.getBasis(1)
    #     self.assertEqual(db1,db11)
    #     self.assertEqual(db0,db00)


# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':
    unittest.main()

