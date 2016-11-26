meco_binaries('cpp_splines','fill_in_the_branch_you_want')

import Basis.*

p1 = Polynomial([0,0,1],'x');
p2 = Polynomial([0,1],'y');

p = p1+p2

p(2,3)

s1 = MonomialBasis(2);
s2 = MonomialBasis(3);
b = TensorBasis({s1,s2});

fail = false;
try,p = b.getBasis(0),catch,fail = true;end
assert(fail);

fail = false;
try,p = b.getBasis(3),catch,fail = true;end
assert(fail);

p = b.getBasis(1);
assert(p.getDegree()== 2);
p = b.getBasis(2);
assert(p.getDegree()== 3);

b.setArguments({'x','y'});

p = b.getBasis(1);
assert(p.getDegree()== 2);
p = b.getBasis(2);
assert(p.getDegree()== 3);

p = b.getBasis('x');
assert(p.getDegree()== 2);
p = b.getBasis('y');
assert(p.getDegree()== 3);

