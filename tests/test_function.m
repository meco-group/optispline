meco_binaries('cpp_splines','fill_in_the_branch_you_want')

import Basis.*

p1 = Polynomial([0,0,1],'x');
p2 = Polynomial([0,1],'y');

assert(all(size(p1)==[1 1]))

p = p1+p2

p.eval(2,3)

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


basis1 = BSplineBasis([0,0,0.4,1,1],1);
basis2 = MonomialBasis(2);
mbasis = TensorBasis({basis1,basis2});


coeff2 = DTensor(rand(9*12,1),[3,3,3,4]);
func2 = Function(mbasis,coeff2);
        
x = rand()
y = rand()

R = func2.eval(x,y);

assert(norm(func2([1,2],[3,2]).eval(x,y)-R([1,2],[3,2]))<1e-12)
assert(norm(func2(:,[3,2]).eval(x,y)-R(:,[3,2]))<1e-12)
assert(norm(func2([1,2],:).eval(x,y)-R([1,2],:))<1e-12)
assert(norm(func2([1,2],2).eval(x,y)-R([1,2],2))<1e-12)
assert(norm(func2(2,[3,2]).eval(x,y)-R(2,[3,2]))<1e-12)
assert(norm(func2(2,1).eval(x,y)-R(2,1))<1e-12)
assert(norm(func2(1:3,2).eval(x,y)-R(1:3,2))<1e-12)
assert(norm(func2(2:3,2:4).eval(x,y)-R(2:3,2:4))<1e-12)

