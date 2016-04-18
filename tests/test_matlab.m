addpath('/home/erik/Documents/casadi-matlab3.0/');
addpath('../swig');

import Basis.*;

b = Basis.MonomialeBasis(4);

disp(b.getDegree());

quit
