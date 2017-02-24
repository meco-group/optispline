#!/usr/bin/env python

import os
import random

from helpers import *

from Basis import *

class Test_Basis_TensorBasis(BasisTestCase):
    def test_basis_functions(self):
        np.random.seed(0)
        degree = 3
        knots1 = [0,0,0,0,0.2,0.5,0.8,1,1,1,1]
        b1 = BSplineBasis(knots1,degree)
        b2 = MonomialBasis(4)

        n1 = b1.dimension()
        n2 = b2.dimension()

        B = TensorBasis([b1,b1,b2])
        FB = B.basis_functions()
        n423 = 3 + n1*1 + n1*n1*2;
        FB423 = FB[n423,0]

        fb1 = b1.basis_functions()
        fb2 = b2.basis_functions()

        fb14 = fb1[3,0]
        fb12 = fb1[1,0]
        fb23 = fb2[2,0]

        N = 100
        x = numpy.random.random(N)
        y = numpy.random.random(N)
        z = numpy.random.random(N)
        for i in range(0,N):
            self.assertEqual(FB423(x[i],y[i],z[i]),fb14(x[i])*fb12(y[i])*fb23(z[i]))

    def test_project_to(self):
        #np.random.seed(0)
        degree = 3
        knots1 = [0,0,0,0,0.2,0.5,0.8,1,1,1,1]
        b1 = TensorBasis([BSplineBasis(knots1,degree)])
        c1 = Coefficient(5*np.random.random(b1.dimension()[0]))
        s1 = Function(b1,c1)

        #knots2 = [0.1,0.1,0.1,0.1,0.1,0.2,0.5,0.8,1,1,1,1,1]
        #knots2 = [0,0,0,0,0,0.2,0.2,0.5,0.5,0.6,0.8,1,1,1,1,1]
        knots2 = [0,0,0,0,0,0.2,0.2,0.5,0.5,0.7,0.8,0.8,1,1,1,1,1]
        b2 = TensorBasis([BSplineBasis(knots2,4)])

        T = b1.project_to(b2)
        c2 = c1.transform(T)
        s2 = Function(b2,c2)

        g2 = numpy.random.random(10)
        for i in g2:
            #print 2*(s1(i)-s2(i))/(s1(i)+s2(i))
            self.assertEqualT(s1(i), s2(i), 1e-6)

    def test_project_to_multi(self):
        #np.random.seed(0)
        degree = 3
        knots1 = [0,0,0,0,0.2,0.5,0.8,1,1,1,1]
        b11 = BSplineBasis(knots1,degree)
        b12 = MonomialBasis(1)
        b1 = TensorBasis([b11, b12])
        c1 = Coefficient(np.random.rand(b11.dimension(), b12.dimension()))
        s1 = Function(b1,c1)

        knots2 = [0,0,0,0,0,0.2,0.2,0.5,0.5,0.7,0.8,0.8,1,1,1,1,1]
        #knots2 = [0,0,0,0,0,0.2,0.4,0.5,0.6,0.8,1,1,1,1,1]
        b21 = BSplineBasis(knots2,4)
        b22 = BSplineBasis([0,0,0.5,1,1], 1)
        b2 = TensorBasis([b21, b22])
        T = b1.project_to(b2)
        c2 = c1.transform(T)
        s2 = Function(b2,c2)

        N = 10;
        x = numpy.random.random(N)
        y = numpy.random.random(N)
        for i in range(0,N):
            # print s1(x[i],y[i]) - s2(x[i],y[i])
            self.assertEqualT(s1(x[i],y[i]), s2(x[i],y[i]), 1e-6)

if __name__ == '__main__':
    unittest.main()

