import sys, os

import meco_binaries;meco_binaries(cpp_splines='fill_in_the_branch_you_want')
from Basis import *
import numpy as np
import matplotlib.pyplot as plt
import casadi as cas

opti = OptiSpline()

def derivative(coeffs, knots1, degree, o=1):
    coeffs = coeffs.getData()
    try:
      n = coeffs.shape[0]
    except:
      coeffs = coeffs.data()
      n = coeffs.shape[0]
    P = np.eye(n)
    knots = knots1.copy()
    knots_out = knots[o:-o]
    for i in range(o):
        knots = knots[1:-1]
        delta_knots = knots[degree - i:] - knots[:- degree + i]
        T = np.zeros((n - 1 - i, n - i))
        j = np.arange(n - 1 - i)
        T[(j, j)] = -1. / delta_knots
        T[(j, j + 1)] = 1. / delta_knots
        P = (degree - i) * np.dot(T, P)
    m = BSplineBasis(knots_out, degree)
    basis = TensorBasis([m])
    if isinstance(coeffs, (cas.SX, cas.MX)):
        coeffs_out = cas.mtimes(cas.DM(P), coeffs)
    else:
        coeffs_out = P.dot(coeffs.squeeze())
    return coeffs_out, knots_out

def FunDerivative(self):
  vx_cfs_, knots2 = derivative(self.getCoefficient(), knots, degree)
  m2 = BSplineBasis(knots2, degree-1)
  basis2 = TensorBasis([m2])
  return Function(basis2, vx_cfs_)

Function.derivative = FunDerivative


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

T = opti.var()
t_via = opti.var(len(via_pnts))

g = []

g.append(x(0) == start_pnt[0]) #.data() ...
g.append(y(0) == start_pnt[1])
g.append(x(1) == end_pnt[0])
g.append(y(1) == end_pnt[1])

g.append(vx(0)==0)
g.append(vy(0)==0)
g.append(vx(1)==0)
g.append(vy(1)==0)

for k, via_pnt in enumerate(via_pnts):
    g.append(x([t_via[k]])== via_pnt[0])
    g.append(y([t_via[k]])== via_pnt[1])
    g.append(t_via[k]>=0.4)
    g.append(t_via[k]<=0.6)

print g

g.append(vx<=T*vmax)
g.append(vx>=-T*vmax)
g.append(vy<=T*vmax)
g.append(vy>=-T*vmax)

sol = opti.solver(T,g,"ipopt")

sol.value(x.getCoefficient(),np.linspace(start_pnt[0], end_pnt[0], m.dimension()))
sol.value(y.getCoefficient(),np.linspace(start_pnt[0], end_pnt[0], m.dimension()))

sol.value(T,2)

if len(via_pnts)>0:
  sol.value(t_via, 0.5)

sol.solve()

T = float(sol.value(T))
t_via = sol.value(t_via)

time = np.linspace(0, T, 101)

x = sol.value(x)

x.getBasis().setKnots(list(T*knots))

y = sol.value(y)

y.getBasis().setKnots(list(T*knots))

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
