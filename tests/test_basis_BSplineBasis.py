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
        knots = np.hstack(((degree)*[0],np.linspace(0.,1.,knotsint),(degree)*[1]))
        dim_b = knots.length - d - 1
        g = np_zeros(dim_b);
        for i in range(0, dim_b-1):
            g[i] = np.sum(knots[i+1 : i+degree]) / degree
        b1 = BSplineBasis(knots,degree)
        g1 = b1.greville()
        b2 = BSplineBasis([0,1],degree,knotsint-1)
        g2 = b2.greville()
        self.assertEqualTensor(g1,g2)
        self.assertEqualTensor(g1,g)

    def test_constructor(self):
        degree = 3
        knotsint = 8
        knots = np.hstack(((degree)*[0],np.linspace(0.,1.,knotsint),(degree)*[1]))
        b = BSplineBasis(np.fliplr(knots),degree)
        self.assertEqualTensor(knots,b.getKnots)
        with self.assertRaises(Exception):
            b = BSplineBasis(np.vstack(knots,knots),degree)

# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':
    unittest.main()
