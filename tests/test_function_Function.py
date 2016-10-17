#!/usr/bin/env python

import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import unittest
from casadi import *
from Basis import *
from helpers import BasisTestCase

class Test_Function_Function(BasisTestCase):

    def test_construct_function(self):
        a = MX.sym('a',3 + 1,1)
        a = MTensor(a, [3 + 1,1,1])
        a = Coefficient(a)
        m = SubMonomialBasis(3)
        b = Basis([m])
        f = Function(b,a)

    def test_function_evaluation_type1(self):
        a_ = [2,0,0,1]
        a_ = DTensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = SubMonomialBasis(3)
        b = Basis([m])
        f = Function(b,a_) 
        x = [1.0] 
        self.assertEqual(type(f(x).data()), casadi.DM)


    def test_function_evaluation_type2(self):
        a_ = [2,0,0,1]
        a_ = DTensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = SubMonomialBasis(3)
        b = Basis([m])
        f = Function(b,a_) 
        x = [SX.sym('x',1,1)] 
        self.assertEqual(type(f(x).data()), casadi.SX)


    def test_function_evaluation_type3(self):
        a_ = SX.sym('a',3 + 1,1)
        a_ = STensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = SubMonomialBasis(3)
        b = Basis([m])
        f = Function(b,a_) 
        x = [1.0] 
        self.assertEqual(type(f(x).data()), casadi.SX)


    def test_function_evaluation_type4(self):
        a_ = SX.sym('a',3 + 1,1)
        a_ = STensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = SubMonomialBasis(3)
        b = Basis([m])
        f = Function(b,a_) 
        x = [SX.sym('x',1,1)] 
        self.assertEqual(type(f(x).data()), casadi.SX)

    def test_function_evaluation_1(self):
        a_ = [2,0,0,1]
        a_ = DTensor(a_, [3 + 1,1,1])
        a_ = Coefficient(a_)
        m = SubMonomialBasis(3)
        b = Basis([m])
        f = Function(b,a_) 
        self.assertEqual( f([0.0]).data(), 2 )
        self.assertEqual( f([1.0]).data(), 3 )
        self.assertEqual( f([2.0]).data(), 10 )

    def test_function_evaluation_2(self):
        a = DM([[1,1],[0,2]]) # 1 + y + 2xy
        a_ = DTensor(a, [2,2,1,1])
        a_ = Coefficient(a_)
        m = SubMonomialBasis(1)
        b = Basis([m,m])
        f = Function(b,a_) 
        for x in range(-5, 4):
            for y in range(-5, 4):
                self.assertEqual( f([x,y]).data() , 1 + y + 2*x*y)

if __name__ == '__main__':
    unittest.main()
