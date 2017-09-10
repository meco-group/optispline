meco_binaries('cpp_splines','')
import splines.*


opti = OptiSplineYalmip();
opti = opti.advanced;
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

constr = opti.yalmip_expr({Z<=0, P>=0});

options = sdpsettings('solver','lmilab','verbose',true);
sol = optimize([constr{:}], opti.yalmip_expr(gamma.integral), options);

Pv = opti.yalmip_var(P.coeff_tensor());
for i=1:4
  double(Pv{i})
end
