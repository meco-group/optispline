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

    def test_basis(self):
        x = MonomialBasis(2)
        y = MonomialBasis(1)

        tb = TensorBasis([x,y], ['x', 'y'])

        r1 = range(3)
        r2 = range(4)

        grid_e = tb.grid_eval([r1  ,r2],['x', 'y'])

        for i in range(len(r1)):
            for j in range(len(r2)):
                self.assertEqualTensor(tb([r1[i], r2[j]]) ,grid_e[i,j])

        tb = TensorBasis([BSplineBasis([0,1],1,4), BSplineBasis([0,1.1],2,2)], ['x', 'y'])

        r1 = np.random.rand(2)
        r2 = np.random.rand(5)

        grid_e = tb.grid_eval([r1  ,r2],['x', 'y'])
        for i in range(len(r1)):
            for j in range(len(r2)):
                self.assertEqualTensor(tb([r1[i], r2[j]],['x','y']) ,grid_e[i,j])

        grid_e = tb.grid_eval([r2  ,r1],['y', 'x'])
        for i in range(len(r1)):
            for j in range(len(r2)):
                self.assertEqualTensor(tb([r2[j], r1[i]],['y','x']),grid_e[j,i])


    def test_function(self):
        x = Polynomial([0, 1], 'x')
        y = Polynomial([0, 1], 'y')

        f  = x + y**2

        r1 = range(3)
        r2 = range(4)

        grid_e = f.grid_eval([r1  ,r2],['x', 'y'])

        for i in range(len(r1)):
            for j in range(len(r2)):
                self.assertAlmostEqual(f(r1[i], r2[j]) ,grid_e[i,j])

        tb = TensorBasis([BSplineBasis([0,1],1,7), BSplineBasis([0,1.1],3,3)], ['x', 'y'])
        c = np.random.rand(*tb.dimension())
        f  = Function(tb, c)

        r1 = np.random.rand(2)
        r2 = np.random.rand(5)

        grid_e = f.grid_eval([r1  ,r2],['x', 'y'])
        for i in range(len(r1)):
            for j in range(len(r2)):
                self.assertAlmostEqual(f([r1[i], r2[j]],['x','y']) ,grid_e[i,j])

        grid_e = f.grid_eval([r2  ,r1],['y', 'x'])
        for i in range(len(r1)):
            for j in range(len(r2)):
                self.assertAlmostEqual(f([r1[i], r2[j]],['x','y']) ,grid_e[j,i])

if __name__ == '__main__':
    unittest.main()

