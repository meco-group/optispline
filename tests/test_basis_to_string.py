#!/usr/bin/env python

import os
import random

from helpers import *

class Test_basis_to_string(BasisTestCase):

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

        # Test symbolic interval to_string
    #     a = SX.sym('a',1)
    #     b = SX.sym('b',1)
    #     interval = Interval(a,b)
    #     interval_string = interval.to_string()
    #     print interval.to_string()

        # Test TensorDomain to_string
        interval1 = Interval(0,1)
        interval2 = Interval(2,3)
        interval3 = Interval(1.001,100.54)
        tens = TensorDomain([interval1, interval2, interval3], ['x','y','z'])
        tens_string = tens.to_string()
        tens_type = tens.type()
        self.assertEqual(tens_type, 'TensorDomain')
        self.assertEqual(tens_string, 'TensorDomain containing 3 domains in 3 arguments: \n \tx: interval [0.00, 1.00]\n\ty: interval [2.00, 3.00]\n\tz: interval [1.00, 100.54]\n')
        
if __name__ == '__main__':
    unittest.main()


