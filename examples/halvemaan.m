meco_binaries('cpp_splines','develop')
import splines.*



test_function = @(x,y,normal) (x - 0.5*normal)^2 + (y - 0.5*normal)^2;
test_function = @(x,y,normal)  x^4*y^2 + x^2*y^4 - 3*x^2*y^2*normal + normal^6;


knots = [0,0,0,1,1,1];

sq05 = sqrt(0.5);
weights = DTensor([1, sq05, 1], [3]);

b = BSplineBasis(knots, 2);

coef_cos_1 = sq05*DTensor([ 1,sqrt(2),1], [3]);
coef_sin_1 = sq05*DTensor([ -1, 0, 1 ], [3]);

cos1 = Function(TensorBasis(b, 'a') , coef_cos_1);
sin1 = Function(TensorBasis(b, 'a') , coef_sin_1);

noemer = Function(TensorBasis(b, 'a') , weights);

no = noemer^2;
s1 = sin1*noemer * sqrt(2);
c1 = cos1*noemer * sqrt(2) - no;
s2 = 2*sin1*cos1;
c2 = cos1^2 - sin1^2;

s = [s1, s2];
c = [c1, c2];

B = TensorBasis({b*b, BSplineBasis([0,1],1,2)}, {'a', 'r'});
s = Function(B, squeeze(s.coeff_tensor()));
c = Function(B, squeeze(c.coeff_tensor()));
p = test_function(c,s,no);

lower_bound_our = [];
for i = 0:6
    coef_ = p.midpoint_refinement(i).coeff_tensor();
    lower_bound_our = [lower_bound_our  min(coef_(:))];
end


lower_bound = [];
time = [];

for j  = 0:7
sdpvar x y lower
p = (1+x*y)^2-x*y+(1-y)^2;
% Motzkin polynomial

p = test_function(x,y,1);

g=[]

n = 3;
r = 2;

g = [-x^2-y^2+1; (x+1)^2+y^2-2];

F = [];
S = [];
C = [];
for g_ = g
    [s1,c1] = polynomial([x y],2 + j);
    S = [S s1];
    C = [C; c1];
    F = [F sos(s1)];
end

% F = [sos(p-lower-[s1 s2 s3 s4]*g), sos(s1), sos(s2), sos(s3), sos(s4)];
% solvesos(F,-lower,[],[c1;c2;c3;c4;lower]);
F = [sos(p-lower-S*g),F];
options = sdpsettings('verbose', 0', 'solver','sdpt3','sdpt3.maxit', 2000);

tic
sol = solvesos(F,-lower,[],[C; lower]);
sol.info
time =[time value(toc)];
lower_bound = [lower_bound value(lower)];

end

time
lower_bound
lower_bound_our


