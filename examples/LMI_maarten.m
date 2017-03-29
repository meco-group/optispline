clear all
close all
clc

% Load plant - from example file satelite tracking
load('plant.mat')
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

%   2.1. Declaring the variables        
X = sdpvar(n);      % nxn top-left subblock of P
Y = sdpvar(n);      % nxn top-left subblock of inv(P)
Gamma = sdpvar(mz); % Lyapunov shaping

c = zeros(mz,1);
for i = 1:noc
    c(Iz1(i):Iz2(i)) = alpha(i)/Mz(i);
    if alpha(i) == 0
        Gamma(Iz1(i):Iz2(i),Iz1(i):Iz2(i)) = eye(Mz(i));
    else
        Gamma(Iz1(i):Iz2(i),Iz1(i):Iz2(i)) = sdpvar(1) * eye(Mz(i));
    end
end

%   2.2. Specifying the LMIs
ABw = [A, Bw, zeros(n,mz); eye(n), zeros(n,mw+mz)];
Zx = ABw'*kron(Phi,X)*ABw + [zeros(n,n+mw),Cz'; zeros(mw,n),-eye(mw),Dzw'; Cz,Dzw,-Gamma];
ACz = [A', Cz', zeros(n,mw); eye(n), zeros(n,mz+mw)];
Zy = ACz'*kron(Phi,Y)*ACz + [zeros(n,n+mz),Bw; zeros(mz,n),-Gamma,Dzw; Bw',Dzw',-eye(mw)];
constr = ([X,eye(n);eye(n),Y] >= 0) + (V'*Zx*V <= 0) + (W'*Zy*W <= 0);

%   2.3. Specifying the objective
goal = trace(diag(c)*Gamma);

%   2.4. Solving the SDP
options = sdpsettings('solver','lmilab');
sol = solvesdp(constr, goal, options);

X = double(X);
Y = double(Y);
Gamma = double(Gamma);
gamma = diag(Gamma);
gamma = sqrt(gamma(Iz1));

%   3.0 Check if the solution checks out with what we computed before
assert(gamma(1,1)==gamma_check(1,1))