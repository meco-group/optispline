import sys, os

from Basis import *
import numpy as np
import matplotlib.pyplot as plt
import casadi as cas


def derivative(coeffs, knots1, degree, o=1):
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
        coeffs_out = P.dot(coeffs)
    return coeffs_out, knots_out

vmax = 0.5
start_pnt = [0., 0.]
end_pnt = [1., 1.]
via_pnts = [[0.5, 0.5]]

via_pnts = []

degree = 3
knots = np.r_[np.zeros(degree), np.linspace(0, 1, 10+1), np.ones(degree)]
m = BSplineBasis(knots, degree)
basis = TensorBasis([m])

x_cfs_ = cas.SX.sym('x', m.getLenght())
x_cfs = STensor(x_cfs_, [m.getLenght(), 1, 1]) # wat is dees?
x_cfs = Coefficient(x_cfs)
x = Function(basis, x_cfs) # also in casadi

y_cfs_ = cas.SX.sym('y', m.getLenght())
y_cfs = STensor(y_cfs_, [m.getLenght(), 1, 1])
y_cfs = Coefficient(y_cfs)
y = Function(basis, y_cfs) # also in casadi

vx_cfs_, knots2 = derivative(x_cfs_, knots, degree) # derivative should be in toolbox
vy_cfs_, knots2 = derivative(y_cfs_, knots, degree)
m2 = BSplineBasis(knots2, degree-1)
basis2 = TensorBasis([m2])

vx_cfs = STensor(vx_cfs_, [m2.getLenght(), 1, 1])
vx_cfs = Coefficient(vx_cfs)
vx = Function(basis2, vx_cfs)

vy_cfs = STensor(vy_cfs_, [m2.getLenght(), 1, 1])
vy_cfs = Coefficient(vy_cfs)
vy = Function(basis2, vy_cfs)

T = cas.SX.sym('T')
t_via = cas.SX.sym('t_via', len(via_pnts))

g = []
lbg = []
ubg = []

g.append(x([0]).data() - start_pnt[0]) #.data() ...
lbg.append(0)
ubg.append(0)
g.append(y([0]).data() - start_pnt[1])
lbg.append(0)
ubg.append(0)
g.append(x([1]).data() - end_pnt[0])
lbg.append(0)
ubg.append(0)
g.append(y([1]).data() - end_pnt[1])
lbg.append(0)
ubg.append(0)

g.append(vx([0]).data())
lbg.append(0)
ubg.append(0)
g.append(vy([0]).data())
lbg.append(0)
ubg.append(0)
g.append(vx([1]).data())
lbg.append(0)
ubg.append(0)
g.append(vy([1]).data())
lbg.append(0)
ubg.append(0)

for k, via_pnt in enumerate(via_pnts):
    g.append(x([t_via[k]]).data() - via_pnt[0])
    lbg.append(0)
    ubg.append(0)
    g.append(y([t_via[k]]).data() - via_pnt[1])
    lbg.append(0)
    ubg.append(0)
    g.append(t_via[k])
    lbg.append(0.4)
    ubg.append(0.6)


g.append(vx.getCoefficient().getData().data() - T*vmax);
for _ in range(vx.getCoefficient().getData().data().shape[0]): # lang...
    lbg.append(-cas.inf)
    ubg.append(0)
g.append(-vx.getCoefficient().getData().data() + T*vmax);
for _ in range(vx.getCoefficient().getData().data().shape[0]):
    lbg.append(0)
    ubg.append(cas.inf)
g.append(vy.getCoefficient().getData().data() - T*vmax);
for _ in range(vy.getCoefficient().getData().data().shape[0]):
    lbg.append(-cas.inf)
    ubg.append(0)
g.append(-vy.getCoefficient().getData().data() + T*vmax);
for _ in range(vy.getCoefficient().getData().data().shape[0]):
    lbg.append(0)
    ubg.append(cas.inf)

x_init = np.r_[np.linspace(start_pnt[0], end_pnt[0], m.getLenght()), np.linspace(start_pnt[1], end_pnt[1], m.getLenght()), 2., 0.5]
nlp = {'x': cas.vertcat(x_cfs_, y_cfs_, T, t_via), 'f': T, 'g': cas.vertcat(*g) }
solver = cas.nlpsol("solver", "ipopt", nlp)
r = solver(lbg = lbg, ubg = ubg)

sol = r['x'].full()
x_cfs_ = sol[:m.getLenght()]
y_cfs_ = sol[m.getLenght():2*m.getLenght()]
T = sol[2*m.getLenght()]
t_via = sol[2*m.getLenght()+1:]

x_cfs = DTensor(x_cfs_, [m.getLenght(), 1, 1])
x_cfs = Coefficient(x_cfs)
y_cfs = DTensor(y_cfs_, [m.getLenght(), 1, 1])
y_cfs = Coefficient(y_cfs)

knotsT = T*knots
basis = TensorBasis([BSplineBasis(knotsT, degree)])
time = np.linspace(0, T, 101)

x = Function(basis, x_cfs)
y = Function(basis, y_cfs)


vx_cfs_, knotsT2 = derivative(x_cfs_, knotsT, degree)
vy_cfs_, knotsT2 = derivative(y_cfs_, knotsT, degree)
basis2 = TensorBasis([BSplineBasis(knotsT2, degree-1)])

vx_cfs = DTensor(vx_cfs_, [m2.getLenght(), 1, 1])
vx_cfs = Coefficient(vx_cfs)
vx = Function(basis2, vx_cfs)

vy_cfs = DTensor(vy_cfs_, [m2.getLenght(), 1, 1])
vy_cfs = Coefficient(vy_cfs)
vy = Function(basis2, vy_cfs)


x_s = np.array([x([t]).data() for t in time.tolist()])
y_s = np.array([y([t]).data() for t in time.tolist()])

vx_s = np.array([vx([t]).data() for t in time.tolist()])
vy_s = np.array([vy([t]).data() for t in time.tolist()])


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

# plt.show(block=True)
