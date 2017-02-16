#!/usr/bin/env python

import os
import random

from helpers import *

from Basis import *
import casadi as C

class Test_Basis_MonomialBasis(BasisTestCase):

    def test_derivative(self):
        return
        degree = 4
        n_der = 1
        b = MonomialBasis(degree)
        db,T = b.derivative(n_der)
        x = casadi.SX.sym( 'x')
        db_c = b([x])
        for i in range(0,n_der):
            db_c = casadi.jacobian(db_c, x)
        db_c = casadi.Function('db_c', [x], [db_c] )

        np.random.seed(0)
        g = -5 + 10 * np.random.rand(4)
        for i in g
            self.assertEqualT(db_c(i), db(i))

# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':
    unittest.main()
