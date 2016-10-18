
import matplotlib
matplotlib.use('TKAgg')

from Basis import *

import casadi as ca
import casadi.tools as ca_tools

import numpy as np

import matplotlib.pyplot as plt

from mpl_toolkits.mplot3d import Axes3D

import random

n = 4

x = [1.0*i/(n-1) for i in range(n) ]
y = [1.0*i/(n-1) for i in range(n) ]

x_list = n*x
y_list = sorted(n*y)

random.seed(2)
z_list = [0.2*random.random() for _ in x_list]

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.scatter(x_list,y_list,z_list,'ko')
plt.show()



degree = 3
m = SubMonomialBasis(degree)
b = Basis([m,m])

opti = Optistack()

bound = opti.var()
fitting = opti.Function(b)

con = []

for x_,y_,z_ in zip(x_list,y_list,z_list):

     con.append(fitting([x_,y_]).data() - bound <= z_)
     con.append(fitting([x_,y_]).data() + bound >= z_)

sol = opti.solver(bound,con,"ipopt",{"ipopt":{"tol":1e-15}})

sol.solve()

e_ = sol.value()

est_function = Function(b,a)



import numpy as np

n = 20

X = np.arange(0, 1.01, 1.0/n)
Y = np.arange(0, 1.01, 1.0/n)
X, Y = np.meshgrid(X, Y)

Z = X.copy()
rX = range(len(list(Z)) )
rY = range(len(list(Z[0])) )

for i in rX:
    for j in rY:
        x_ = X[i,j]
        y_ = Y[i,j]
        Z[i,j] = est_function([x_,y_]).data()
fig = plt.figure()
ax = fig.gca(projection='3d')
surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=matplotlib.cm.coolwarm,
                       linewidth=0, antialiased=False)

ax.scatter(x_list,y_list,z_list,'ko')
plt.show()

