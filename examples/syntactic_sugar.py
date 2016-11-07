from Basis import *

import casadi as C
import numpy as np

degree = 2
m_path = BSplineBasis([0,1],degree,6)
b_path = TensorBasis([m_path])

opti = OptiSpline()

t = opti.var()
max_path = opti.var()

x_path = opti.Function(b_path)
y_path = opti.Function(b_path)

con = []

x_0 = 0.1
y_0 = 0.9

x_1 = 0.9
y_1 = 0.1

n = 41 

degree = 3
m = MonomialBasis(degree)
b = TensorBasis([m,m])

a = C.DM([range(m.getLength()) for i in range(m.getLength())])
print a
a = DTensor(a, [m.getLength(),m.getLength(),1,1])
print a
a = Coefficient(a)
est_function = Function(b,a)

for t_ in np.linspace(0.,1.,n):
    x_n = x_path([t_])
    y_n = y_path([t_])
    z_n = est_function([x_n,y_n])
    con.append(max_path-z_n>=0)
               
con.append(x_path>=0)
con.append(x_path<=1)
con.append(y_path>=0)
con.append(y_path<=1)

con.append(x_path([0])==x_0)
con.append(y_path([0])==y_0)

con.append(x_path([1])==x_1)
con.append(y_path([1])==y_1)


sol = opti.solver(max_path,con,"ipopt",{"ipopt":{"tol":1e-15}})

sol.solve()

x_path_ = sol.value(x_path)
y_path_ = sol.value(y_path)

assert abs(x_path_([0.3])-0.3845627935194639)<1e-3
assert abs(y_path_([0.7])-0.1623137739180175)<1e-3

print "%.16f" % x_path_([0.3])
print "%.16f" % y_path_([0.7])
