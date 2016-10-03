#!/usr/bin/env python

import unittest
import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

from Basis import *
from casadi import *

import pdb

def wrap(s, casadiType):
    def eval(a):
        x = [casadiType.sym('x') for i in a]
        print x
        temp = Function('temp',x,[s(x).data()])
        print temp
        print temp(a)
        return DTensor(temp(a),[s.getLenght()])
    return eval

class Test_Basis_SubBasis(unittest.TestCase):

    def assertEqualTensor(self, a, b):
        try:
          a = a.data()
        except:
          pass
        self.assertTrue(list(a.full())==b)

    def assertNotEqualTensor(self, a, b):
        try:
          a = a.data()
        except:
          pass
        self.assertFalse(list(a.full())==b)

    def test_getLenght(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.getLenght(), 4)

    def test_getDimension(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.getDimension(), 1)

    def test_getDegree(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.getDegree(), 2)

    def test_evaluation1(self):
        s = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)
        r = s([0.1]).data()
        self.assertEqual(r.size(1),s.getLenght())

    def test_evaluation2(self):
        S = SubBSplineBasis([0,0,0,0.5,1,1,1], 2)

        for s in [S, wrap(S,SX), wrap(S,MX)]:
            for arg, check in [
                ([0.0], [1,0,0,0]),
                ([0.5], [0,0.5,0.5,0]),
                ([1.0], [0,0,0,1])
                ]:
                self.assertEqualTensor(s(arg),check)


if __name__ == '__main__':
    unittest.main()
