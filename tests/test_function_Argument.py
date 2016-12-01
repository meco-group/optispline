#!/usr/bin/env python

from helpers import *

class Test_Function_Argument(BasisTestCase):

    def test_representation1(self):
        a = Argument()
        self.assertEqual(a.getRepresentation(), "Argument _")

    def test_equal1(self):
        a = Argument()
        self.assertEqual(a, "_")

    def test_equal2(self):
        b = Argument("b")
        self.assertEqual(b, "b")

    def test_equal3(self):
        a = Argument()
        b = Argument("b")
        self.assertNotEqual(a, b)

    def test_equal4(self):
        a = Argument()
        self.assertEqual(a, a)

    def test_setName1(self):
        a = Argument()
        a.setName("a")
        self.assertEqual(a, "a")

    def test_getName1(self):
        a = Argument()
        self.assertEqual(a.getName(), "_")

    def test_getName2(self):
        b = Argument("b")
        self.assertEqual(b.getName(), "b")

if __name__ == '__main__':
    unittest.main()
