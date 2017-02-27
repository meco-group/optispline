#!/usr/bin/env python

import os
import random

from helpers import *

class Test_Basis_Basis(BasisTestCase):

    def test_ubasis_n_basis(self):
        s1 = UnivariateBasis()
        s2 = UnivariateBasis()
        s3 = UnivariateBasis()
        b = TensorBasis()
        b = b.add_basis(s1)
        self.assertEqual(b.n_basis(), 1)

        b = b.add_basis(s2)
        b = b.add_basis(s3)
        self.assertEqual(b.n_basis(), 3)

    def test_tbasis_n_basis(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        s3 = MonomialBasis(1)
        b = TensorBasis()
        b = b.add_basis(s1)
        self.assertEqual(b.n_basis(), 1)

        b = b.add_basis(s2)
        b = b.add_basis(s3)
        self.assertEqual(b.n_basis(), 3)

    def test_tbasis_dimension(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        s3 = MonomialBasis(1)
        b = TensorBasis()
        b = b.add_basis(s1)
        self.assertEqual(b.dimension(), [4])

        b = b.add_basis(s2)
        b = b.add_basis(s3)
        self.assertEqual(b.dimension(), [4,5,2])

    def test_tbasis_n_inputs1(self):
        s1 = MonomialBasis(3)
        b = TensorBasis()
        b = b.add_basis(s1)
        self.assertEqual(b.n_inputs(), 1)

    def test_tbasis_n_inputs2(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        s3 = MonomialBasis(1)
        b = TensorBasis()
        b = b.add_basis(s1)
        b = b.add_basis(s2)
        b = b.add_basis(s3)
        self.assertEqual(b.n_inputs(), 3)

    def test_tbasis_n_inputs2(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        s3 = MonomialBasis(1)
        b = TensorBasis()
        b = b.add_basis(s1)
        b = b.add_basis(s2)
        b = b.add_basis(s3)
        self.assertEqual(b.n_inputs(), 3)

    def test_bases1(self):
        s1 = MonomialBasis(3)
        b = TensorBasis()
        b = b.add_basis(s1)
        self.assertEqual(len(b.bases()), 1)

    def test_bases2(self):
        s1 = MonomialBasis(3)
        s2 = MonomialBasis(4)
        b = TensorBasis()
        b = b.add_basis(s1)
        b = b.add_basis(s2)
        b = b.add_basis(s1)
        self.assertEqual(len(b.bases()), 3)

    def test_getEvaluation1(self):
        s1 = MonomialBasis(3)
        b = TensorBasis()
        b = b.add_basis(s1)
        x = [0.1]
        for _ in range(10):
            x = [random.random()]
            self.assertEqualTensor(b(x),s1(x))

    def test_getEvaluation2(self):
        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis()
        b = b.add_basis(s1)
        b = b.add_basis(s2)
        x = [0.1]
        with self.assertRaises(Exception):
            b(x)

    def test_getEvaluation3(self):
        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis()
        b = b.add_basis(s1)
        b = b.add_basis(s2)
        x = [0.1,2]
        self.assertEqualTensor(b(x),np.array([[1,2,4,8],[0.1,0.2,0.4,0.8],[0.01,0.02,0.04,0.08]]))

    def test_hasArgument1(self):
        b = TensorBasis()
        self.assertFalse(b.hasArguments())

    def test_hasArgument2(self):
        a = "a"
        b = TensorBasis()
        s1 = MonomialBasis(2)
        b = b.add_basis(s1)
        b = b.add_basis(s1)
        b = TensorBasis(b, [a,a])
        self.assertTrue(b.hasArguments())

    def test_Index(self):
        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis([s1,s2])
        with self.assertRaises(Exception):
          p = b.basis(-1)
        with self.assertRaises(Exception):
          p = b.basis(2)

        p = b.basis(0)
        self.assertEqual( p.degree(), 2)
        p = b.basis(1)
        self.assertEqual( p.degree(), 3)

        # p = b.basis("x")

        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        b = TensorBasis([s1,s2])
        b = TensorBasis(b, ["x","y"])

        p = b.basis(0)
        self.assertEqual( p.degree(), 2)
        p = b.basis(1)
        self.assertEqual( p.degree(), 3)

        p = b.basis("x")
        self.assertEqual( p.degree(), 2)
        p = b.basis("y")
        self.assertEqual( p.degree(), 3)

    def test_derivative(self):
        # Test derivative of TensorBasis

        d0 = 4
        b0 = MonomialBasis(d0)
        d1 = 3
        k1 = np.r_[np.zeros(d1), np.linspace(0.,1.,4.), np.ones(d1)]
        b1 = BSplineBasis(k1,d1)

        # Derivative of bases
        nd0 = 1
        db0,T0 = b0.derivative(nd0)
        nd1=2
        db1,T1 = b1.derivative(nd1)

        # Derivative of TensorBasis
        B = TensorBasis([b0,b1], ['x', 'y'])
        dB,T = B.derivative([nd0,nd1],[0,1])
        dB,T = B.derivative([nd0,nd1],['x','y'])

        # Same T's and same Bases
        self.assertEqualT(T[0], T0)
        self.assertEqualT(T[1], T1)
        db00 = dB.basis(0)
        db11 = dB.basis(1)

        self.assertEqual(db1.knots(),db11.knots())
        self.assertEqual(db1.degree(),db11.degree())
        self.assertEqual(db0.degree(),db00.degree())

    def test_basis_functions(self):
        knots = [0,0,0.2,0.8,1,1]
        degree = 1
        b = BSplineBasis(knots, degree)
        f = b.basis_functions()

        self.assertEqualT(f.shape,[4,1])
        self.assertEqualT(f(0).T,[[1,0,0,0]])
        self.assertEqualT(f(0.2).T,[[0,1,0,0]])
        self.assertEqualT(f(0.8).T,[[0,0,1,0]])
        self.assertEqualT(f(1).T,[[0,0,0,1]])
        self.assertEqualT(f(0.5).T,[[0,0.5,0.5,0]])

    def test_project_to(self):
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = [0,0,0,0,0.2,0.5,0.8,1,1,1,1]
        b1 = BSplineBasis(knots1, degree)
        c1 = Coefficient(np.random.rand(b1.dimension()))
        s1 = Function(b1,c1)
        knots2 = [0,0,0,0,0,0.2,0.2,0.5,0.5,0.7,0.8,0.8,1,1,1,1,1]
        b2 = BSplineBasis(knots2, degree+1)
        T = b1.project_to(b2)
        c2 = c1.transform(T)
        s2 = Function(b2, c2)
        g2 = b2.greville()
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)

    def test_transform_to(self):
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = [0,0,0,0,0.2,0.5,0.8,1,1,1,1]
        b1 = BSplineBasis(knots1, degree)
        c1 = Coefficient(np.random.rand(b1.dimension()))
        s1 = Function(b1,c1)
        bm = MonomialBasis(degree)
        cm = Coefficient(np.random.rand(bm.dimension()))
        sm = Function(bm,cm)
        knots2 = [0,0,0,0,0,0.2,0.2,0.5,0.5,0.7,0.8,0.8,1,1,1,1,1]
        b2 = BSplineBasis(knots2, degree+1)
        b12, T = b1.transform_to(b2)
        c2 = c1.transform(T)
        s2 = Function(b12, c2)
        g2 = b12.greville()
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)
        b12, T = b1.transform_to(bm)
        c2 = c1.transform(T)
        s2 = Function(b12, c2)
        g2 = b12.greville()
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)

# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':
    unittest.main()

