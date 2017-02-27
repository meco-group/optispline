#!/usr/bin/env python

import os
import random

from helpers import *

class Test_to_string(BasisTestCase):

    def test_MonomialMasis(self):
        b = MonomialBasis(3)
        b_type = b.type()
        b_string = b.to_string()

        print b_string

        self.assertEqual(b_type, 'MonomialBasis')
        self.assertEqual(b_string, 'MonomialBasis of degree 3, on interval [-inf, inf]')

    def test_BSplineBasis(self):
        degree = 3
        knotsint = 8
        knots = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
        b = BSplineBasis(knots,degree)
        b_type = b.type()
        b_string = b.to_string()

        self.assertEqual(b_type, 'BSplineBasis')
        self.assertEqual(b_string, 'BSplineBasis of degree 3, with 8 internal knots, on interval [0.00, 1.00]')

    def test_TensorBasis(self):
        b1 = MonomialBasis(3)
        degree = 4
        knotsint = 12
        knots = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
        b2 = BSplineBasis(knots,degree)
        b3 = TensorBasis([b1,b2])

        b3_type = b3.type()
        print b3_type
        b3_string = b3.to_string()
        print b3_string
        self.assertEqual(b3_type, 'TensorBasis')
        self.assertEqual(b3_string, 'TensorBasis containing 2 bases:\n\tMonomialBasis of degree 3, on interval [-inf, inf]\n\tBSplineBasis of degree 4, with 12 internal knots, on interval [0.00, 1.00]\n')

        b4 = TensorBasis([b1])
        b4_type = b4.type()
        print b4_type
        b4_string = b4.to_string()
        print b4_string
        self.assertEqual(b4_type, 'TensorBasis')
        self.assertEqual(b4_string, 'TensorBasis containing 1 basis:\n\tMonomialBasis of degree 3, on interval [-inf, inf]\n')

        degree = 4
        knotsint = 12
        knots = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
        b2 = BSplineBasis(knots,degree)
        b5 = TensorBasis([b1,b2], ['x','y'])
        b5_type = b5.type()
        print b5_type
        b5_string = b5.to_string()
        print b5_string
        self.assertEqual(b5_type, 'TensorBasis')
        self.assertEqual(b5_string, 'TensorBasis containing 2 bases in 2 arguments: \n \tx: MonomialBasis of degree 3, on interval [-inf, inf]\n\ty: BSplineBasis of degree 4, with 12 internal knots, on interval [0.00, 1.00]\n')

    def test_TensorDomain(self):
        interval1 = Interval(0,1)
        interval2 = Interval(2,3)
        interval3 = Interval(1.001,100.54)
        tens = TensorDomain([interval1, interval2, interval3], ['x','y','z'])
        tens_string = tens.to_string()
        tens_type = tens.type()
        print tens_type
        print tens_string
        self.assertEqual(tens_type, 'TensorDomain')
        self.assertEqual(tens_string, 'TensorDomain containing 3 domains in 3 arguments: \n \tx: interval [0.00, 1.00]\n\ty: interval [2.00, 3.00]\n\tz: interval [1.00, 100.54]\n')

    def test_Interval(self):
        # Test symbolic interval to_string
        a = SX.sym('a',1)
        b = SX.sym('b',1)
        interval = Interval(a,b)
        interval_string = interval.to_string()
        interval_type = interval.type()
        print interval_type
        print interval_string
        self.assertEqual(interval_type, 'Interval')
        self.assertEqual(interval_string, 'interval [a, b]')

    def test_Coefficient(self):
        np.random.seed(0)
        c1 = Coefficient(np.random.rand(8,1,1))
        c1_type = c1.type()
        c1_string = c1.to_string()
        print c1_type
        print c1_string        
        self.assertEqual(c1_type, 'Coefficient')
        self.assertEqual(c1_string, 'Coefficient, consisting of 1 coefficient with shape [1,1], of dimension [8]')

        t = DM([[1,1],[0,2]]) # 1 + y + 2xy
        t_ = DTensor(t, [2,2,1,1])
        c2 = Coefficient(t_)
        c2_type = c2.type()  
        c2_string = c2.to_string()                
        print c2_type
        print c2_string
        self.assertEqual(c2_type, 'Coefficient')
        self.assertEqual(c2_string, 'Coefficient, consisting of 1 coefficient with shape [1,1], of dimension [2,2]')

        t2 = DM([[1,1,1],[0,2,3], [1,5,6], [0,3,2]])
        t2_ = DTensor(t2, [3,4,1,1])
        c3 = Coefficient(t2_)
        c3_type = c3.type()  
        c3_string = c3.to_string()                
        print c3_type
        print c3_string
        self.assertEqual(c3_type, 'Coefficient')
        self.assertEqual(c3_string, 'Coefficient, consisting of 1 coefficient with shape [1,1], of dimension [3,4]')


    def test_Polynomial(self):

        deg = 4
        p = Polynomial([1,2,3,4])
        p_type = p.type()
        p_string = p.to_string()

        print p_type
        print p_string
        self.assertEqual(p_type, 'Polynomial')
        self.assertEqual(p_string, 'Polynomial, consisting of:\nTensorBasis containing 1 basis:\n\tMonomialBasis of degree 3, on interval [-inf, inf]\nand:\n\t Coefficient, consisting of 1 coefficient with shape [1,1], of dimension [4]')
        
    def test_SplineFunction(self):
        degree = 3
        knotsint = 8
        knots1 = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
        b1 = BSplineBasis(knots1, degree)
        c1 = np.random.rand(b1.dimension())
        s1 = Function(b1, c1)

        s_type = s1.type()
        s_string = s1.to_string()
        print s_type
        print s_string
        self.assertEqual(s_type, 'Function')
        self.assertEqual(s_string, 'Function, consisting of a TensorBasis containing 1 basis:\n\tBSplineBasis of degree 3, with 8 internal knots, on interval [0.00, 1.00]\nand:\n\tCoefficient, consisting of 1 coefficient with shape [1,1], of dimension [10]')

        np.random.seed(0)
        d1 = 3
        nki1 = 8
        k1 = np.r_[np.zeros(d1),np.linspace(0., 1., nki1), np.ones(d1)]
        b1 = BSplineBasis(k1, d1)
        ka1 = np.random.rand(2)
        k1a = np.sort(np.r_[k1, ka1])
        d2 = 2
        nki2 = 5
        k2 = np.r_[np.zeros(d2),np.linspace(0., 1., nki2), np.ones(d2)]
        b2 = BSplineBasis(k2, d2)
        ka2 = np.random.rand(3)
        k2a = np.sort(np.r_[k2, ka2])
        d3 = 6
        b3 = MonomialBasis(d3)
        B = TensorBasis([b1,b2,b3], ['x', 'y', 'z'])
        dims = B.dimension()
        C = np.random.rand(dims[0], dims[1], dims[2])
        f = Function(B,C)

        f_type = f.type()
        f_string = f.to_string()
        print f_type
        print f_string
        self.assertEqual(f_type, 'Function')
        self.assertEqual(f_string, 'Function, consisting of a TensorBasis containing 3 bases in 3 arguments: \n \tx: BSplineBasis of degree 3, with 8 internal knots, on interval [0.00, 1.00]\n\ty: BSplineBasis of degree 2, with 5 internal knots, on interval [0.00, 1.00]\n\tz: MonomialBasis of degree 6, on interval [-inf, inf]\nand:\n\tCoefficient, consisting of 1 coefficient with shape [1,1], of dimension [10,6,7]')

if __name__ == '__main__':
    unittest.main()


