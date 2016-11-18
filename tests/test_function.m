meco_binaries('cpp_splines','fill_in_the_branch_you_want')

import Basis.*

p1 = Polynomial([0,0,1],'x');
p2 = Polynomial([0,1],'y');

p = p1+p2

p(2,3)

