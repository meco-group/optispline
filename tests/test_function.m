meco_binaries('cpp_splines','fill_in_the_branch_you_want')

import Basis.*

p1 = Polynomial([0,0,1],'x');
p2 = Polynomial([0,1],'y');

assert(all(size(p1)==[1 1]))

p = p1+p2

p(2,3)

s1 = MonomialBasis(2);
s2 = MonomialBasis(3);
b = TensorBasis({s1,s2},{'x','y'});

fail = false;
try,p = b.basis(0),catch,fail = true;end
assert(fail);

fail = false;
try,p = b.basis(3),catch,fail = true;end
assert(fail);

p = b.basis(1);
assert(p.degree()== 2);
p = b.basis(2);
assert(p.degree()== 3);

p = b.basis(1);
assert(p.degree()== 2);
p = b.basis(2);
assert(p.degree()== 3);

p = b.basis('x');
assert(p.degree()== 2);
p = b.basis('y');
assert(p.degree()== 3);

