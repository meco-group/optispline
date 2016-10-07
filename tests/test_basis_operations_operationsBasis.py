#!/usr/bin/env python

import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import unittest
from Basis import *
from casadi import *

class Test_Basis_SubBasis(unittest.TestCase):

    def assertEqualTensor(self, a, b):
        self.assertTrue(list(a.data().full())==b)

    def assertNotEqualTensor(self, a, b):
        self.assertFalse(list(a.data().full())==b)

if __name__ == '__main__':
    s1 = SubMonomialBasis(2)
    s2 = SubMonomialBasis(3)
    s3 = SubBSplineBasis([0,1],2,2)

    b1 = Basis([s1,s2])
    b2 = Basis([s2,s3])

    S1 = s1 + s2
    S2 = s2 + s3

    print S1
    print S2

    b = b1 + b2

    print "b : ", str(b)
    print b1.getSubBasis()
    print b([2.0,10.0])
    print b.getSubBasis()
    unittest.main()
