#!/usr/bin/env python
from helpers import *

class Test_Function_SubBasis(BasisTestCase):


    def test_coefficients_construction_1(self):
        a_ = [2,0,0,1]
        a = DTensor(a_, [4,1,1])
        a = Coefficient(a)
        self.assertEqualTensor(a.data(), a_)

    def test_coefficients_data_k(self):
        a = DTensor(list(range(4*2*3*5)), [4,2,3,5])
        a = Coefficient(a)
        cnt = 0
        for j in range(2):
          for i in range(4):
             self.assertEqualTensor(a.data(cnt), a.data()[i,j,:,:])
             cnt+=1
    def test_coefficients_construction_2(self):
        a_ = SX.sym('a',3 + 1,1)
        a = STensor(a_, [3 + 1,1,1])
        a = Coefficient(a)
        testfun = C.Function('testfun',[a_],[a.data().data()])
        v = [0.1,0.2,0.3,0.4]
        self.assertEqualArray(testfun(v),v)

    def test_coefficients_dimension1(self):
        a = MX.sym('a',3 + 1,1)
        a = MTensor(a, [3 + 1,1,1])
        a = Coefficient(a)
        self.assertEqual(a.dimension(), [4])

    def test_coefficients_dimension2(self):
        a = MX.sym('a', (3+1)*(4+1)*(1+1))
        a = MTensor(a, [3+1,4+1,1+1,1,1])
        a = Coefficient(a)
        self.assertEqual(a.dimension(), [4,5,2])

    def test_coefficients_shape1(self):
        a = MX.sym('a',3 + 1,1)
        a = MTensor(a, [3 + 1,1,1])
        a = Coefficient(a)
        self.assertEqual(a.shape(), [1,1])

    def test_coefficients_shape2(self):
        a = MX.sym('a', (3+1)*(4+1)*(1+1)*3*2)
        a = MTensor(a, [3+1,4+1,1+1,3,2])
        a = Coefficient(a)
        self.assertEqual(a.shape(), [3,2])

    def test_argument_tensor_1(self):
        a = [[1,1],[0,2]]
        a = DM(a)
        a_ = DTensor(a, [2,2,1,1])
        a_ = Coefficient(a_)

    def test_transform(self):
        np.random.seed(0)
        a = np.random.rand(2, 2)
        a_cfs = Coefficient(a)
        Tf = np.random.rand(2,2)
        self.assertEqualT(a_cfs.transform(Tf, 0), Tf.dot(a))
        self.assertEqualT(a_cfs.transform(Tf, 1), a.dot(Tf.T))
        with self.assertRaises(Exception):
            a_cfs.transform(np.random.rand(2), 0)
        with self.assertRaises(Exception):
            Tf = np.random.rand(2, 3)
            a_cfs.transform(Tf, 0)

    def test_transform2(self):
        a = np.random.rand(2,2,3)
        a = Coefficient(a)
        T = np.random.rand(1,2)

        aT1 = a.transform(T)
        aT2 = a.transform(T,0)
        aT3 = a.transform(T,[0])

        self.assertEqualT(aT1,aT2)
        self.assertEqualT(aT1,aT3)

    def test_rm_direction(self):
        a = np.random.rand(1,2,1,4)
        a = Coefficient(a)
        b = a.rm_direction([0, 2])
        self.assertEqual(b.shape(), [2, 4])
        with self.assertRaises(Exception):
            a.rm_direction([0, 1])


if __name__ == '__main__':
    unittest.main()
