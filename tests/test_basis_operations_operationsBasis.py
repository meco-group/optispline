#!/usr/bin/env python

from helpers import *

class Test_Basis_Operations(BasisTestCase):

    def test_code_example(self):
        s1 = MonomialBasis(2)
        s2 = MonomialBasis(3)
        s3 = BSplineBasis([0,1],2,2)

        b1 = TensorBasis([s1,s2])
        b2 = TensorBasis([s2,s3])

        S1 = s1 + s2
        S2 = s2 + s3

        b = b1 + b2
        

if __name__ == '__main__':
    s1 = MonomialBasis(2)
    s2 = MonomialBasis(3)
    s3 = MonomialBasis(4)
    s4 = BSplineBasis([0,1],2,2)

    b1 = TensorBasis([s1,s2])
    b2 = TensorBasis([s3,s4])

    print "NO ARGUMENT"
    b = b1 + b2
    print b
    print b.getSubBasis()


    x1 = Argument('x1')
    x2 = Argument('x2')
    x3 = Argument('x3')

    b1.setArguments([x1,x2])
    b2.setArguments([x2,x1])

    print "SWAP"
    b = b1 + b2
    print b
    print b.getSubBasis()

    b3 = TensorBasis([s3,s4])
    b3.setArguments([x2,x3])

    print "ARGUMENT COMBINE"
    b = b1 + b3
    print b
    print b.getSubBasis()
    unittest.main()
