% Parametric programming to compute approximation of the trade-off curves
% considered in:
%   G. Pipeleers, B. Demeulenaere, J. De Schutter, and J. Swevers,
%   "Robust high-order repetitive control: Optimal performance trade-offs,"
%   Automatica, 44(10), 2628�2634, 2008.
%
%
% This file is part of splines -- A framework for manipulating and optimizing (B-)splines
% Copyright (C) 2015 Wannes Van Loock
%
% splines is free software: you can redistribute it and/or modify
% it under the terms of the GNU Lesser General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% splines is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU Lesser General Public License for more details.
%
% You should have received a copy of the GNU Lesser General Public License
% along with splines.  If not, see <http://www.gnu.org/licenses/>.

clear all
close all
clear global
clc

meco_binaries('cpp_splines','develop')
import splines.*

opti = OptiSplineYalmip();

% Input data
% ==========

% Uncertainty on f0
delta = 0.05;               % relative error periodic frequency
Delta_theta = delta*2*pi;
t = -2*cos(Delta_theta);

% Data for repetitive controller
M = 3;                      % order of W(z^N)
Lb = 1;
Ub = 4;
Gamma_np = linspace(Lb,Ub,301)';

% Auxiliaries
A = zeros(M,M);     A(1:end-1,2:end) = eye(M-1);
B = zeros(M,1);     B(end) = 1;
D = 1;

% Data for parametric approximation
deg = 3;
n = 5;
Bp = BSplineBasis([Lb Ub], deg, n);
knots_v = [1.2;1.4;2.4;2.6];    % refinement of the parametrization, set to [] for none

Bp = Bp.insert_knots(knots_v);
knots_c = (Lb:0.1:Ub)';         % refinement of the relaxation, set to [] for none

options = sdpsettings('solver','MOSEK','verbose',2,'mosek.MSK_DPAR_INTPNT_TOL_STEP_SIZE',0);



% Preprocessing for dual
% ======================

I0 = [eye(M), zeros(M,1)];
AB = [A, B];
I = eye(M);
G = zeros(M+1,M+1);
G(end,end) = 1;

F_matrices = zeros(M+1,M+1,M+1);
f = zeros(M+1,1);
H_matrices = zeros(M,M,M+1);

X = dlyap(A,B*B');
F = [X, zeros(M,1); zeros(1,M), 1];
F_matrices(:,:,1) = F;
f(1) = trace(F*G);
H_matrices(:,:,1) = I0*F*AB' + AB*F*I0' + t*I0*F*I0';

for i = 1:M
    u = I(:,i);
    Q = B*u'*A'+A*u*B';
    X = dlyap(A,Q);
    F = [X, u; u', 0];
    F_matrices(:,:,i+1) = F;
    f(i+1) = trace(F*G);
    H_matrices(:,:,i+1) = I0*F*AB' + AB*F*I0' + t*I0*F*I0';
end

string_F1 = 'F1 = u1(1)*F_matrices(:,:,1)';
string_F2 = 'F2 = u2(1)*F_matrices(:,:,1)';
string_H1 = 'H1 = u1(1)*H_matrices(:,:,1)';
for i = 2:M+1
    string_F1 = [string_F1, ' + u1(', num2str(i), ')*F_matrices(:,:,', num2str(i), ')'];
    string_F2 = [string_F2, ' + u2(', num2str(i), ')*F_matrices(:,:,', num2str(i), ')'];
    string_H1 = [string_H1, ' + u1(', num2str(i), ')*H_matrices(:,:,', num2str(i), ')'];
end
string_F1 = [string_F1, ';'];
string_F2 = [string_F2, ';'];
string_H1 = [string_H1, ';'];

% Tradeoff curve by gridding primal
% =================================

try load tradeoff_gridded
catch

Gamma_p1 = zeros(size(Gamma_np));
CPU_1 = zeros(size(Gamma_np));

for i = 1:length(Gamma_np)
    C = sdpvar(1,M);
    gamma_np = Gamma_np(i);
    gamma_p = sdpvar(1,1);
    P1 = sdpvar(M);
    Q1 = sdpvar(M);
    P2 = sdpvar(M);

    Z1 = [(A'*P1*A-P1)+(Q1*A+A'*Q1+t*Q1), A'*P1*B+Q1*B; B'*P1*A+B'*Q1, B'*P1*B-gamma_p];
    Z2 = [A'*P2*A-P2, A'*P2*B; B'*P2*A, B'*P2*B-gamma_np];
    constr = [Q1 >= 0, [Z1, [C';D]; [C,D], -gamma_p] <= 0, [Z2, [C';D]; [C,D], -gamma_np] <= 0];
    goal = gamma_p;

    sol = solvesdp(constr, goal);
    CPU_1(i) = sol.solvertime;
    Gamma_p1(i) = value(gamma_p);

end

save tradeoff_gridded Gamma_np CPU_1 Gamma_p1

end

figure
plot(Gamma_np, Gamma_p1)
hold all
axis tight, grid on
xlabel('\gamma_{np}')
ylabel('\gamma_{p}')

% Parametric approximation of primal
% ==================================

C = opti.Function(Bp,[1,M]);
gamma_np = Parameter();
gamma_p = opti.Function(Bp, [1,1]);
P1 = opti.Function(Bp, [M,M],'symmetric');
Q1 = opti.Function(Bp, [M,M],'symmetric');
P2 = opti.Function(Bp, [M,M],'symmetric');

Z1 = [A'*P1*A-P1 + Q1*A+A'*Q1+t*Q1, A'*P1*B + Q1*B   , C'      ;
      B'*P1*A    + B'*Q1          , B'*P1*B - gamma_p, D       ;
      C                           , D                ,-gamma_p ];
Z2 = [A'*P2*A-P2, A'*P2*B         , C'     ;
      B'*P2*A   , B'*P2*B-gamma_np, D      ;
      C         , D               ,-gamma_np];


constr = {Q1 >= 0, Z1 <= 0, Z2 <= 0};
goal = gamma_p;
opti.minimize(goal.integral);
opti.subject_to(constr);
opti.solver('yalmip', struct('yalmip_options', options));
sol = opti.solve();

gamma_p1 = sol.value(gamma_p);
plot(Gamma_np, gamma_p1.list_eval(Gamma_np))

 constr = {Q1.insert_knots({knots_c},1) >= 0, Z1.insert_knots({knots_c},1) <= 0, Z2.insert_knots({knots_c},1) <= 0};
 goal = gamma_p;

 opti.minimize(goal.integral);
 opti.subject_to();
 opti.subject_to(constr);
 
 sol = opti.solve();
 gamma_p1 = sol.value(gamma_p);
 plot(Gamma_np, gamma_p1.list_eval(Gamma_np))




 % Parametric approximation of dual
 % ================================

 gamma_np = Parameter();
 u1 = opti.Function(Bp, [M+1,1]);
 v11 = opti.Function(Bp, [M,1]);
 v1 = [v11; opti.Function(Bp, [1,1])];
 w1 = opti.Function(Bp, [1,1]);
 u2 = opti.Function(Bp, [M+1,1]);
 v2 = opti.Function(Bp, [1,1]);
 w2 = opti.Function(Bp, [1,1]);
 v21 = [-v11;v2];

 eval(string_F1);
 eval(string_F2);
 eval(string_H1);
 Z1 = [F1, v1; v1', w1];
 Z2 = [F2, v21; v21', w2];

 constr = { 1 - w1 - u1'*f == 0, Z1 >= 0, Z2 >= 0, H1 >= 0 };
 goal =  gamma_np*(w2+u2'*f) - 2*D*v1(M+1) - 2*D*v2;

 opti.minimize(goal.integral);
 opti.subject_to();
 opti.subject_to(constr);
 sol = opti.solve();
 gamma_p2 = -sol.value(goal);
 plot(Gamma_np, gamma_p2.list_eval(Gamma_np));

 constr = { 1 - w1 - u1'*f == 0, Z1.insert_knots({knots_c},1) >= 0, Z2.insert_knots({knots_c},1) >= 0, H1.insert_knots({knots_c},1) >= 0 };
 goal =  gamma_np*(w2+u2'*f) - 2*D*v1(M+1) - 2*D*v2;

 opti.minimize(goal.integral);
 opti.subject_to();
 opti.subject_to(constr);
 sol = opti.solve();
 gamma_p2 = -sol.value(goal);
 plot(Gamma_np, gamma_p2.list_eval(Gamma_np));
