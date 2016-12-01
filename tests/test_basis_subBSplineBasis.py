#!/usr/bin/env python

from helpers import *
import pdb

def wrap(s, casadiType):
    def eval(a):
        x = [casadiType.sym('x') for i in a]
        temp = C.Function('temp',x,[s(x)])
        return DTensor(temp(a),[s.getLength()])
    return eval

class Test_Basis_SubBSpline(BasisTestCase):

    def test_getLength(self):
        s = BSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.getLength(), 4)

    def test_getDimension(self):
        s = BSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.getNumberOfSubBasis(), 1)

    def test_getDegree(self):
        s = BSplineBasis([0,0,0,0.5,1,1,1], 2)
        self.assertEqual(s.getDegree(), 2)

    def test_evaluation1(self):
        s = BSplineBasis([0,0,0,0.5,1,1,1], 2)
        r = s([0.1])
        self.assertEqual(r.shape[0],s.getLength())

    def test_evaluation2(self):
        S = BSplineBasis([0,0,0,0.2,0.5,0.7,1,1,1], 2)

        for s in [S, wrap(S,SX), wrap(S,MX)]:
            for arg, check in [
                ([0.0], [1,0,0,0,0,0]),
                ([0.5], [0,0,0.4,0.6,0,0]),
                ([1.0], [0,0,0,0,0,1])
                ]:
                self.assertEqualTensor(s(arg),check)

        S = BSplineBasis([0,0,0.2,0.5,0.7,1,1], 2)

        for s in [S, wrap(S,SX), wrap(S,MX)]:
            for arg, check in [
                ([0.0], [0,0,0,0]),
                ([0.5], [0,0.4,0.6,0]),
                ([1.0], [0,0,0,0])
                ]:
                self.assertEqualTensor(s(arg),check)
if __name__ == '__main__':
    unittest.main()
