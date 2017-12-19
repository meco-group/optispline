import numpy as np
import time
import matplotlib.pyplot as plt

import meco_binaries
meco_binaries(cpp_splines="develop")
from splines import *

coef_f_poly = [10,1,4,0,-1]
f = Polynomial(coef_f_poly)


r = np.linspace(-2,2,201)
f_eval = f.list_eval(r)

plt.subplot(2,1,1)
plt.plot(r, f_eval)

b = BSplineBasis([-2,2],1, 3)

g = f.transform_to(b)

plt.plot(g.basis().greville(), g.coeff_tensor().flatten())
opti = OptiSpline()

alpha = opti.variable()
# opti.subject_to(g<=alpha)  Werkt niet
opti.subject_to(alpha - g >= 0)
opti.solver('ipopt')
opti.minimize(alpha)
sol = opti.solve()

plt.plot([-2, 2], [sol.value(alpha)] *2)

plt.subplot(2,1,2)
plt.plot(r, f_eval)

b = BSplineBasis([-2,2],1, 6)

g = f.transform_to(b)

plt.plot(g.basis().greville(), g.coeff_tensor().flatten())

opti = OptiSpline()

alpha = opti.variable()
# opti.subject_to(g<=alpha)  Werkt niet
opti.subject_to(alpha - g >= 0)
opti.solver('ipopt')
opti.minimize(alpha)
sol = opti.solve()

plt.plot([-2, 2], [sol.value(alpha)] *2)
plt.show()

#########################################################
### simple multi variate global optinmization problem ###
#########################################################
grens = 1.25
knots = [0.5]
b = BSplineBasis([0, 0, 0.8, grens, grens], 1)
# b = BSplineBasis([0, grens], 1, 6)

x = Polynomial([0,1],'x')
y = Polynomial([0,1],'y')

F = x**4 * y**2 +  x**2 * y**4 - 3 * x**2 * y**2 + 1

F = F.transform_to(TensorBasis([ b,b ]))

args = ['x', 'y']

from mpl_toolkits.mplot3d import Axes3D

import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

fig = plt.figure()
ax = fig.gca(projection='3d')

# Make data.
X_grid = np.linspace(0,grens, 100)
Y_grid = np.linspace(0,grens, 100)
grid = np.meshgrid(X_grid, Y_grid)

Z = F.grid_eval([X_grid, Y_grid])[:,:,0,0]

# Plot the surface.
X_G, Y_G = np.meshgrid(X_grid, Y_grid)
surf = ax.plot_surface(X_G, Y_G, Z,
                       linewidth=0, antialiased=False, alpha=0.5)

X_wire, Y_wire = np.meshgrid(F.basis(0).greville() , F.basis(1).greville())
print(X_wire)
print(Y_wire)
C = F.coeff_tensor()[:,:,0,0]
ax.plot_wireframe(X_wire, Y_wire, C, linewidth=0.8 , color="black")
X_G, Y_G = np.meshgrid([0, grens], [0, grens])
surf = ax.plot_surface(X_G, Y_G, np.ndarray.min(C),
                       linewidth=0, antialiased=False, alpha=0.3, color="grey")
# ax.plot_wireframe(F.basis(0).greville() , F.basis(1).greville(), F.coeff_tensor()[:,:,0,0], rstride=10, cstride=10)
ax.set_xlim(0, grens)
ax.set_ylim(0, grens)
ax.set_zlim(-0.4, 1.4)
plt.show()
