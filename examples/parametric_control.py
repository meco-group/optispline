import meco_binaries;meco_binaries(cpp_splines='fill_in_the_branch_you_want')
from splines import *
import numpy as np

opti = OptiSpline()

# Define movement information
D_min = 0.1       #[m] max value of the distance parameter
D = 13       #[m] max value of the distance parameter

v_max = 1  #[m/s]
v_min = -1e-2    #[m/s]
a_max = 0.2    #[m/s^2]
a_min = -0.2   #[m/s^2]

# Define spline
deg = 3             #degree
n = 13              #number of knots

# You determine which variable the spline is a function of by choosing the basis. E.g. basis(t)
B = BSplineBasis([0, 1], deg, n) #tau = [0 1], dimensionless
Bd = BSplineBasis([D_min, D], deg, n) #d = [0 D], [m]
TB = TensorBasis([B, Bd], ['tau', 'd'])
x = opti.Function(TB) #2D Bspline x(tau,d)
v = x.derivative(1, 'tau')  # time derivative
a = v.derivative(1, 'tau')  # second time derivative

Td = TensorBasis([Bd], [ 'd' ])
T_mot = opti.Function(Td) #motion time, function of d
obj = T_mot.integral()  #minimize motion time for all d in [0,D]

con = []
# start condictions
con.append(x.partial_eval(0,'tau') == 0)
con.append(v.partial_eval(0,'tau') == 0)
con.append(a.partial_eval(0,'tau') == 0)

# end conditions
d = Polynomial([0,1], 'd')
# d = Patameter('d')
con.append(x.partial_eval(1,'tau') - d == 0)
con.append(v.partial_eval(1,'tau') == 0)
con.append(a.partial_eval(1,'tau') == 0)

# global constraints
con.append(v - T_mot*v_max <= 0)
con.append(v - T_mot*v_min >= 0)

con.append(a - T_mot**2*a_max <= 0)
con.append(a - T_mot**2*a_min >= 0)
con.append(T_mot>=0)

# sol = opti.solver(obj, con, "ipopt",{"ipopt":{"tol":1e-5,"max_iter":1}})
sol = opti.solver(obj, con, "ipopt",{"ipopt":{"tol":1e-5}})
g = d.transform_to(Bd).coeff_tensor()
sol.value(T_mot.coeff_tensor().data(), g)
sol.solve()

x_ = sol.value(x)
v_ = sol.value(v)
a_ = sol.value(a)
T_mot_ = sol.value(T_mot)

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

fig = plt.figure()
# Make data.
D_ = np.arange(D_min, D, 0.02)
T_ = np.arange(0, 1, 0.02)

ax = fig.add_subplot(1, 3, 1, projection='3d')
for d_ in range(D + 1):
    time = T_mot_(d_) *T_ + 1e-5
    position = x_.partial_eval(d_, 'd').list_eval(T_)
    ax.plot(time , d_*np.ones(len(time)), position, zdir='z')
ax.plot(T_mot_.list_eval(D_), D_, zdir='z')

ax = fig.add_subplot(1, 3, 2, projection='3d')
for d_ in range(D + 1):
    time = T_mot_(d_) *T_ + 1e-5
    velo = v_.partial_eval(d_, 'd').list_eval(T_)/T_mot_(d_)
    ax.plot(time , d_*np.ones(len(time)), velo, zdir='z')
ax.plot(T_mot_.list_eval(D_), D_, zdir='z')

ax = fig.add_subplot(1, 3, 3, projection='3d')
for d_ in range(D + 1):
    time = T_mot_(d_) *T_ + 1e-5
    acu = a_.partial_eval(d_, 'd').list_eval(T_)/( T_mot_(d_) **2)
    ax.plot(time , d_*np.ones(len(time)), acu, zdir='z')
ax.plot(T_mot_.list_eval(D_), D_, zdir='z')
plt.show()

