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
    p1 = Polynomial([0,0,1],'x')
    print p1
    # print p1([5])
    p2 = Polynomial([0,1],'y')
    print p2
    # print p2([5])

    p = p1 + p2
    p = p1 * p2
    print p
    print p([0,0])
    print p([0,1])
    print p([0,2])
    print p([1,0])
    print p([1,1])
    print p([1,2])
    print p([2,0])
    print p([2,1])
    print p([2,2])


    x = Polynomial([0,1],'x')
    y = Polynomial([0,1],'y')

    f = x + y*x + y*y

    for i  in range(3):
        for j  in range(3):
            print i + i*j +j**2,
            print " <--> "
            print f([i,j])

    h = f + p
    for i  in range(3):
        for j  in range(3):
            print str(i) + " , " + str(j) + " : ",
            print i + j**2  + i*j + i**2*j,
            print " <--> "
            print h([i,j])
    h = -f - p
    for i  in range(3):
        for j  in range(3):
            print str(i) + " , " + str(j) + " : ",
            print -(i + j**2  + i*j + i**2*j),
            print " <--> "
            print h([i,j])

    unittest.main()
