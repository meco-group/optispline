#!/usr/bin/env python

from helpers import *

class Test_Optistack(BasisTestCase):

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
      
if __name__ == '__main__':
    unittest.main()
