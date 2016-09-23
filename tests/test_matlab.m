addpath('/home/erik/Documents/casadi-matlab3.0/');
addpath('../swig');

import Basis.*;

b = Basis.MonomialBasis(4);

disp(b.getDegree());

quit
