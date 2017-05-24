import meco_binaries;meco_binaries(cpp_splines='fill_in_the_branch_you_want')
from splines import *
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

opti = OptiSpline()

# Define movement information
D_min = 0.0       #[m] max value of the distance parameter
D = 13       #[m] max value of the distance parameter

v_max = 1  #[m/s]
v_min = -1e-2    #[m/s]   zero gives probems
a_max = 0.2    #[m/s^2]
a_min = -0.2   #[m/s^2]

# Define spline
deg = 3             #degree
n = 8              #number of knots

# You determine which variable the spline is a function of by choosing the basis. E.g. basis(t)
basis_tau = BSplineBasis([0, 1], deg, n) #tau = [0 1], dimensionless
basis_d = BSplineBasis([D_min, D], deg, n) #d = [0 D], [m]
tensorBasis_d = TensorBasis([basis_d], [ 'd' ])
tensorBasis_tua_d = TensorBasis([basis_tau, basis_d], ['tau', 'd'])

d = Parameter('d')
tau = Parameter('tau')

x = opti.Function(tensorBasis_tua_d) #2D Bspline x(tau,d)
v = x.derivative(1, tau)  # time derivative
# v = x.derivative(1, 'tau')  # alternative
# v = x.derivative([ 1 ], [ tau ])  # alternative
a = v.derivative(1, tau)  # second time derivative

T_to_reach_d = opti.Function(tensorBasis_d) #motion time, function of d
obj = T_to_reach_d.integral()  #minimize motion time for all d in [0,D]

con = []
# start condictions
con.append(x.partial_eval(0,'tau') == 0)
con.append(v.partial_eval(0,'tau') == 0)

# end conditions
con.append(x.partial_eval(1,'tau') - d == 0)
con.append(v.partial_eval(1,'tau') == 0)

# global constraints
con.append(v - T_to_reach_d*v_max <= 0)
con.append(v - T_to_reach_d*v_min >= 0)

con.append(a - T_to_reach_d**2*a_max <= 0)
con.append(a - T_to_reach_d**2*a_min >= 0)
con.append(T_to_reach_d >= 0)

sol = opti.solver(obj, con, "ipopt",{"ipopt":{"tol":1e-5}})

# warm starting solver
sol.value(T_to_reach_d, 2 * d)
sol.value(x, d*tau)

sol.solve()

x_ = sol.value(x)
v_ = sol.value(v)
a_ = sol.value(a)
T_to_reach_d_ = sol.value(T_to_reach_d)

fig = plt.figure()
# Make data.
D_ = np.arange(D_min, D, 0.02)
T_ = np.arange(0, 1.01, 0.02)

ax = fig.add_subplot(1, 3, 1, projection='3d')
for d_ in range(D + 1):
    time = T_to_reach_d_(d_) *T_
    position = x_.partial_eval(d_, 'd').list_eval(T_)
    ax.plot(time , d_*np.ones(len(time)), position, zdir='z')
ax.plot(T_to_reach_d_.list_eval(D_), D_, zdir='z')

ax = fig.add_subplot(1, 3, 2, projection='3d')
for d_ in range(D + 1):
    time = T_to_reach_d_(d_) *T_
    velo = v_.partial_eval(d_, 'd').list_eval(T_)/( T_to_reach_d_(d_))
    # velo = v_.grid_eval([T_, [d_]], ['tau', 'd'])/T_to_reach_d_(d_)
    ax.plot(time , d_*np.ones(len(time)), velo, zdir='z')
ax.plot(T_to_reach_d_.list_eval(D_), D_, zdir='z')

ax = fig.add_subplot(1, 3, 3, projection='3d')
for d_ in range(D + 1):
    time = T_to_reach_d_(d_) *T_
    acu = a_.partial_eval(d_, 'd').list_eval(T_)/( T_to_reach_d_(d_) **2)
    ax.plot(time , acu, zs=d_, zdir='y')
ax.plot(T_to_reach_d_.list_eval(D_), D_, zdir='z')
ax.set_zlim(a_min, a_max)

plt.show()

