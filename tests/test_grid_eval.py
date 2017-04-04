#!/usr/bin/env python

import os
import random

from helpers import *

class Test_grid_eval(BasisTestCase):

    def test_monomial(self):
        s1 = MonomialBasis(3)
        b = TensorBasis()
        b = b.add_basis(s1)
        self.assertEqual(len(b.bases()), 1)

if __name__ == '__main__':
    unittest.main()

