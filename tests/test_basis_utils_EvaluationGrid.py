#!/usr/bin/env python
from helpers import *

class Test_Basis_utils(BasisTestCase):

    def test_evaluationGrid(self):
      b1 = MonomialBasis(2)
      b2 = MonomialBasis(3)
      b3 = BSplineBasis([0,1],2,2)
      b1 = TensorBasis(b1)
      eg1 = EvaluationGrid(b1)

if __name__ == '__main__':

    unittest.main()
