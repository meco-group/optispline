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


    def test_isupper(self):
        self.assertTrue('FOO'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_split(self):
        s = 'hello world'
        self.assertEqual(s.split(), ['hello', 'world'])
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)

if __name__ == '__main__':
    s1 = SubMonomialBasis(2)
    s2 = SubMonomialBasis(3)
    s3 = SubBSplineBasis([0,1],2,2)

    b1 = Basis([s1,s2])
    b2 = Basis([s2,s3])

    print b1.getDimension()
    print b2.getDimension()

    b = b1 + b2

    print b
    print b.getSubBasis()
    unittest.main()
