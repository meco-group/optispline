import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import meco_binaries;meco_binaries(cpp_splines='fill_in_the_branch_you_want')
from Basis import *
import casadi as ca
import numpy as np
import random
import time

opti = OptiSpline()

# Generate some data
# First we generate the wall, then some obstacle. This represents lidar data.
# The list of (x,y) pairs represents the measurements
r = 2.
Np = 40
theta = np.linspace(-2,2,Np)
xwall = r*np.sin(theta)
ywall = r*np.cos(theta)
zwall = theta*0.

xobst1 = 0.2*np.sin(theta) + 0.5
yobst1 = 0.4*np.cos(theta) + 0.3
zobst1 = theta*0.

x = np.hstack((xwall,xobst1))
y = np.hstack((ywall,yobst1))
z = np.hstack((zwall,zobst1))

fig = plt.figure()
plt.plot(x,y,'ko')
plt.show()

# Define a 2-dimensional bspline with the same knots in both directions
# This BSpline will be representing the potential field we are looking for
degree = 2
knotsint = 10
knots = np.hstack(((degree)*[-r],np.linspace(-r,r,knotsint),(degree)*[r]))
m = BSplineBasis(knots,degree)
b = TensorBasis([m,m])

potential_field = opti.Function(b) # This is the spline in X and Y which we will be using as the potential field
 
# Now let's formulate the optimization problem using CasADi
# The problem we want to solve is:
#
# minimize(integral(potential_field))
#     ci
#
# subject to
#     potential_field(p) >= 0 (1)
#     ci >= -1                (2)
#
# Basicly, we want to minimize the integral so that the potential is as low as possible. However, we need to consider 2 things:
# 1. In order not to end up with an unbounded problem, we want the minimal value of the potential to be -1. This is achieved by relaxing the infinite set of constraints to a relaxed finite set on the coefficients.
# 2. The potential should by larger than zero in order to prevent getting close to measured objects. So we add the constraints. 

# Since the integral of a BSpline has not yet been implemented, we use a standard integration scheme
_start_optimization = time.time() # The construction of the integral will take the main part...
_start_integration = time.time()
obj = 0.
dx = 0.1
dy = 0.1
x_ = -r
while (x_ < r):
    y_ = -r
    while (y_ < r):
        obj = obj + potential_field([x_,y_])*dx*dy/(4.*r*r) # divide by the area to get a non-dimensional measure
        y_ = y_ + dy
    x_ = x_ + dx
_stop_integration = time.time()

# Now, let's do the constraints.
con = []

# constraint (1)
for i in range(0,x.size):
    con.append(potential_field([x[i],y[i]])>=0)

# constraint (2)
con.append(potential_field>=-1)

# Now let's solve the problem
sol = opti.solver(obj,con,"ipopt")
sol.solve()

potential_field = sol.value(potential_field)
_stop_optimization = time.time()

# Now let's evaluate the potential_field to verify the solution.
_start_plotting = time.time()
N = 50
interval = np.linspace(-r,r,N)
X,Y = np.meshgrid(interval,interval)
potential_field_eval = X.copy() 

# evaluate the potential field on the proposed grid
for i in range(0,N):
    for j in range(0,N):
        x_ = X[i,j]
        y_ = Y[i,j]
        potential_field_eval[i,j] = potential_field([x_,y_])

# Plot the solution
fig = plt.figure()
C =plt.contourf(X,Y,potential_field_eval)
plt.plot(x,y,'ko')
plt.colorbar(C)
_stop_plotting = time.time()

# Print some timing diagnostics
print "Diagnostics"
print "optimization time = %f" % (_stop_optimization-_start_optimization)
print "    integration time = %f" % (_stop_integration-_start_integration)
print "    actual casadi optimization time = %f" %  ((_stop_optimization-_start_optimization)-(_stop_integration-_start_integration))
print "plotting time = %f" % (_stop_plotting-_start_plotting)

# Show the plots
plt.show()
