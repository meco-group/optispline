# Set to where you downloaded the contents of 'SET-PMA-MECO-ME0038/Research-0002/software'
path = '/mnt/Shared/SET-PMA-MECO-ME0038/Research-0002/software';

import os
if os.name=='nt':
  osname = 'WINDOWS'
else:
  osname = 'LINUX'

import sys
sys.path.insert(0,os.path.join(path,"optistack","PYTHON"))
sys.path.insert(0,os.path.join(path,"casadi","3.0.0",osname + "-PYTHON"))
sys.path.insert(0,os.path.join(path,"cpp_splines","develop",osname + "-PYTHON"))

import casadi

from Basis import *

import numpy as np

import matplotlib.pyplot as plt

from mpl_toolkits.mplot3d import Axes3D

import random

import optistack as opt
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



degree = 3
#m = BSplineBasis([0,1],degree,3)
m = MonomialBasis(degree)
b = TensorBasis([m,m])

a_ = opt.optivar(m.getLenght(),m.getLenght())
bound = opt.optivar()

a = MTensor(a_, [m.getLenght(),m.getLenght(),1,1])
a = Coefficient(a)
est_function = Function(b,a)
con = []

for x_,y_,z_ in zip(x_list,y_list,z_list):
  z_expr = est_function([x_,y_]).data()
  con.append(z_expr+bound>=z_)
  con.append(z_expr-bound<=z_)

obj = bound

opt.optisolve(obj,con)

a = DTensor(opt.optival(a_), [m.getLenght(),m.getLenght(),1,1])

a = Coefficient(a)
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
surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=plt.cm.coolwarm,
                       linewidth=0, antialiased=False)

ax.scatter(x_list,y_list,z_list,'ko')
plt.show()
