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

    def test_function(self):
        x = Polynomial([0, 1], 'x')
        y = Polynomial([0, 1], 'y')

        r = range(3)

        print r
        print x.grid_eval([r],['x'])

if __name__ == '__main__':
    unittest.main()

