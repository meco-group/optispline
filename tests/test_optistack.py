#!/usr/bin/env python

from helpers import *

class Test_Optistack(BasisTestCase):

    def test_simple(self):
        opti = OptiSpline()
        
        x = opti.var()
        y = opti.var()
        
        sol = opti.solver((x-1)**2+(y-2)**2,[],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1)
        self.assertEqualTensor(sol.value(y), 2)

        sol = opti.solver((x-1)**2+(y-2)**2,[y>=x],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2,tol=1e-7)
        

        sol = opti.solver((x-1)**2+(y-2)**2,[y==x],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1.5,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        
        sol = opti.solver((x-1)**2+(y-2)**2,[y<=x],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1.5,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
       
        sol = opti.solver((x-1)**2+(y-2)**2,[y<=0],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 0,tol=1e-7)

        sol = opti.solver((x-1)**2+(y-2)**2,[(3<= y) <=4],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 3,tol=1e-7)
        
        sol = opti.solver((x-1)**2+(y-2)**2,[3<= (y <=4)],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 3,tol=1e-7)

        sol = opti.solver((x-1)**2+(y-2)**2,[(0<= y) <=1],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1,tol=1e-7)

        sol = opti.solver((x-1)**2+(y-2)**2,[(4>= y) >=3],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 3,tol=1e-7)
        
        sol = opti.solver((x-1)**2+(y-2)**2,[(1>= y) >=0],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1,tol=1e-7)
        
        x = opti.var(3,3)
        f = trace(x)
        with self.assertRaises(Exception):
          sol = opti.solver(f,[x>=0],"ipopt")
          
        x = opti.var(2,1)
        sol = opti.solver((x[0]-1)**2+(x[1]-2)**2,[x[1]>=x[0]],"ipopt")
        sol.solve()
        self.assertEqualTensor(sol.value(x), [1,2],tol=1e-7)
          
        sol = opti.solver((x[0]-1)**2+(x[1]-2)**2,[x[1]<=x[0]],"ipopt")
        sol.solve()
        self.assertEqualTensor(sol.value(x), [1.5,1.5],tol=1e-7)
        
        sol = opti.solver((x[0]-1)**2+(x[1]-2)**2,[x<=0.5],"ipopt")
        sol.solve()
        print "test", sol.value(x)
        self.assertEqualTensor(sol.value(x), [0.5,0.5],tol=1e-7)
        
    def test_symm(self):
        opti = OptiSpline()
        
        P = opti.var(2, 2, 'symmetric')
        
        R = DM([[1,2],[4,4]])
        
        f = sum2(sum1((P-R)**2))

        x = symvar(f)[0]

        sol = opti.solver(f,[],"ipopt")

        sol.solve()
        
        F = casadi.Function('f',[x],[f])

        self.assertEqualTensor(sol.value(P), DM([[1,3],[3,4]]))

        P = opti.var(2, 2)
        
        
        f = sum2(sum1((P-R)**2))

        x = symvar(f)[0]

        sol = opti.solver(f,[],"ipopt")

        sol.solve()
        
        F = casadi.Function('f',[x],[f])

        self.assertEqualTensor(sol.value(P), R)

    def test_symmcoef(self):
    
        return
        b = MonomialBasis(3)
        opti = OptiSpline()
        
        P = opti.Function(b, [2,2], "symmetric")
        
        R = DM([[1,2],[4,4]])
        
        print ((P-R)**2).integral()
        print ((P-R)**2).integral().shape
        
        f = sum2(sum1(((P-R)**2).integral()))
        
        print f

        x = symvar(f)
        
        F = casadi.Function('f',x,[f])
        print F(0,0,0,0)

        sol = opti.solver(f,[],"ipopt")

        sol.solve()
        


        self.assertEqualTensor(sol.value(P(0)), DM([[1,3],[3,4]]))

        P = opti.var(2, 2)
        
        print 
        
        f = sum2(sum1(((P-R)**2).integral))

        x = symvar(f)[0]

        sol = opti.solver(f,[],"ipopt")

        sol.solve()
        
        F = casadi.Function('f',[x],[f])

        self.assertEqualTensor(sol.value(P), R)

if __name__ == '__main__':
    unittest.main()
