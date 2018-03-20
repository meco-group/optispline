#!/usr/bin/env python

from helpers import *

class Test_Optistack(BasisTestCase):

    def test_set_value_expr(self):
        for MyOpti in [Opti, OptiSpline]:
          opti = MyOpti()
          p = opti.parameter(3,1)
          v = opti.parameter(2,1)
          x = opti.variable(2)
          
          opti.set_value(p, 0)
          opti.set_value(v, 0)
          opti.set_value(p[0], 3)
          self.assertEqualTensor(opti.debug.value(p),vertcat(3,0,0))
          opti.set_value(p[[0,2]], 2)
          self.assertEqualTensor(opti.debug.value(p),vertcat(2,0,2)) 
          opti.set_value(p[[0,2]], [1,2])
          self.assertEqualTensor(opti.debug.value(p),vertcat(1,0,2)) 
          opti.set_value(p[[2,0]], [1,2])
          self.assertEqualTensor(opti.debug.value(p),vertcat(2,0,1)) 

          opti.set_value(veccat(v,p), [1,2,3,4,5])
          self.assertEqualTensor(opti.debug.value(p),vertcat(3,4,5))
          self.assertEqualTensor(opti.debug.value(v),vertcat(1,2))    
         
         
          opti.set_value(p, 0)
          opti.set_value(veccat(p[0],p[0]), [4,4])
          self.assertEqualTensor(opti.debug.value(p),vertcat(4,0,0))
          with self.assertInException("ambiguous"):
            opti.set_value(veccat(p[0],p[0]), [4,5])
          with self.assertInException("cannot set a value for a variable"):
            opti.set_value(veccat(p,x,v), [4,5])
          opti.set_value(p, 0)
          opti.set_value(3*p[0], 3)
          self.assertEqualTensor(opti.debug.value(p),vertcat(1,0,0))
          with self.assertInException("cannot set initial/value of an arbitrary expression"):
            opti.set_value(p[0]+p[1], 3)

          opti.set_value(veccat(v,3), [1,2,3])
          opti.set_value(veccat(2,v,5,p), [2,3,4,5,6,7,8])

          with self.assertInException("inconsistent"):
            opti.set_value(veccat(v,3), [1,2,4])
          with self.assertInException("inconsistent"):
            opti.set_value(veccat(2,v,5,p), [5,3,4,2,6,7,8])
          
    def test_shapes(self):
        for MyOpti in [Opti, OptiSpline]:
          opti = MyOpti()
          
          F = opti.variable(10,1)
          x = opti.variable(2,11)
         
          opti.subject_to(x[:,0]==x[:,1]) 
          opti.subject_to(opti.bounded(-1,F,1))

          opti.solver('ipopt')
          opti.minimize(sum1(F))
          sol = opti.solve()
    def test_print(self):
      opti = OptiSpline()
      print(opti)
      self.assertTrue("variables" in str(opti))
      self.assertTrue("variables" in str(opti.debug))
      x = opti.variable()
      y = opti.variable()
      
      p = opti.parameter()
      print(opti)
      
      opti.minimize((x**2-y)**2)
      print(opti)
      opti.solver("ipopt")
      print(opti)
      sol = opti.solve()
      print(sol)
      print(opti)      
            
      opti = Opti()
      print(opti)
      x = opti.variable()
      y = opti.variable()
      
      p = opti.parameter()
      print(opti)
      
      opti.subject_to((x**2-y)**2<=1)
      print(opti)
      opti.solver("ipopt")
      print(opti)
      sol = opti.solve()
      print(sol)
      print(opti)
      
      self.assertTrue("variables" in str(sol))
      
    def test_callback(self):
        for MyOpti in [Opti, OptiSpline]:
          opti = MyOpti()
          
          eps = 1e-5 
          
          x = opti.variable()
          y = opti.variable()
          
          p = opti.parameter()
          
          opti.minimize((x-1)**4+(y-p)**4)
          opti.solver("ipopt")

          opti.callback(lambda i: ret.setdefault('a',opti.debug.value(p)))
          opti.set_value(p, 3)
          ret = {}
          sol = opti.solve()

          self.assertTrue(ret['a']==3)
          opti.set_value(p, 2)
          ret = {}
          sol = opti.solve()
          self.assertTrue(ret['a']==2)
          opti.set_value(p, 3)
          ret = {}
          sol = opti.solve()
          self.assertTrue(ret['a']==3)
          
          opti.callback()
          ret = {}
          sol = opti.solve()
          self.assertFalse('a' in ret)

          opti.callback(lambda i: ret.setdefault('b',opti.debug.value(p)))
          sol = opti.solve()
          self.assertTrue(ret['b']==3)

    def test_debug_value(self):
      for MyOpti in [Opti, OptiSpline]:
        opti = MyOpti()
        p = opti.parameter()
        opti.set_value(p, 3)
        self.assertEqualTensor(opti.debug.value(p**2), 9)
        x = opti.variable()
        
        with self.assertInException("This action is forbidden since you have not solved the Opti stack yet"):
          opti.debug.value(x**2)
        
        with self.assertInException("You cannot set a value for a variable"):
          opti.set_value(x, 2)
        opti.set_initial(x, 2)
        
        with self.assertInException("This action is forbidden since you have not solved the Opti stack yet"):
          opti.debug.value(x**2)
        
        self.assertEqualTensor(opti.debug.value(x**2, opti.initial()), 4)
        
        y = opti.variable()
        
        with self.assertInException("This action is forbidden since you have not solved the Opti stack yet"):
          opti.debug.value(x*y)
      
        
    def test_simple(self):
      for MyOpti in [Opti, OptiSpline]:
        opti = MyOpti()
        
        eps = 1e-5 
        
        x = opti.variable()
        y = opti.variable()
      
        f = (x-1)**2+(y-2)**2  
        opti.minimize(f)

        opti.solver("ipopt")
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1)
        self.assertEqualTensor(sol.value(y), 2)
      

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
          opti.subject_to()
          opti.subject_to(con)
          sol = opti.solve()
  

          self.assertEqualTensor(sol.value(x), xs,tol=1e-7)
          self.assertEqualTensor(sol.value(y), ys,tol=1e-7)
        
          opti.subject_to()
          opti.subject_to(coneps)
          sol2 = opti.solve()

          s = -sign(sol2.value(f)-sol.value(f))
          
          count+=1
          if mul is not None:
            self.assertEqualTensor(sol.value(opti.dual(con)), s*abs(mul),tol=1e-6)
        
        opti.subject_to()
        opti.subject_to(1.5==y)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        
        opti.subject_to()
        opti.subject_to(y==1.5)
        sol = opti.solve()
        
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)

        opti.subject_to()
        opti.subject_to(y==x)
        sol = opti.solve()
        
        x = opti.variable(3,3)
        f = trace(x)
        opti.subject_to()
        opti.subject_to(x>=0)
        with self.assertRaises(Exception):
          sol = opti.solve()
        
        x = opti.variable(2,1)
        opti.subject_to()
        opti.minimize((x[0]-1)**2+(x[1]-2)**2)
        opti.subject_to(x[1]>=x[0])
        sol = opti.solve()
        self.assertEqualTensor(sol.value(x), [1,2],tol=1e-7)
          
        opti.subject_to()
        opti.subject_to(x[1]<=x[0])
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), [1.5,1.5],tol=1e-7)
        
        opti.subject_to()
        opti.subject_to(x<=0.5)
        sol = opti.solve()
        
        print("test", sol.value(x))
        self.assertEqualTensor(sol.value(x), [0.5,0.5],tol=1e-7)

    def test_parametric(self):
      for MyOpti in [Opti, OptiSpline]:
        opti = MyOpti()
        
        x = opti.variable()
        y = opti.variable()
        p = opti.parameter()
        
        opti.minimize((x-1)**2+(y-p)**2)
        opti.solver("ipopt")
        opti.set_value(p, 2)
        sol = opti.solve()

        self.assertEqualTensor(sol.value(x), 1)
        self.assertEqualTensor(sol.value(y), 2)

        opti.set_value(p, 3)
        sol = opti.solve()


        self.assertEqualTensor(sol.value(x), 1)
        self.assertEqualTensor(sol.value(y), 3)
        
        opti.minimize((x-1)**2+(y-2)**2)
        
        opti.subject_to()
        opti.subject_to(y>=p)
        opti.set_value(p, 2.5)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2.5,tol=1e-7)
        
        opti.set_value(p, 1.5)
        sol = opti.solve()
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2,tol=1e-7)
        
        opti.subject_to()
        opti.subject_to(p>=y)
        opti.set_value(p, 2.5)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2,tol=1e-7)
        
        opti.set_value(p, 1.5)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        
        
        opti.subject_to()
        opti.subject_to(y<=p)
        opti.set_value(p, 2.5)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2,tol=1e-7)
        
        opti.set_value(p, 1.5)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        
        opti.subject_to()
        opti.subject_to(p<=y)
        opti.set_value(p, 2.5)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2.5,tol=1e-7)
        
        opti.set_value(p, 1.5)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 2,tol=1e-7)
        
        
        opti.subject_to()
        opti.subject_to(p==y)
        opti.set_value(p, 1.5)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        
        opti.subject_to()
        opti.subject_to(y==p)
        opti.set_value(p, 1.5)
        sol = opti.solve()
        
        self.assertEqualTensor(sol.value(x), 1,tol=1e-7)
        self.assertEqualTensor(sol.value(y), 1.5,tol=1e-7)
        

    def test_parametric_coeffs(self):
    
        c = [1,2,3]
        b = MonomialBasis(2)
        opti = OptiSpline()
        
        P = Function(b, c)
        
        x = opti.variable()
        y = P(x)
        
        opti.minimize(y)
        opti.solver("ipopt")
        sol = opti.solve()
       
        self.assertEqualTensor(sol.value(x), -1.0/3)
        cmx = opti.parameter(3)
        b = MonomialBasis(2)
        
        P = Function(b, cmx)
        
        x = opti.variable()
        y = P(x)
        
        print(jacobian(y,cmx))
        
        opti.minimize(y)
        opti.set_value(cmx, c)
        sol = opti.solve()
        self.assertEqualTensor(sol.value(x), -1.0/3)

        opti.set_value(cmx, [3, 2, 1])
        sol = opti.solve()
        self.assertEqualTensor(sol.value(x), -1)
        
    def test_parametric_knots(self):
    
        r = np.random.random(7)
        degree = 3
        k = [0]*degree+list(np.linspace(0,1,5))+[1]*degree
        
        b = BSplineBasis(k,degree)
        opti = OptiSpline()
        
        P = Function(b, r)
        
        x = opti.variable()
        y = P(x)
        
        Pd = P.derivative()
        
        opti.minimize(y)
        opti.solver("ipopt")
        sol = opti.solve()
        
        xsol = sol.value(x)
       
        print(Pd(sol.value(x)))
        self.assertEqualTensor(Pd(xsol), 0, tol=1e-8)
        kmx = opti.parameter(len(k))
        b = BSplineBasis(kmx,degree)
        
        P = Function(b, r)
        
        x = opti.variable()
        y = P(x)
        
        print(y)
        self.assertTrue(jacobian(y,kmx).is_zero())
        #jtimes(y,kmx,1)
        #jtimes(y,kmx,1,True)
        
        opti.minimize(y)
        opti.set_value(kmx, k)
        sol = opti.solve()
        self.assertEqualTensor(sol.value(x), xsol)

        opti.set_value(kmx, np.array(k)*2)
        sol = opti.solve()
        with self.assertRaises(Exception):
          self.assertEqualTensor(sol.value(x), xsol)


    def test_symm(self):
      for MyOpti in [Opti, OptiSpline]:
        opti = MyOpti()
        
        P = opti.variable(2, 2, 'symmetric')
        
        R = DM([[1,2],[4,4]])
        
        f = sum2(sum1((P-R)**2))

        x = symvar(f)[0]

        opti.minimize(f)
        opti.solver("ipopt")

        sol = opti.solve()
        
        F = casadi.Function('f',[x],[f])

        self.assertEqualTensor(sol.value(P), DM([[1,3],[3,4]]))

        P = opti.variable(2, 2)
        
        
        f = sum2(sum1((P-R)**2))

        x = symvar(f)[0]

        
        opti.minimize(f)
        sol = opti.solve()
        
        F = casadi.Function('f',[x],[f])

        self.assertEqualTensor(sol.value(P), R)

    def test_functionconstr(self):
        b = BSplineBasis([0,1],3,10)
        opti = OptiSpline()
        
        P = opti.Function(b)
        opti.minimize((P**2).integral())
        opti.solver("ipopt")
        
        sol = opti.solve()
        Psol = sol.value(P)
    
        self.assertEqualTensor(Psol(0), 0)    
        self.assertEqualTensor(Psol(0.5), 0)
        self.assertEqualTensor(Psol(1), 0)
        
        opti.subject_to()
        opti.subject_to(P<=2)
        opti.solver("ipopt",{"ipopt":{"tol":1e-10}})
        
        sol = opti.solve()
        Psol = sol.value(P)
        for e in [0,0.5,1]:
          self.assertEqualTensor(Psol(e), 0,tol=1e-5)
        
        opti.subject_to()
        opti.subject_to(P>=2)
        sol = opti.solve()
        Psol = sol.value(P)
        for e in [0,0.5,1]: self.assertEqualTensor(Psol(e), 2,tol=1e-5)
    

        opti.subject_to()
        opti.subject_to(P==2)
        sol = opti.solve()
        Psol = sol.value(P)
        for e in [0,0.5,1]: self.assertEqualTensor(Psol(e), 2,tol=1e-5)

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

        P = opti.variable(2, 2)
        
        print() 
        
        f = sum2(sum1(((P-R)**2).integral))

        x = symvar(f)[0]

        sol = opti.solver(f,[],"ipopt")

        sol.solve()
        
        F = casadi.Function('f',[x],[f])

        self.assertEqualTensor(sol.value(P), R)

if __name__ == '__main__':
    unittest.main()

