#!/usr/bin/env python

import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import unittest
from Basis import *
from casadi import *
from helpers import BasisTestCase

class Test_Basis_utils(BasisTestCase):

    def test_evaluationGrid(self):
      b1 = MonomialBasis(2)
      b2 = MonomialBasis(3)
      b3 = BSplineBasis([0,1],2,2)
      b1 = TensorBasis(b1)
      eg1 = EvaluationGrid(b1)
      print eg1

if __name__ == '__main__':

    unittest.main()
