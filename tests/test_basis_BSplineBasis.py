#!/usr/bin/env python

import os
import random

from helpers import *

from Basis import *
from casadi import *

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
        with self.assertRaises(Exception):
            b = BSplineBasis(np.c_[knots], degree)
            b = BSplineBasis(np.vstack(knots,knots), degree)
        # with symbolic knots
        sym_k1 = MX.sym('knot1')
        sym_k2 = MX.sym('knot2')
        knots1 = knots.tolist() + [sym_k1, sym_k2]
        knots2 = knots.tolist() + [0.8, 0.55]
        b = BSplineBasis(knots1, degree)
        kn = casadi.Function('f', [sym_k1, sym_k2], b.getKnots())
        self.assertEqualT(np.sort(knots2), kn(0.8, 0.55))

# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':
    unittest.main()
