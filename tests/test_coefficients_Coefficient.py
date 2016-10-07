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

    def assertEqualTensor(self, a, b):
        self.assertTrue(list(a.data().full())==b)

    def assertNotEqualTensor(self, a, b):
        self.assertFalse(list(a.data().full())==b)

    def assertEqualArray(self, a, b,tol=0):
        a = np.array(a)
        if(len(a.shape) == 2):
            if(a.shape[1] == 1):
                a = a[:,0]
        print np.array(a),np.array(b),np.linalg.norm(np.array(a)-np.array(b))
        self.assertTrue(np.linalg.norm(np.array(a)-np.array(b))<=tol)

    def test_coefficients_construction_1(self):
        a_ = [2,0,0,1]
        a = DTensor(a_, [4,1,1])
        a = Coefficient(a)
        self.assertTrue( all(a.getData().data().full()[:,0] == [2,0,0,1]))

    def test_coefficients_construction_2(self):
        a_ = SX.sym('a',3 + 1,1)
        a = STensor(a_, [3 + 1,1,1])
        a = Coefficient(a)
        # print list(a.getData().data() == a_)
        # print(any(False == ( a.getData().data() == a_)))
        testfun = Function('testfun',[a_],[a.getData().data()])
        v = [0.1,0.2,0.3,0.4]
        self.assertEqualArray(testfun(v),v)

    def test_argument_tensor_1(self):
        a = [[1,1],[0,2]]
        a = DM(a)
        a_ = DTensor(a, [2,2,1,1])
        a_ = Coefficient(a_)
        # print all(a_.getData().data() == a)
     
if __name__ == '__main__':
    unittest.main()
