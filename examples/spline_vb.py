import sys, os

import meco_binaries;meco_binaries(cpp_splines='fill_in_the_branch_you_want')
from splines import *
import numpy as np
import matplotlib.pyplot as plt
import casadi as cas

opti = OptiSpline()

vmax = 0.5
start_pnt = [0., 0.]
end_pnt = [1., 1.]
via_pnts = [[0.5, 0.5]]

via_pnts = []

degree = 3
knots = np.r_[np.zeros(degree), np.linspace(0, 1, 10+1), np.ones(degree)]
m = BSplineBasis(knots, degree)
basis = TensorBasis([m])

x = opti.Function(basis)
y = opti.Function(basis)

vx = x.derivative()
vy = y.derivative()

T = opti.variable()
t_via = opti.variable(len(via_pnts))

opti.subject_to(x(0) == start_pnt[0]) #.data() ...
opti.subject_to(y(0) == start_pnt[1])
opti.subject_to(x(1) == end_pnt[0])
opti.subject_to(y(1) == end_pnt[1])

opti.subject_to(vx(0)==0)
opti.subject_to(vy(0)==0)
opti.subject_to(vx(1)==0)
opti.subject_to(vy(1)==0)

for k, via_pnt in enumerate(via_pnts):
    opti.subject_to(x([t_via[k]])== via_pnt[0])
    opti.subject_to(y([t_via[k]])== via_pnt[1])
    opti.subject_to(t_via[k]>=0.4)
    opti.subject_to(t_via[k]<=0.6)

opti.subject_to(vx<=T*vmax)
opti.subject_to(vx>=-T*vmax)
opti.subject_to(vy<=T*vmax)
opti.subject_to(vy>=-T*vmax)

opti.minimize(T)
opti.solver("ipopt")


opti.set_initial(x.coeff(),np.linspace(start_pnt[0], end_pnt[0], m.dimension()))
opti.set_initial(y.coeff(),np.linspace(start_pnt[0], end_pnt[0], m.dimension()))

opti.set_initial(T,2)

if len(via_pnts)>0:
  opti.set_initial(t_via, 0.5)

sol = opti.solve()
T = float(sol.value(T))
t_via = sol.value(t_via)

time = np.linspace(0, T, 101)

x = sol.value(x)
"""
x.basis().setKnots(list(T*knots))

y = sol.value(y)

y.basis().setKnots(list(T*knots))

vx = x.derivative()
vy = y.derivative()

x_s = np.array([float(x(t)) for t in time.tolist()])
y_s = np.array([float(y(t)) for t in time.tolist()])

vx_s = np.array([float(vx(t)) for t in time.tolist()])
vy_s = np.array([float(vy(t)) for t in time.tolist()])


plt.figure()
plt.plot(x_s, y_s)

plt.figure()
plt.subplot(2, 1, 1)
plt.plot(time, x_s)
plt.subplot(2, 1, 2)
plt.plot(time, y_s)

plt.figure()
plt.subplot(2, 1, 1)
plt.plot(time, vx_s)
plt.subplot(2, 1, 2)
plt.plot(time, vy_s)

plt.show(block=True)
"""
