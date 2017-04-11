#!/usr/bin/env python

import os
import random

from helpers import *

class Test_grid_eval(BasisTestCase):

    # def test_tensorbasis(self):
    #     x = Polynomial([0, 1], 'x')
    #     y = Polynomial([0, 1], 'y')
    #     z = Polynomial([0, 1], 'z')
    #     xyz = ['x','y','z']
    #     yxz = ['y','x','z']
    #     zxy = ['z','x','y']

    #     f_xyz = x + y + z
    #     f_yxz = y + x + z
    #     f_xzy = z + x + y

# #         print f_xyz
# #         print f_yxz
# #         print f_xzy

    #     tb_xyz = f_xyz.tensor_basis()
    #     tb_yxz = f_yxz.tensor_basis()
    #     tb_xzy = f_xzy.tensor_basis()

# #         print tb_xyz
# #         print tb_yxz
# #         print tb_xzy

    #     e1 = [1 ,0 ,0]
    #     e2 = [0 ,1 ,0]
    #     e3 = [0 ,0 ,1]

# #         print tb_xyz(e1)
# #         print tb_xyz(e2)
# #         print tb_xyz(e3)


# #         print tb_xyz(e1, xyz)

# #         print tb_xyz(e2, yxz)
# #         print tb_xyz(e2, yxz, False)

# #         print tb_xyz(e3, zxy)
# #         print tb_xyz(e3, zxy, False)

    #     self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e2, yxz, False))
    #     self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e3, zxy, False))

    #     tb = TensorBasis([BSplineBasis([0,2],2,4), BSplineBasis([0,1],1,4), BSplineBasis([0,1.1],2,2)], ['x', 'y', 'z'])

    #     self.assertEqualTensor(tb(e1, xyz), tb(e2, yxz, False))
    #     self.assertEqualTensor(tb(e1, xyz), tb(e3, zxy, False))


    def test_function(self):
        x = Polynomial([0, 1], 'x')
        y = Polynomial([0, 1], 'y')
        z = Polynomial([0, 1], 'z')
        xyz = ['x','y','z']
        yxz = ['y','x','z']
        zxy = ['z','x','y']


        m = MonomialBasis(1)
        m2 = MonomialBasis(2)
        m3 = MonomialBasis(3)
        xt = TensorBasis([m,m], [ 'x','y' ])

        grid = xt.grid_eval([[0.1, 0.2], [0.1, 0.2 ,0 ], [0.1, 0.2,0 ,5]] , zxy)
        # print grid
        grid = xt.grid_eval([[0.1, 0.2], [0.1, 0.2 ,0 ], [0.1, 0.2,0 ,5]] , yxz)
        # print grid
        grid = x.grid_eval([[0.1, 0.2], [0.1, 0.2 ,0 ], [0.1, 0.2,0 ,5]] , zxy)
        grid = x.grid_eval([[0.1, 0.2], [0.1, 0.2 ,0 ], [0.1, 0.2,0 ,5]] , xyz)

        x = TensorBasis([m], [ 'x' ])
        y = TensorBasis([m2], [ 'y' ])
        t = TensorBasis([m,m2], [ 'x','y' ])

        eg = EvaluationGrid(t)
        # print eg
        # print eg.eval()

#         print t.arguments()
#         print m.getEvaluationGrid()
#         print m.evaluation_grid()
        grid = t.evaluation_grid()

        xy = ['x','y']
        # print t.grid_eval(grid, xy)
        # print x.grid_eval(grid, xy)
        # print y.grid_eval(grid, xy)

        fx = Function(x, np.random.rand(*x.dimension()))
        fy = Function(y, np.random.rand(*y.dimension()))
        # print fx
        # print fy

        # print fx.grid_eval(grid, xy)
        # print fy.grid_eval(grid, xy)
        # print fx
        # print fy
        x = Polynomial([0, 1], 'x')
        y = Polynomial([0, 1], 'y')
        z = Polynomial([0, 1], 'z')

        operations = [lambda x,y: x+y, lambda x,y: x*y]
        st_function = [x, y,z]
        test_results = []

        import itertools as it

        n_fun = 3

        np.random.seed(0)
        r_data = list(np.random.rand(3))
        print r_data
        for c in it.product(operations, repeat = n_fun - 1):
            for r in it.product(r_data, repeat = n_fun):
                n_ = r[0]
                for i in range(n_fun - 1):
                    n_ = c[i](n_, r[i+1])
                test_results.append(n_)
            cnt = 0
            for f in it.product(st_function, repeat = n_fun):
                f_ = f[0]
                for i in range(n_fun - 1):
                    f_ = c[i](f_, f[i+1])
                self.assertEqualTensor(f_(r_data,['x','y','z']), test_results[cnt])
                cnt+=1

#         er = np.random.rand(3)

#         print f_(*e1)
#         print f_(*e2)
#         print f_(*e3)

#         e = e1
#         print f(e, xyz)

#         print f(e, yxz)
#         print f(e, yxz)

#         print f(e, zxy)
#         print f(e3, zxy)

#         self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e2, yxz, False))
#         self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e3, zxy, False))

#         tb = TensorBasis([BSplineBasis([0,2],2,4), BSplineBasis([0,1],1,4), BSplineBasis([0,1.1],2,2)], ['x', 'y', 'z'])

#         self.assertEqualTensor(tb(e1, xyz), tb(e2, yxz, False))
#         self.assertEqualTensor(tb(e1, xyz), tb(e3, zxy, False))

        # print tb_xyz(e1, zxy)

    # def test_monomial(self):
    #     s1 = MonomialBasis(3)
    #     b = TensorBasis()
    #(     b = b.add_basis(s1)
    #     self.assertEqual(len(b.bases()), 1)
    # def test_basis(self):
    #     x = MonomialBasis(2)
    #     y = MonomialBasis(1)

        # self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e2, yxz, False))
        # self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e3, zxy, False))
    #     tb = TensorBasis([x,y], ['x', 'y'])

    #     r1 = range(3)
    #     r2 = range(4)

    #     grid_e = tb.grid_eval([r1  ,r2],['x', 'y'])

    #     for i in range(len(r1)):
    #         for j in range(len(r2)):
    #             self.assertEqualTensor(tb([r1[i], r2[j]]) ,grid_e[i,j])

    #     tb = TensorBasis([BSplineBasis([0,1],1,4), BSplineBasis([0,1.1],2,2)], ['x', 'y'])

    #     r1 = np.random.rand(2)
    #     r2 = np.random.rand(5)

    #     grid_e = tb.grid_eval([r1  ,r2],['x', 'y'])
    #     for i in range(len(r1)):
    #         for j in range(len(r2)):
    #             self.assertEqualTensor(tb([r1[i], r2[j]],['x','y']) ,grid_e[i,j])

    #     grid_e = tb.grid_eval([r2  ,r1],['y', 'x'])
    #     for i in range(len(r1)):
    #         for j in range(len(r2)):
    #             # print tb([r2[j], r1[i]],['y','x'])
    #             # print tb([r1[i], r2[j]],['x','y'])
    #             self.assertEqualTensor(tb([r2[j], r1[i]],['y','x']) ,grid_e[j,i])


    # def test_function(self):
    #     x = Polynomial([0, 1], 'x')
    #     y = Polynomial([0, 1], 'y')

    #     f  = x + y**2

    #     r1 = range(3)
    #     r2 = range(4)

    #     grid_e = f.grid_eval([r1  ,r2],['x', 'y'])

    #     for i in range(len(r1)):
    #         for j in range(len(r2)):
    #             self.assertAlmostEqual(f(r1[i], r2[j]) ,grid_e[i,j])

    #     tb = TensorBasis([BSplineBasis([0,1],1,7), BSplineBasis([0,1.1],3,3)], ['x', 'y'])
    #     c = np.random.rand(*tb.dimension())
    #     f  = Function(tb, c)

    #     r1 = np.random.rand(2)
    #     r2 = np.random.rand(5)

    #     grid_e = f.grid_eval([r1  ,r2],['x', 'y'])
    #     for i in range(len(r1)):
    #         for j in range(len(r2)):
    #             self.assertAlmostEqual(f([r1[i], r2[j]],['x','y']) ,grid_e[i,j])

    #     grid_e = f.grid_eval([r2  ,r1],['y', 'x'])
    #     for i in range(len(r1)):
    #         for j in range(len(r2)):
    #             self.assertAlmostEqual(f([r1[i], r2[j]],['x','y']) ,grid_e[j,i])

if __name__ == '__main__':
    unittest.main()

