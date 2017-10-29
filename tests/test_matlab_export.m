meco_binaries('cpp_splines','develop')
import splines.*


% LPV model
n = 2;
A0 = [-0.05, 0.97; -1, 0.01]; 
A1 = [-1.63, -2.41; 1.94, 0.21];
B = [1;0]; 
C = [0,1];
D = 0;

% Parameter domain
aL = 0;     aU = 1;
daB = 0.15;
vertices = [aL    , daB;
            aU-daB, daB;
            aU    , 0  ;
            aU    , -daB;
            aL+daB, -daB;
            aL    , 0  ];

% Parameterization
dQ = 2;



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% B-spline

% Transformation
Ba = BSplineBasis([aL,aL,aL+daB,aU-daB,aU,aU], 1);
Ba = TensorBasis(Ba,'a');
Su = Function(Ba,[daB;daB;daB;0]);
Sl = Function(Ba,[0;-daB;-daB;-daB]);
Bdb = BSplineBasis([-daB,-daB,daB,daB], 1);
Bdb = TensorBasis(Bdb,'db');
da = Sl * Function(Bdb,[1;0]) + Su * Function(Bdb,[0;1]);

% B-spline relaxation
opti = OptiSplineYalmip();
a = Polynomial([0 1], 'a'); a = a.transform_to(BSplineBasis([aL,aU],1,2));
A = A0 + a*A1;
F = [eye(n), zeros(n,1); A, B; zeros(1,n), 1; C, D];

Qa = zeros(n);
Qada = zeros(n);
for i = 0:dQ
    eval(['Q_', num2str(i),' = opti.variable(n,n,''symmetric'');']);
    eval(['Qa = Qa + a^',num2str(i),' * Q_', num2str(i), ';']);
    eval(['Qada = Qada + (a+da)^',num2str(i),' * Q_', num2str(i), ';']);
end
gam = opti.variable(1);
Z = F' * blkdiag(-Qa, Qada, -gam, 1) * F;

g = {Qa.coeff_tensor.data, Z.coeff_tensor.data};
args = symvar(veccat(g{:}));
f_mx = casadi.Function('f',args,g);
f_sx = f_mx.expand();

opti.advanced.matlab_dump(f_mx, 'f_mx_exported.m')
opti.advanced.matlab_dump(f_sx, 'f_sx_exported.m')
rehash

rng(1);

N = f_mx.n_out;

args_num = {};
for i=1:f_mx.n_in
   a = args{i};
   args_num{i} = sparse(casadi.DM(sparsity(a),rand(nnz(a),1)));
end

[f_mx_res{1:N}] = f_mx(args_num{:});
[f_sx_res{1:N}]  = f_sx(args_num{:});

[f_mx_exported_res{1:N}] = f_mx_exported(args_num{:});
[f_sx_exported_res{1:N}] = f_sx_exported(args_num{:});

for i=1:length(f_mx_res)
    assert(norm(full(f_mx_res{i}-f_sx_res{i}))<1e-12);
    assert(norm(full(f_mx_res{i}-f_mx_exported_res{i}))<1e-12);
    assert(norm(full(f_mx_res{i}-f_sx_exported_res{i}))<1e-12);
end

delete('f_mx_exported.m')
delete('f_sx_exported.m')