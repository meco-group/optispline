import unittest

#!/usr/bin/env python
import sys
# sys.path.insert( 0, '/home/erik/Documents/cpp_splines/swig')
sys.path.insert( 0, '../swig')
sys.path.insert(0, '/home/erik/Documents/casadi3.0')
import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

from Basis import *
from casadi import *

class Test_Basis_SubBasis(unittest.TestCase):
    def assertEqualTensor(a, b):
        self.assertTrue(list(a.data().full())==b)

    def assertNotEqualTensor(self, a, b):
        self.assertFalse(list(a.data().full())==b)

    def test_getLenght(self):
        s = SubMonomialBasis(3)
        self.assertEqual(s.getLenght(), 4)

    def test_getDimension(self):
        s = SubMonomialBasis(3)
        self.assertEqual(s.getDimension(), 1)

    def test_getDegree(self):
        s = SubMonomialBasis(3)
        self.assertEqual(s.getDegree(), 3)

    def test_evaluation1(self):
        s = SubMonomialBasis(3)
        r = s([2.0]).data()
        self.assertEqual(r.size(1),s.getLenght())

    def test_evaluation2(self):
        s = SubMonomialBasis(3)
        r = s([2.0]).data()
        assertEqualTensor(r,[1,2,4,8])

    def test_evaluation3(self):
        s = SubMonomialBasis(3)
        r = s([2.0]).data()
        assertNotEqualTensor(r,[1,2,4,7])

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
    s = SubMonomialBasis(3)
    print s
    unittest.main()
