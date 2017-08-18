#!/usr/bin/env python

from helpers import *

class Test_Optistack(BasisTestCase):

    def test_simple(self):
        opti = OptiSpline()
        
        eps = 1e-5 
        
        x = opti.var()
        y = opti.var()
        
        sol = opti.solver((x-1)**2+(y-2)**2,[],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1)
        self.assertEqualTensor(sol.value(y), 2)
       
        f = (x-1)**2+(y-2)**2
        

        count = 0
        for con, coneps, xs, ys, mul in [
          (y>=2.5, y+eps>=2.5, 1, 2.5, 1),
          (y>=1.5, y+eps>=1.5, 1, 2, 0),
          (2.5>=y, 2.5+eps>=y, 1, 2, 0),
          (1.5>=y, 1.5+eps>=y, 1, 1.5, 1),
          (y<=2.5, y<=2.5+eps, 1, 2, 0),
          (y<=1.5, y<=1.5+eps, 1, 1.5, 1),
          (2.5<=y, 2.5<=y+eps, 1, 2.5, 1),
          (1.5<=y, 1.5<=y+eps, 1, 2, 0),
          (y>=x,   y+eps>=x, 1, 2, 0),
          (y<=x,   y<=x+eps, 1.5, 1.5, 1),
          (y<=0,   y<=eps, 1, 0, 4),
          ((3<= y) <=4, (3<= y+eps) <=4, 1, 3, None),
          (3<= (y <=4), 3<= (y+eps <=4), 1, 3, None),
          (0<= (y <=1), 0<= (y <=1+eps), 1, 1, None),
          ((0<= y) <=1, (0<= y) <=1+eps, 1, 1, None),
          ((4>= y) >=3, (4>= y+eps) >=3, 1, 3, None),
          ((1>= y) >=0, (1+eps>= y) >=0, 1, 1, None),
          (opti.bounded(3,y,4), opti.bounded(3,y+eps,4), 1, 3, None),
          (opti.bounded(0,y,1), opti.bounded(0,y+eps,1), 1, 1, None)
        
          
          
          ]:
          sol = opti.solver(f,[con],"ipopt")
          sol.solve()

          self.assertEqualTensor(sol.value(x), xs,tol=1e-7)
          self.assertEqualTensor(sol.value(y), ys,tol=1e-7)
        
          sol2 = opti.solver(f,[coneps],"ipopt")
          sol2.solve()
          s = -sign(sol2.value(f)-sol.value(f))
          print "count", count, con, sol.dual(con)

          count+=1
          if mul is not None:
            self.assertEqualTensor(sol.dual(con), s*abs(mul),tol=1e-6)
        
        
        sol = opti.solver((x-1)**2+(y-2)**2,[1.5==y],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        
        sol = opti.solver((x-1)**2+(y-2)**2,[y==1.5],"ipopt")
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)

        sol = opti.solver((x-1)**2+(y-2)**2,[y==x],"ipopt")
        sol.solve()
        
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
        print("test", sol.value(x))
        self.assertEqualTensor(sol.value(x), [0.5,0.5],tol=1e-7)

    def test_parametric(self):
        opti = OptiSpline()
        
        x = opti.var()
        y = opti.var()
        p = opti.par()
        
        sol = opti.solver((x-1)**2+(y-p)**2,[],"ipopt")
        sol.value(p, 2)
        sol.solve()

        self.assertEqualTensor(sol.value(x), 1)
        self.assertEqualTensor(sol.value(y), 2)

        sol.value(p, 3)
        sol.solve()


        self.assertEqualTensor(sol.value(x), 1)
        self.assertEqualTensor(sol.value(y), 3)
        
        

        sol = opti.solver((x-1)**2+(y-2)**2,[y>=p],"ipopt")
        sol.value(p, 2.5)
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2.5,tol=1e-7)
        
        sol.value(p, 1.5)
        sol.solve()
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2,tol=1e-7)
        
        sol = opti.solver((x-1)**2+(y-2)**2,[p>=y],"ipopt")
        sol.value(p, 2.5)
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2,tol=1e-7)
        
        sol.value(p, 1.5)
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        
        

        sol = opti.solver((x-1)**2+(y-2)**2,[y<=p],"ipopt")
        sol.value(p, 2.5)
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2,tol=1e-7)
        
        sol.value(p, 1.5)
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        
        sol = opti.solver((x-1)**2+(y-2)**2,[p<=y],"ipopt")
        sol.value(p, 2.5)
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2.5,tol=1e-7)
        
        sol.value(p, 1.5)
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2,tol=1e-7)
        
        
        sol = opti.solver((x-1)**2+(y-2)**2,[p==y],"ipopt")
        sol.value(p, 1.5)
        sol.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        
        sol = opti.solver((x-1)**2+(y-2)**2,[y==p],"ipopt")
        sol.value(p, 1.5)
        sol.solve()
        

    def test_parametric_coeffs(self):
    
        c = [1,2,3]
        b = MonomialBasis(2)
        opti = OptiSpline()
        
        P = Function(b, c)
        
        x = opti.var()
        y = P(x)
        
        sol = opti.solver(y,[],"ipopt")
        sol.solve()
       
        self.assertEqualTensor(sol.value(x), -1.0/3)
        cmx = opti.par(3)
        b = MonomialBasis(2)
        
        P = Function(b, cmx)
        
        x = opti.var()
        y = P(x)
        
        print(jacobian(y,cmx))
        
        sol = opti.solver(y,[],"ipopt")
        sol.value(cmx, c)
        sol.solve()
        self.assertEqualTensor(sol.value(x), -1.0/3)

        sol.value(cmx, [3, 2, 1])
        sol.solve()
        self.assertEqualTensor(sol.value(x), -1)
        
    def test_parametric_knots(self):
    
        r = np.random.random(7)
        degree = 3
        k = [0]*degree+list(np.linspace(0,1,5))+[1]*degree
        
        b = BSplineBasis(k,degree)
        opti = OptiSpline()
        
        P = Function(b, r)
        
        x = opti.var()
        y = P(x)
        
        Pd = P.derivative()
        
        sol = opti.solver(y,[],"ipopt")
        sol.solve()
        
        xsol = sol.value(x)
       
        print(Pd(sol.value(x)))
        self.assertEqualTensor(Pd(xsol), 0, tol=1e-8)
        kmx = opti.par(len(k))
        b = BSplineBasis(kmx,degree)
        
        P = Function(b, r)
        
        x = opti.var()
        y = P(x)
        
        print(y)
        self.assertTrue(jacobian(y,kmx).is_zero())
        jtimes(y,kmx,1)
        jtimes(y,kmx,1,True)
        
        sol = opti.solver(y,[],"ipopt")
        sol.value(kmx, k)
        sol.solve()
        self.assertEqualTensor(sol.value(x), xsol)

        sol.value(kmx, np.array(k)*2)
        sol.solve()
        with self.assertRaises(Exception):
          self.assertEqualTensor(sol.value(x), xsol)


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

    def test_functionconstr(self):
        opti = OptiSpline()
        b = BSplineBasis([0,1],3,10)
        opti = OptiSpline()
        
        P = opti.Function(b)
        sol = opti.solver((P**2).integral(),[],"ipopt")
        sol.solve()
        P = sol.value(P)
    
        self.assertEqualTensor(P(0), 0)    
        self.assertEqualTensor(P(0.5), 0)
        self.assertEqualTensor(P(1), 0)
        
        P = opti.Function(b)
            
        sol = opti.solver((P**2).integral(),[P<=2],"ipopt",{"ipopt":{"tol":1e-10}})
        sol.solve()
        P = sol.value(P)
        for e in [0,0.5,1]:
          self.assertEqualTensor(P(e), 0,tol=1e-5)
        
        P = opti.Function(b)
        
        sol = opti.solver((P**2).integral(),[P>=2],"ipopt")
        sol.solve()
        P = sol.value(P)
        for e in [0,0.5,1]: self.assertEqualTensor(P(e), 2,tol=1e-5)
    
        P = opti.Function(b)
    
        sol = opti.solver((P**2).integral(),[P==2],"ipopt")
        sol.solve()
        P = sol.value(P)
        for e in [0,0.5,1]: self.assertEqualTensor(P(e), 2,tol=1e-5)

    def test_symmcoef(self):
    
        return
        b = MonomialBasis(3)
        opti = OptiSpline()
        
        P = opti.Function(b, [2,2], "symmetric")
        
        R = DM([[1,2],[4,4]])
        
        print(((P-R)**2).integral())
        print(((P-R)**2).integral().shape)
        
        f = sum2(sum1(((P-R)**2).integral()))
        
        print(f)

        x = symvar(f)
        
        F = casadi.Function('f',x,[f])
        print(F(0,0,0,0))

        sol = opti.solver(f,[],"ipopt")

        sol.solve()
        


        self.assertEqualTensor(sol.value(P(0)), DM([[1,3],[3,4]]))

        P = opti.var(2, 2)
        
        print() 
        
        f = sum2(sum1(((P-R)**2).integral))

        x = symvar(f)[0]

        sol = opti.solver(f,[],"ipopt")

        sol.solve()
        
        F = casadi.Function('f',[x],[f])

        self.assertEqualTensor(sol.value(P), R)
   
if __name__ == '__main__':
    unittest.main()

