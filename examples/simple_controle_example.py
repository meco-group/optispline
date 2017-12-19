import meco_binaries
meco_binaries(cpp_splines="")
from splines import *
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

opti = OptiSpline()

class Circle:
    def __init__(self, center, r):
        self.x = center[0]
        self.y = center[1]
        self.r = r

    def distance_to(self, obj):
        return (obj[0] - self.x)**2 + (obj[1] - self.y)**2 - self.r **2

    def plot(self, plt):
        a_ = np.linspace(0,np.pi*2,200)
        plt.plot(self.r * np.cos(a_) + self.x,self.r * np.sin(a_) + self.y)

obsticals = [Circle((0.3, 0.1), 0.2), Circle((0.5, 0.8), 0.3)]

v_max = 1  #[m/s]

# Define spline
deg = 3             #degree
n = 15              #number of knots

basis = BSplineBasis([0, 1], deg, n)

x = opti.Function(basis, [ 2, 1 ])
v = x.derivative(1)  # time derivative

total_time = opti.variable() #motion time, function of d

# start condictions
opti.subject_to(x(0) == [0,0])

# end conditions
opti.subject_to(x(1)  == [1,1.4])

# global constraints
for o in obsticals:
    opti.subject_to(o.distance_to(x) >= 0)

opti.subject_to(v[0]**2 + v[1]**2 <= total_time*v_max)

opti.subject_to(total_time >= 0)

# warm starting solver
opti.set_initial(x, [1, 1.4] * Parameter())

opti.minimize(total_time)
opti.solver("ipopt",{"ipopt":{"tol":1e-5}})

sol = opti.solve()
x_sol = sol.value(x)
v_sol = sol.value(v)
total_time_sol = sol.value(total_time)

print("total_time_sol : " + str( total_time_sol))

# Make data.
T_ = np.linspace(0,1,100)
time = total_time_sol *T_
x_pos  = x_sol.list_eval(T_)
fig = plt.figure()
ax = fig.add_subplot(1, 2, 1)
ax.plot(x_pos[ :,0 ], x_pos[ :,1 ])

a_ = np.linspace(0,np.pi*2,200)

for o in obsticals:
    o.plot(ax)
ax.axis([-0.1, 1.1, -0.1, 1.5])
ax = fig.add_subplot(1, 2, 2)
v_abs = (v_sol[0]**2 + v_sol[1]**2) * (1 / total_time_sol)
ax.plot(total_time_sol * T_, v_abs.list_eval(T_))

ax.axis([0, total_time_sol, 0, 1.2])
plt.show()
