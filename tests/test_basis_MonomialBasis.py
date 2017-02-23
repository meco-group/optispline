#!/usr/bin/env python

import os
import random

from helpers import *

from Basis import *
import casadi as C
import numpy as np

class Test_Basis_MonomialBasis(BasisTestCase):

    def test_derivative(self):
        degree = 4
        # test first derivative
        n_der = 1
        b = MonomialBasis(degree)
        db,T = b.derivative(n_der)
        x = casadi.SX.sym('x')
        db_c = b([x])
        for i in range(0,n_der):
            db_c = casadi.jacobian(db_c, x)
        db_c = casadi.Function('db_c', [x], [db_c] )

        np.random.seed(0)
        g = -5 + 10 * np.random.rand(degree+1)
        for i in g:
            # to be able to compare casadi DM with np.array
            self.assertEqualT(np.array(db_c(i)).reshape(degree+1,), np.dot(T.transpose(),db([i])))

        # test degree'th derivative
        n_der = 2
        db,T = b.derivative(n_der)
        db_c = b([x])
        for i in range(0,n_der):
            db_c = casadi.jacobian(db_c, x)
        db_c = casadi.Function('db_c', [x], [db_c] )
        for i in g:
            self.assertEqualT(np.array(db_c(i)).reshape(degree+1,), np.dot(T.transpose(),db([i])))

        # test degreet'th+1 derivative, should give 0 T-matrix
        db,T = b.derivative(degree+1)
        T = np.reshape(T, np.prod(T.shape))
        self.assertTrue(all(T == 0))

    def test_antiderivative(self):
        order = 2
        degree = 3
        b = MonomialBasis(degree)
        banti, Tanti = b.antiderivative(order)
        b_, Tder = banti.derivative(order)
        self.assertEqualT(b_.degree(), b.degree())
        self.assertEqualT(Tder.dot(Tanti), np.eye(Tder.shape[0]))

    def test_degree_elevation(self):
        np.random.seed(0)
        degree = 3
        b1 = MonomialBasis(degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1,c1)
        elev = 2
        b2,T = b1.degree_elevation(elev)
        degree2 = degree+elev
        self.assertEqualT(degree2, b2.degree(), tol=1e-6)
        c2 = T.dot(c1)
        s2 = Function(b2,c2)
        for i in range(b2.dimension()+1):
            self.assertEqualT(s1(i), s2(i), 1e-6)

# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':
    unittest.main()
