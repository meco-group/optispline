import matplotlib
matplotlib.use('TKAgg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

from Basis import *

import casadi as ca
import numpy as np
import random

# Generating the points
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

# Construct the casadi problem

opti = OptiSpline()

degree = 0
#m = SubBSplineBasis([0,1],degree,3)
m = MonomialBasis(degree)
b = TensorBasis([m,m])
est_function = opti.Function(b)
bound = opti.var()

obj = bound

con = []


for x_,y_,z_ in zip(x_list,y_list,z_list):
     con.append(est_function([x_,y_]) - bound <= z_)
     con.append(est_function([x_,y_]) + bound >= z_)

sol = opti.solver(obj ,con,"ipopt",{"ipopt":{"tol":1e-10}})

sol.solve()

bound_ = sol.value(bound)

est_function = sol.value(est_function)

x_0 = 0.1
y_0 = 0.9

x_1 = 0.9
y_1 = 0.1

z_0 = est_function([x_0, y_0])
z_1 = est_function([x_1, y_1])

degree = 2
m_path = BSplineBasis([0,1],degree,6)
b_path = TensorBasis([m_path])

max_path = opti.var()
x_path = opti.Function(b_path)
y_path = opti.Function(b_path)

con = []

x_ = x_0
y_ = y_0
z_ = z_0

obj = 0

n = 41

for t_ in np.linspace(0.,1.,n):
    x_n = x_path([t_])
    y_n = y_path([t_])
    z_n = est_function([x_n,y_n])

    con.append(max_path <= z_n)

obj = max_path


for t_ in np.linspace(0.,1.,n):
    x_n = x_path([t_])
    y_n = y_path([t_])

    obj += (x_n - x_) ** 2 + (y_n - y_) ** 2

    x_ = x_n
    y_ = y_n
    z_ = z_n


con.append(x_path >= 0)
con.append(x_path <= 1)
con.append(y_path >= 0)
con.append(y_path <= 1)

con.append(x_path([0]) == x_0)
con.append(y_path([0]) == y_0)
con.append(x_path([1]) == x_1)
con.append(y_path([1]) == y_1)

sol = opti.solver(obj ,con,"ipopt",{"ipopt":{"tol":1e-15}})

sol.solve()
x_path = sol.value(x_path)
y_path = sol.value(y_path)

nt =101

time = np.linspace(0.,1.,nt)

x_p = [x_path([t_]) for t_ in time]
y_p = [y_path([t_]) for t_ in time]
z_p = [est_function([x_path([t_]),y_path([t_])]) for t_ in time]

fig = plt.figure()
axs = fig.gca(projection='3d')

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
        Z[i,j] = est_function([x_,y_])
fig = plt.figure()
ax = fig.gca(projection='3d')
surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=matplotlib.cm.coolwarm,
                       linewidth=0, antialiased=False)

axs.scatter([x_0, x_1],[y_0, y_1],[z_0, z_1],c='r')
li = axs.plot(x_p,y_p,z_p)
li = axs.plot(x_p,y_p,nt*[0])


plt.figure()
plt.plot(x_p,y_p,'ro')


plt.show()
