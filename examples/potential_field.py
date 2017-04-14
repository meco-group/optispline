import matplotlib
import matplotlib.pyplot as plt
import meco_binaries;meco_binaries(cpp_splines='fill_in_the_branch_you_want')
from splines import *
import casadi as ca
import numpy as np
import random

opti = OptiSpline()

x = [0.15,0.18,0.45,0.50,0.9]
y = np.zeros(5)
plt.plot(x,y,'ko')
plt.show()

degree = 3
knotsint = 20
knots = np.hstack(((degree)*[0],np.linspace(0.,1.,knotsint),(degree)*[1]))
# knots = (degree)*[0] + [0.,.1,.2,.3,.4,.5,.6,.7,.8,.9,1.] + (degree)*[1]
m = BSplineBasis(knots,degree)
b = TensorBasis([m])

s = opti.Function(b)

# Objective
dx = 0.02
xs = np.r_[0:1:dx]
obj = ca.sum1(s.list_eval(xs))*dx

# Constraints
con = []
con.append(s.list_eval(x)>=0)
con.append(s>=-1)

sol = opti.solver(obj,con,"ipopt")
sol.solve()

s = sol.value(s)
interval = np.linspace(0.,1.,101)

field = s.list_eval(interval)

plt.figure()
plt.plot(x,y,'ko')
plt.plot(interval,field,'r')
plt.show()

