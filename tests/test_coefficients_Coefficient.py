#!/usr/bin/env python
from helpers import *

class Test_Function_SubBasis(BasisTestCase):


    def test_coefficients_construction_1(self):
        a_ = [2,0,0,1]
        a = DTensor(a_, [4,1,1])
        a = Coefficient(a)
        self.assertEqualTensor(a.getData(), a_)

    def test_coefficients_construction_2(self):
        a_ = SX.sym('a',3 + 1,1)
        a = STensor(a_, [3 + 1,1,1])
        a = Coefficient(a)
        testfun = C.Function('testfun',[a_],[a.getData().data()])
        v = [0.1,0.2,0.3,0.4]
        self.assertEqualArray(testfun(v),v)

    def test_argument_tensor_1(self):
        a = [[1,1],[0,2]]
        a = DM(a)
        a_ = DTensor(a, [2,2,1,1])
        a_ = Coefficient(a_)

if __name__ == '__main__':
    unittest.main()
