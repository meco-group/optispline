import matplotlib
import matplotlib.pyplot as plt

from Basis import *

import casadi as ca
import numpy as np
import random

# Generating the points

n = 10

x = [1.0*i/(n-1) for i in range(n) ]
random.seed(4)
y = [random.random() for _ in range(n)]

plt.figure()
plt.plot(x,y,'ko')


plt.show()
# Construct the casadi problem

opti = OptiSpline()

est = opti.var()
bound = opti.var()

obj = bound

con = []

for x_,y_ in zip(x,y):
     con.append(est - bound <= y_)
     con.append(est + bound >= y_)

sol = opti.solver(obj ,con,"ipopt",{"ipopt":{"tol":1e-15}})

sol.solve()

est_ = sol.value(est)
bound_ = sol.value(bound)
print "center value is ", str(est_)
print "distance to the bound is ", str(bound_)


upper_bound = 2 * [est_ + bound_]
lower_bound = 2 * [est_ - bound_]
middel = 2 * [est_]
interval = [0, 1]

plt.figure()
plt.plot(x,y,'ko')
plt.plot(interval,upper_bound,'b')
plt.plot(interval,lower_bound,'b')
plt.plot(interval,middel,'r')

plt.show()

# Now we will try something simular but our estimation will be linear depending on $x$. To do this we will construct a Monomial and optimise over its coefficients.

degree = 1
b = MonomialBasis(degree)

b = TensorBasis([b])
est_function = opti.Function(b)

con = []

for x_,y_ in zip(x,y):
     con.append(est_function([x_]) - bound <= y_)
     con.append(est_function([x_]) + bound >= y_)

sol = opti.solver(obj ,con,"ipopt",{"ipopt":{"tol":1e-15}})

sol.solve()

est_ = sol.value(est_function)
bound_ = sol.value(bound)

est_function = sol.value(est_function)

interval = np.linspace(0.,1.,101)
upper_bound = []
lower_bound = []
middel = []

for i in interval:
    upper_bound.append(est_function([i]) + bound_)
    lower_bound.append(est_function([i]) - bound_)
    middel.append(est_function([i]))

plt.figure()
plt.plot(x,y,'ko')
plt.plot(interval,upper_bound,'b')
plt.plot(interval,lower_bound,'b')
plt.plot(interval,middel,'r')

plt.show()
# In[10]:

degree = 3
knots = (degree+1)*[0] + [0.2,0.5,0.8] + (degree+1)*[1]
m = BSplineBasis(knots ,degree)

b = TensorBasis([m])
est_function = opti.Function(b)

con = []

for x_,y_ in zip(x,y):
     con.append(est_function([x_]) - bound <= y_)
     con.append(est_function([x_]) + bound >= y_)

sol = opti.solver(obj ,con,"ipopt",{"ipopt":{"tol":1e-15}})

sol.solve()

est_ = sol.value(est_function)
bound_ = sol.value(bound)

est_function = sol.value(est_function)

interval = np.linspace(0.,1.,101)
upper_bound = []
lower_bound = []
middel = []

for i in interval:
    upper_bound.append(est_function([i]) + bound_)
    lower_bound.append(est_function([i]) - bound_)
    middel.append(est_function([i]))

plt.figure()
plt.plot(x,y,'ko')
plt.plot(interval,upper_bound,'b')
plt.plot(interval,lower_bound,'b')
plt.plot(interval,middel,'r')


plt.show()
