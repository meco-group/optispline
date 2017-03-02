#!/usr/bin/env python

import os
import random

from helpers import *

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
        self.assertEqualT(knots, b.knots()) # test if sorted
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
        kn = casadi.Function('f', [sym_k1, sym_k2], b.knots())
        self.assertEqualT(np.sort(knots2), kn(0.8, 0.55))


    def test_insert_knots(self):
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = np.r_[np.zeros(degree),np.linspace(0.,1.,knotsint),np.ones(degree)]
        b1 = BSplineBasis(knots1,degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1,c1)
        knots_add = np.random.rand(2)
        b2,T = b1.insert_knots(knots_add)
        knots2 = np.sort(np.r_[knots_add, knots1])
        self.assertEqualT(knots2, b2.knots(), tol=1e-6)
        c2 = T.dot(c1)
        s2 = Function(b2,c2)
        g2 = b2.greville()
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)
        b2,T = b1.insert_knots(0.1)

    def test_midpoint_refinement(self):
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = np.r_[np.zeros(degree),np.linspace(0.,1.,knotsint),np.ones(degree)]
        b1 = BSplineBasis(knots1,degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1,c1)
        ref = 2
        b2,T = b1.midpoint_refinement(ref)
        knots2 = np.r_[np.zeros(degree),np.linspace(0., 1., 2*ref*(knotsint-1)+1), np.ones(degree)]
        self.assertEqualT(knots2, b2.knots(), 1e-6)
        c2 = T.dot(c1)
        s2 = Function(b2,c2)
        g2 = b2.greville()
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)

    def test_degree_elevation(self):
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = np.r_[np.zeros(degree),np.linspace(0.,1.,knotsint),np.ones(degree)]
        b1 = BSplineBasis(knots1,degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1,c1)
        elev = 2
        b2,T = b1.degree_elevation(elev)
        degree2 = degree+elev
        knots2 = np.r_[np.zeros(degree), sorted(np.linspace(0, 1, knotsint).tolist()*(elev+1)), np.ones(degree)]
        self.assertEqualT(knots2, b2.knots(), tol=1e-6)
        self.assertEqualT(degree2, b2.degree(), tol=1e-6)
        c2 = T.dot(c1)
        s2 = Function(b2,c2)
        g2 = b2.greville()
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)

    def test_kick_boundary(self):
        np.random.seed(0)
        degree = 3
        knotsint = 8
        knots1 = np.r_[np.zeros(degree),np.linspace(0.,1.,knotsint),np.ones(degree)]
        b1 = BSplineBasis(knots1,degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1,c1)
        elev = 2
        lb, ub = -0.1, 0.95
        b2,T = b1.kick_boundary([lb, ub])
        knots2 = np.r_[lb*np.ones(degree+1), knots1[degree+1:-degree-1], ub*np.ones(degree+1)]
        self.assertEqualT(knots2, b2.knots(), tol=1e-6)
        self.assertEqualT(degree, b2.degree(), tol=1e-6)
        # c2 = T.dot(c1)
        # s2 = Function(b2,c2)
        # g2 = b2.greville()
        # for i in g2:
        #     self.assertEqualT(s1(i), s2(i), 1e-6)

    def test_derivative(self):
        # Check first derivative
        n_der = 1
        degree = 3
        knotsint = 8
        b = BSplineBasis(np.r_[np.zeros(degree),np.linspace(0.,1.,knotsint),np.ones(degree)],degree)
        db,T = b.derivative(n_der)
        x = casadi.SX.sym('x')
        db_c = b([x])
        for i in range(0,n_der):
            db_c = casadi.jacobian(db_c, x)
        db_c = casadi.Function('db_c', [x], [db_c] )

        g = np.r_[b.greville(), db.greville()]

        for i in g:
            self.assertEqualT(np.array(db_c(i)).reshape(knotsint+degree-1,), np.dot(T.transpose(),db([i])))

        # Check second derivative
        n_der = 2
        db,T = b.derivative(n_der)
        x = casadi.SX.sym('x')
        db_c = b([x])
        for i in range(0,n_der):
            db_c = casadi.jacobian(db_c, x)
        db_c = casadi.Function('db_c', [x], [db_c] )

        for i in g:
            self.assertEqualT(np.array(db_c(i)).reshape(knotsint+degree-1,), np.dot(T.transpose(),db([i])))

        # Check degree'th derivative
        n_der = degree
        db,T = b.derivative(n_der)
        x = casadi.SX.sym('x')
        db_c = b([x])
        for i in range(0,n_der):
            db_c = casadi.jacobian(db_c, x)
        db_c = casadi.Function('db_c', [x], [db_c] )

        for i in g:
            self.assertEqualT(np.array(db_c(i)).reshape(knotsint+degree-1,), np.dot(T.transpose(),db([i])))

        # Check degree'th+1 derivative, should give T-matrix = 0
        return
        db,T = b.derivative(degree+1)
        T = np.reshape(T, np.prod(T.shape))
        self.assertTrue(all(T == 0))

    def test_antiderivative(self):
        order = 2
        degree = 3
        knotsint = 8
        b = BSplineBasis(np.r_[np.zeros(degree), np.linspace(0.,1.,knotsint), np.ones(degree)],degree)
        banti, Tanti = b.antiderivative(order)
        b_, Tder = banti.derivative(order)
        self.assertEqualT(b_.degree(), b.degree())
        self.assertEqualT(b_.knots(), b.knots())
        self.assertEqualT(Tder.dot(Tanti), np.eye(Tder.shape[0]))


# TODO constructor
    # def test_getEvaluation1(self):
    # def test_getEvaluation2(self):


if __name__ == '__main__':
    unittest.main()
