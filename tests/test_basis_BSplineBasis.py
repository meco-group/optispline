#!/usr/bin/env python

import os
import random

from helpers import *

from Basis import *
import casadi as C

class Test_Basis_BSplineBasis(BasisTestCase):


    def test_greville(self):
        degree = 3
        knotsint = 8
        knots = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
        dim_b = knots.shape[0] - degree - 1
        g = np.zeros(dim_b);
        for i in range(dim_b):
            g[i] = np.sum(knots[i+1 : i+degree+1]) / degree
        b1 = BSplineBasis(knots,degree)
        g1 = b1.greville()
        b2 = BSplineBasis([0,1],degree,knotsint)
        g2 = b2.greville()
        self.assertEqualT(g1, g2, 1e-6)
        self.assertEqualT(g1, g, 1e-6)


    def test_constructor(self):
        degree = 3
        knotsint = 8
        knots = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
        b = BSplineBasis(np.flipud(knots), degree)
        self.assertEqualT(knots, b.getKnots()) # test if sorted
        b = BSplineBasis(MX.sym('knots', 13), degree)
        b = BSplineBasis(np.c_[knots], degree)
        with self.assertRaises(Exception):
            b = BSplineBasis(np.vstack((knots,knots)), degree)
        with self.assertRaises(Exception):
            b = BSplineBasis(np.zeros((13, 2)), degree)
        # with symbolic knots
        sym_k1 = MX.sym('knot1')
        sym_k2 = MX.sym('knot2')
        knots1 = knots.tolist() + [sym_k1, sym_k2]
        knots2 = knots.tolist() + [0.8, 0.55]
        b = BSplineBasis(knots1, degree)
        kn = casadi.Function('f', [sym_k1, sym_k2], b.getKnots())
        self.assertEqualT(np.sort(knots2), kn(0.8, 0.55))


    def test_insert_knots(self):
        return
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = np.r_[np.zeros(degree),np.linspace(0.,1.,knotsint),np.ones(degree)]
        b1 = BSplineBasis(knots1,degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1,c1)
        knots_add = np.random.rand(2)
        b2,T = b1.insert_knots(knots_add)
        knots2 = np.sort(np.r_[knots_add, knots])
        self.assertEqualT(knots2, b2.getKnots(), tol=1e-6)

        c2 = T.dot(c1)
        s2 = Function(b2,c2)
        g2 = b2.greville()
        for i in g2:
            self.assertTrue(all(s1([i]) == s2([i])))


# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':
    unittest.main()
