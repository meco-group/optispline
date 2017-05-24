meco_binaries('cpp_splines','fill_in_the_branch_you_want')

import splines.*

try
  F = splines.Function(3);
except
  
end

knots = linspace(0,1,20);

degree = 3;

B2 = BSplineBasis(knots,degree);


123

B2.knots()

b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,1,1));
s1 = splines.Function(b1, c1);

coeff_tensor(s1+casadi.SX.sym('x',2))
coeff_tensor(casadi.SX.sym('x',2)+s1)

w = DTensor(rand(2,2,2))
w+casadi.SX.sym('x')
casadi.SX.sym('x')+w

b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,4,2));
s1 = splines.Function(b1, c1);

assert(norm(size(s1)-[4,2])==0)
assert(norm(size(s1,1)-[4])==0)
assert(norm(size(s1,2)-[2])==0)
%assert(norm(numel(s1)-[8])==0)
assert(norm(size(s1(1:end,1:end),1)-[4])==0)
assert(norm(size(s1(1:end,1:end),2)-[2])==0)
assert(norm(size(s1(2:end,2:end),3)-[1])==0)
assert(norm(size(s1(2:end,2:end),2)-[1])==0)

a = DTensor(rand(4,4,2));
a(:)
