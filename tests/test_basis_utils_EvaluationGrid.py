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

def getGrid(basis):
    subBasis = list(basis.getSubBasis())
    print subBasis
    # print subBasis[0].getEvaluationGrid()
    # return [sB.getEvaluationGrid() for sB in subBasis]
    #
if __name__ == '__main__':
    b1 = MonomialBasis(2)
    b2 = MonomialBasis(3)
    b3 = BSplineBasis([0,1],2,2)

    eg1 = EvaluationGrid(b1)
    print eg1
    print(getGrid(b1))
    unittest.main()
