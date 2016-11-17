#!/usr/bin/env python

import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import meco_binaries;meco_binaries(cpp_splines='fill_in_the_branch_you_want')

import unittest
import numpy as np
import casadi as C

from casadi import *
from Basis import *

class BasisTestCase(unittest.TestCase):


    @staticmethod
    def polyval(p,x):
      return np.polyval(list(reversed(p)),x)

    def assertEqualArray(self, a, b,tol=0):
        a = np.array(a)
        if(len(a.shape) == 2):
            if(a.shape[1] == 1):
                a = a[:,0]
        self.assertTrue(np.linalg.norm(np.array(a)-np.array(b))<=tol)


    def assertEqualTensor(self, a, b,tol=-1e-9):
        try:
          a = C.DM(a)
        except:
          a = a.data()
        try:
          b = C.DM(b)
        except:
          b = b.data()
        self.assertTrue(float(C.norm_inf(a-b))<1e-9)

    def assertNotEqualTensor(self, a, b):
        try:
          a = a.data()
        except:
          pass
        self.assertFalse(list(a.full())==b)

