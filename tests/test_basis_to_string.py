#!/usr/bin/env python

import os
import random

from helpers import *

class Test_basis_to_string(BasisTestCase):

    # def test_MonomialMasis(self):
    #     b = MonomialBasis(3)
    #     b_type = b.type()
    #     b_string = b.to_string()

    #     self.assertEqual(b_type, 'MonomialBasis')
    #     self.assertEqual(b_string, 'MonomialBasis of degree 3')

    # def test_BSplineBasis(self):
    #     degree = 3
    #     knotsint = 8
    #     knots = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
    #     b = BSplineBasis(knots,degree)
    #     b_type = b.type()
    #     b_string = b.to_string()

    #     self.assertEqual(b_type, 'BSplineBasis')
    #     self.assertEqual(b_string, 'BSplineBasis of degree 3, with 8 internal knots')

    def test_TensorBasis(self):
    #     b1 = MonomialBasis(3)
    #     degree = 4
    #     knotsint = 12
    #     knots = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
    #     b2 = BSplineBasis(knots,degree)
    #     b3 = TensorBasis([b1,b2])

    #     b3_type = b3.type()
    #     print b3_type
    #     b3_string = b3.to_string()
    #     print b3_string
    #     self.assertEqual(b3_type, 'TensorBasis')
    #     self.assertEqual(b3_string, 'TensorBasis containing 2 bases:\n\tMonomialBasis of degree 3\n\tBSplineBasis of degree 4, with 12 internal knots\n')

    #     b4 = TensorBasis([b1])
    #     b4_type = b4.type()
    #     print b4_type
    #     b4_string = b4.to_string()
    #     print b4_string
    #     self.assertEqual(b4_type, 'TensorBasis')
    #     self.assertEqual(b4_string, 'TensorBasis containing 1 basis:\n\tMonomialBasis of degree 3\n')

    #     degree = 4
    #     knotsint = 12
    #     knots = np.r_[np.zeros(degree),np.linspace(0., 1., knotsint), np.ones(degree)]
    #     b2 = BSplineBasis(knots,degree)
    #     b5 = TensorBasis([b1,b2], ['x','y'])
    #     b5_type = b5.type()
    #     print b5_type
    #     b5_string = b5.to_string()
    #     print b5_string
    #     self.assertEqual(b5_type, 'TensorBasis')
    #     self.assertEqual(b5_string, 'TensorBasis containing 2 bases in 2 arguments: \n \tx: MonomialBasis of degree 3\n\ty: BSplineBasis of degree 4, with 12 internal knots\n')

        a = SX.sym('a',1)
        b = SX.sym('b',1)
        interval = Interval(a,b)
        interval_string = interval.to_string()
        print interval.to_string()
        # self.assertEqual(b5_type, 'TensorBasis')
        # self.assertEqual(b5_string, 'TensorBasis containing 2 bases in 2 arguments: \n \tx: MonomialBasis of degree 3\n\ty: BSplineBasis of degree 4, with 12 internal knots\n')
        

if __name__ == '__main__':
    unittest.main()


