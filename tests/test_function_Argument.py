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

    def test_sum_arguments(self):
        x = Argument("x")
        y = Argument("y")

        b1 = TensorBasis(BSplineBasis([0,0,1,1],1), x)

        b2 = b1
        b2 = TensorBasis(b1, [y])

        self.assertEqual(b1.arguments()[0].getName(), "x")
        self.assertEqual(b2.arguments()[0].getName(), "y")

        # with self.assertRaises(Exception):
        #     b1.setArguments([x,y])

        # c = Coefficient(np.random.random((2,3,5)));
        # s = Function(b,c);



if __name__ == '__main__':
    unittest.main()
