#!/usr/bin/env python

import os
import random

from helpers import *

class Test_grid_eval(BasisTestCase):

    def test_tensorbasis(self):
        x = Polynomial([0, 1], 'x')
        y = Polynomial([0, 1], 'y')
        z = Polynomial([0, 1], 'z')
        xyz = ['x','y','z']
        yxz = ['y','x','z']
        zxy = ['z','x','y']

        f_xyz = x + y + z
        f_yxz = y + x + z
        f_xzy = z + x + y

        tb_xyz = f_xyz.tensor_basis()
        tb_yxz = f_yxz.tensor_basis()
        tb_xzy = f_xzy.tensor_basis()

        e1 = [1 ,0 ,0]
        e2 = [0 ,1 ,0]
        e3 = [0 ,0 ,1]

        self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e2, yxz, False))
        self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e3, zxy, False))

        tb = TensorBasis([BSplineBasis([0,2],2,4), BSplineBasis([0,1],1,4), BSplineBasis([0,1.1],2,2)], ['x', 'y', 'z'])

        self.assertEqualTensor(tb(e1, xyz), tb(e2, yxz, False))
        self.assertEqualTensor(tb(e1, xyz), tb(e3, zxy, False))


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
        operation_names= ['+','*']
        st_function = [x, y,z]

        import itertools as it

        n_fun = 4

        order = ['x','y','z']
        st_function_names = order
        np.random.seed(0)
        r_data = list(np.random.rand(3))

        for c in it.product(operations, repeat = n_fun - 1):
            test_results = []
            for r in it.product(r_data, repeat = n_fun):
                n_ = r[0]
                for i in range(n_fun - 1): n_ = c[i](n_, r[i+1])
                test_results.append(n_)
            cnt = 0
            for fi in it.product(range(len(st_function)), repeat = n_fun):
                f = [st_function[i] for i in fi]
                f_ = f[0]
                for i in range(n_fun - 1): f_ = c[i](f_, f[i+1])
                args = f_.tensor_basis().arguments()
                f_args = [r_data[order.index(k)] for k in args]

                for perm in list(it.permutations(range(len(args))))[:2]:
                  f_args_perm = [f_args[p] for p in perm]
                  args_perm = [args[p] for p in perm]
                  print f_args_perm, args_perm
                  s = f_(f_args_perm,args_perm)
                  self.assertEqualTensor(s, test_results[cnt],tol=1e-6)
                #for zip(args,f_args)

                #import ipdb
                #ipdb.set_trace()

                #print "f_args", f_args, args, r_data
                #print f_(f_args,args), test_results[cnt]
                #print [st_function_names[i] for i in fi], [operation_names[operations.index(e)] for e in c]
                #f = (x+x)*x
                #print "test", f(r_data[0]), (r_data[0]+r_data[0])*r_data[0]
                cnt+=1

        er = np.random.rand(3)

        print f_(*e1)
        print f_(*e2)
        print f_(*e3)

        e = e1
        print f(e, xyz)

        print f(e, yxz)
        print f(e, yxz)

        print f(e, zxy)
        print f(e3, zxy)

        self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e2, yxz, False))
        self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e3, zxy, False))

        tb = TensorBasis([BSplineBasis([0,2],2,4), BSplineBasis([0,1],1,4), BSplineBasis([0,1.1],2,2)], ['x', 'y', 'z'])

        self.assertEqualTensor(tb(e1, xyz), tb(e2, yxz, False))
        self.assertEqualTensor(tb(e1, xyz), tb(e3, zxy, False))

        # print tb_xyz(e1, zxy)

        self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e2, yxz, False))
        self.assertEqualTensor(tb_xyz(e1, xyz), tb_xyz(e3, zxy, False))
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
                # print tb([r2[j], r1[i]],['y','x'])
                # print tb([r1[i], r2[j]],['x','y'])
                self.assertEqualTensor(tb([r2[j], r1[i]],['y','x']) ,grid_e[j,i])


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

