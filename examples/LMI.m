meco_binaries('cpp_splines','')
import splines.*

options = sdpsettings('solver','lmilab','verbose',0);

opti = OptiSplineYalmip();
% LPD model
aL = 1;
aU = 2;

a = Polynomial([0 1]);
a = a.transform_to(BSplineBasis([aL,aU],1,2));

A =  [0 1;-a*a -2*a];
B = [0; 1];
C = [0, 1];
D = [0];
nx = 2;


% Hinf norms by gridding
a_ = linspace(aL, aU, 1e2)';

gamma_grid = zeros(size(a_));
for i = 1:length(a_)
    sys = ss(A.eval(a_(i)), B, C, D);
    gamma_grid(i) = norm(sys, inf);
end


% Primal LMI
dP = 2;
nP = 1;
BP = BSplineBasis([aL*ones(dP,1); linspace(aL,aU,nP+2)';  aU*ones(dP,1)], dP);
dg = 2;
ng = 1;
Bg = BSplineBasis([aL*ones(dg,1); linspace(aL,aU,ng+2)';  aU*ones(dg,1)], dg);


P = opti.Function({BP}, [nx,nx],'symmetric');
gamma = opti.Function({Bg}, [1,1],'symmetric');

Z = [A'*P+P*A, P*B   , C'    ;
     B'*P    , -gamma, D'    ;
     C       , D     , -gamma];
 
sol = opti.solver(gamma.integral, {Z<=0, P>=0}, 'yalmip', struct('yalmip_options', options));
sol.solve();

gamma_prim = sol.value(gamma);

% Dual LMI
E = [zeros(nx+1), [C,D]'; [C,D], 0];
AB = [A, B, zeros(nx,1)];
IO = [eye(nx), zeros(nx,2)];

dZ = 2;
nZ = 1;
BZ = BSplineBasis([aL*ones(dZ,1); linspace(aL,aU,nZ+2)';  aU*ones(dZ,1)], dZ);

Z = opti.Function({BZ}, [nx+2,nx+2], 'symmetric');
obj = trace(E*Z);
Q = AB*Z*IO' + IO*Z*AB';



sol = opti.solver(-obj.integral, {Q >= 0, Z >= 0, Z(nx+1,nx+1)+Z(nx+2,nx+2)<=1}, 'yalmip', struct('yalmip_options', options));
sol.solve();

gamma_dual = sol.value(obj);


% Overview figure
figure
plot(a_, [gamma_grid, gamma_prim.eval(a_), gamma_dual.eval(a_)])
xlabel('a'), ylabel('gamma'),

