meco_binaries('cpp_splines','')
import splines.*

clear all

% Load plant - from example file satelite tracking
load('../examples/plant.mat')
gamma_check = gamma;
my = nmeas;
mu = ncont;

% Create LMIs - from mixedHinfsyn
% 1.0. Continuous time!
Phi = [0,1;1,0];

% 1.1. State-space model and dimensions
[A,B,C,D] = ssdata(plant);
n = length(A(:,1));
mz = sum(Mz);
mw = length(B(1,:))-mu;

% 1.2. Subdivision according to inputs and outputs
                        Bw = B(:,1:mw);             Bu = B(:,mw+1:end);
Cz = C(1:mz,:);         Dzw = D(1:mz,1:mw);         Dzu = D(1:mz,mw+1:end);
Cy = C(mz+1:end,:);     Dyw = D(mz+1:end,1:mw);

% 1.3. Various Hinf constraints
Mz = Mz(:);
alpha = alpha(:);
noc = length(alpha);
Iz2 = cumsum(Mz);
Iz1 = [1;Iz2(1:end-1)+1];

% STEP 2. SDP to compute the optimal gamma_i
% ------------------------------------------

% 2.0. Preprocessing: null-spaces
V = null([Cy, Dyw]);
[rV,cV] = size(V);
V = [V, zeros(rV,mz); zeros(mz,cV), eye(mz)];

W = null([Bu', Dzu']);
[rW,cW] = size(W);
W = [W, zeros(rW,mw); zeros(mw,cW), eye(mw)];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% IMPORTANT STUFF FOR JORIS STARTS HERE %%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


opti = OptiSplineYalmip();

%   2.1. Declaring the variables        
X = opti.variable(n,n,'full');      % nxn top-left subblock of P
Y = opti.variable(n,n,'symmetric');      % nxn top-left subblock of inv(P)
Gamma = opti.variable(mz, mz, 'symmetric'); % Lyapunov shaping

c = zeros(mz,1);
for i = 1:noc
    c(Iz1(i):Iz2(i)) = alpha(i)/Mz(i);
    if alpha(i) == 0
        Gamma(Iz1(i):Iz2(i),Iz1(i):Iz2(i)) = eye(Mz(i));
    else
        Gamma(Iz1(i):Iz2(i),Iz1(i):Iz2(i)) = opti.variable(1,1,'symmetric') * eye(Mz(i));
    end
end

%   2.2. Specifying the LMIs
ABw = [A, Bw, zeros(n,mz); eye(n), zeros(n,mw+mz)];
Zx = ABw'*kron(Phi,X)*ABw + [zeros(n,n+mw),Cz'; zeros(mw,n),-eye(mw),Dzw'; Cz,Dzw,-Gamma];
ACz = [A', Cz', zeros(n,mw); eye(n), zeros(n,mz+mw)];
Zy = ACz'*kron(Phi,Y)*ACz + [zeros(n,n+mz),Bw; zeros(mz,n),-Gamma,Dzw; Bw',Dzw',-eye(mw)];

%   2.3. Specifying the objective
goal = trace(diag(c)*Gamma);

%   2.4. Solving the SDP
options = sdpsettings('solver','lmilab','verbose',2);
opti.solver('yalmip', struct('yalmip_options', options));
opti.minimize(goal);


M = [vec([X;X])];
M2 = [X(3,:);X(2,:);X(1:3,:)]+3;
M3 = vertsplit(X,2);
M4 = vertsplit(vec(X),2);
M5 = diagcat(X,2*X)
g = {M, M2, M3{1}, M4{1},M5,M5+3};

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
