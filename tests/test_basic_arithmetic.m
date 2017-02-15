meco_binaries('cpp_splines','develop')

import Basis.*

%% plus

% scalar spline constant coeffs + scalar spline constant coeffs
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(ones(3,1,1));
s1 = Basis.Function(b1, c1); % check if dimensions basis and coefficient 
                             % are consistent!!!

b2 = BSplineBasis([0,0,1,1],1);
c2 = Coefficient(2*ones(2,1,1));
s2 = Basis.Function(b2, c2); 

s3 = s1 + s2;
assert(isequal(s3.getCoefficient.getData,3*ones(3,1,1)));

% scalar spline constant rand coeffs + scalar spline constant rand coeffs
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,1,1));
s1 = Basis.Function(b1, c1);

b2 = BSplineBasis([0,0,1,1],1);
c2 = Coefficient(rand(2,1,1));
s2 = Basis.Function(b2, c2);

s3 = s1 + s2;
assert(abs(s3(0.6) - s1(0.6) - s2(0.6)) <= eps);

% scalar spline constant coeffs + scalar double
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,1,1));
s1 = Basis.Function(b1, c1);

% s2 = s1 + 5; % spline + double % does not work yet
% assert(abs(s2(0.6) - s1(0.6) - 5) <= eps); 

% s2 = 5 + s1; % double + spline % does not work yet
% assert(abs(s2(0.6) - s1(0.6) - 5) <= eps);


%% uminus

% % scalar spline constant coeffs
% b1 = BSplineBasis([0,0,1/2,1,1],1);
% c1 = Coefficient(ones(3,1,1));
% s1 = Basis.Function(b1, c1);
% m1 = -s1;
% assert(isequal(m1.getCoefficient.getData,-ones(3,1,1)));
% 
% % scalar spline constant rand coeffs
% b1 = BSplineBasis([0,0,1/2,1,1],1);
% c1 = Coefficient(rand(3,1,1));
% s1 = Basis.Function(b1, c1);
% m1 = -s1;
% assert(abs(m1(0) + s1(0)) < eps);


%% minus

% scalar spline constant coeffs - scalar spline constant coeffs
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(ones(3,1,1));
s1 = Basis.Function(b1, c1);

b2 = BSplineBasis([0,0,1,1],1);
c2 = Coefficient(2*ones(2,1,1));
s2 = Basis.Function(b2, c2);

s3 = s1 - s2;
assert(isequal(s3.getCoefficient.getData,-ones(3,1,1)));

% scalar spline constant rand coeffs + scalar spline constant rand coeffs
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,1,1));
s1 = Basis.Function(b1, c1);

b2 = BSplineBasis([0,0,1,1],1);
c2 = Coefficient(rand(2,1,1));
s2 = Basis.Function(b2, c2);

s3 = s1 - s2;
assert(abs(s3(0.6) - s1(0.6) + s2(0.6)) <= eps);

% scalar spline constant coeffs + scalar double
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,1,1));
s1 = Basis.Function(b1, c1);

% s2 = s1 - 5; % spline + double % does not work yet
% assert(abs(s2(0.6) - s1(0.6) + 5) <= eps); 

% s2 = 5 - s1; % double + spline % does not work yet
% assert(abs(s2(0.6) + s1(0.6) - 5) <= eps);


%% mtimes

% scalar spline constant coeffs
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(4*ones(3,1,1));
s1 = Basis.Function(b1, c1);

% scalar spline random coeffs
b2 = BSplineBasis([0,0,1,1],1);
c2 = Coefficient(rand*ones(2,1,1));
s2 = Basis.Function(b1, c1);

% p1 = s1*3; % does not work yet
% p2 = 3*s1; % does not work yet
p3 = s1*s1;
p4 = s1*s2;
p5 = s2*s2;
assert(p3(1) == 16);
assert(abs(p4(0.3) - s1(0.3)*s2(0.3)) < 1e-14);
assert(abs(p5(0.3) - s2(0.3)*s2(0.3)) < 1e-14);


%% power

% to the power 0

% scalar spline random coeffs





% %
% % test addition of B-splines
% %
% 
% %% Bspline + double
% 
% % univariate matrix-valued
% a = parameter(1); a = a.to_bspline({[0,6]});
% A = [-0.03-0.3*a; -0.47+0.9*a]; B = [2; 3]; C = A + B;
% d = C - [-0.03+2-0.3*a; -0.47+3+0.9*a];
% assert(max(abs(d.coeffs.data)) < 1e-13,'univariate B-spline + double')
% 
% % bivariate matrix-valued 
% ab = parameter(2); ab = ab.to_bspline({[0,6],[-1,1]}); a = ab(1); b = ab(2);
% A = [0.3*a + b, -1+2*a; 2, -5*b]; B = [1, 2; 3, 4]; C = A + B;
% d = C - [0.3*a + b + 1, 1+2*a; 5, 4-5*b];
% assert(max(max(abs(d.coeffs.data))) < 1e-13,'multivariate B-spline + double')
% 
% % trivariate matrix-valued 
% abc = parameter(3); abc = abc.to_bspline({[0,6],[-1,1],[2,3]}); a = abc(1); b = abc(2); c = abc(3);
% A = [0.3*a + b - c, -1+2*a; 2*c, -5*b]; B = [1, 2; 3, 4]; C = A + B;
% d = C - [0.3*a + b - c + 1, 1+2*a; 2*c+3, 4-5*b];
% assert(max(max(abs(d.coeffs.data))) < 1e-13,'multivariate B-spline + double')
% 
% 
% %% Bspline + SDPVAR
% 
% % univariate matrix-valued 
% a = parameter(1); a = a.to_bspline({[0,6]});
% A = [1, a; 2, -5*a]; 
% % B = BSpline.sdpvar({BSplineBasis([0 6],0)},[2,2],'symmetric'); 
% B = sdpvar(2,2);
% C = A + B; assert(isa(C,'BSpline'),'multivariate B-spline + SDPVAR')
% % C = B + A; assert(isa(C,'BSpline'),'SDPVAR + multivariate B-spline')
% 
% % bivariate matrix-valued 
% ab = parameter(2); ab = ab.to_bspline({[0,6],[-1,1]}); a = ab(1); b = ab(2);
% A = [0.3*a + b, -1+2*a; 2, -5*b];
% % B = BSpline.sdpvar({BSplineBasis([0 6],0),BSplineBasis([-1 1],0)},[2,2],'symmetric'); 
% B = sdpvar(2,2);
% C = A + B; assert(isa(C,'BSpline'),'multivariate B-spline + SDPVAR')
% % C = B + A; assert(isa(C,'BSpline'),'SDPVAR + multivariate B-spline')
% 
% % trivariate matrix-valued
% abc = parameter(3); abc = abc.to_bspline({[0,6],[-1,1],[2,3]}); a = abc(1); b = abc(2); c = abc(3);
% A = [0.3*a + b - c, -1+2*a; 2*c, -5*b];
% B = sdpvar(2,2);
% C = A + B; assert(isa(C,'BSpline'),'multivariate B-spline + SDPVAR')
% % C = B + A; assert(isa(C,'BSpline'),'SDPVAR + multivariate B-spline')
% 
% 
% %% Bspline + Bspline
% 
% % univariate matrix-valued 
% a = parameter(1); a = a.to_bspline({[0,6]});
% A = [1, a; 2, -5*a]; B = [a, 3; 1, 2+a];
% C = A + B; 
% d = C - [1+a, 3+a; 3, 2 - 4*a];
% assert(max(max(d.coeffs.data)) < 1e-13,'univariate B-spline + univariate B-spline')
% 
% % bivariate matrix-valued 
% ab = parameter(2); ab = ab.to_bspline({[0,6],[-1,1]}); a = ab(1); b = ab(2);
% A = [0.3*a + b, -1+2*a; 2, -5*b]; B = [1, 2*a; 3*b, 4*a*b];
% C = A + B;
% d = C - [1+0.3*a+b, -1+4*a; 2+3*b, 4*a*b - 5*b];
% assert(max(max(d.coeffs.data)) < 1e-13,'multivariate B-spline + multivariate B-spline')
% 
% % trivariate matrix-valued 
% abc = parameter(3); abc = abc.to_bspline({[0,6],[-1,1],[2,3]}); a = abc(1); b = abc(2); c = abc(3);
% A = [0.3*a + b + c, -1+2*a; 2*c, -5*b]; B = [1 + c, 2*a; 3*b, 4*a*b];
% C = A + B;
% d = C - [1+0.3*a+b+2*c, -1+4*a; 2*c+3*b, 4*a*b - 5*b];
% assert(max(max(d.coeffs.data)) < 1e-13,'multivariate B-spline + multivariate B-spline')
% 
% 
% %% BSpline SDPVAR + double
% 
% % univariate matrix-valued
% BP = BSplineBasis([0 0 0 1 1 1],2);
% P = BSpline.sdpvar(BP,[3 2]); Q = [1 2; 3 4; 5 6];
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 1]));
% assert(isequal(R.coeffs.shape,[3 2]));
% 
% % bivariate matrix-valued
% B1 = BSplineBasis([0 0 0 1 1 1],2); B2 = BSplineBasis([0 0 1 1],1);
% P = BSpline.sdpvar({B1,B2},[3 2]); Q = [1 2; 3 4; 5 6];
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 2]));
% assert(isequal(R.coeffs.shape,[3 2]));
% 
% % trivariate matrix-valued
% B1 = BSplineBasis([0 0 0 1 1 1],2); B2 = BSplineBasis([0 0 1 1],1);
% B3 = BSplineBasis([0 0 1 1],1);
% P = BSpline.sdpvar({B1,B2,B3},[3 2]); Q = [1 2; 3 4; 5 6];
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 2 2]));
% assert(isequal(R.coeffs.shape,[3 2]));
% 
% 
% %% BSpline SDPVAR + sdpvar
% 
% % univariate matrix-valued
% BP = BSplineBasis([0 0 0 1 1 1],2);
% P = BSpline.sdpvar(BP,[3 2]); Q = sdpvar(3,2);
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 1]));
% assert(isequal(R.coeffs.shape,[3 2]));
% % R = Q + P;
% % assert(isequal(R.coeffs.size,[3 1]));
% % assert(isequal(R.coeffs.shape,[3 2]));
% 
% % bivariate matrix-valued
% B1 = BSplineBasis([0 0 0 1 1 1],2); B2 = BSplineBasis([0 0 1 1],1);
% P = BSpline.sdpvar({B1,B2},[3 2]); Q = sdpvar(3,2);
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 2]));
% assert(isequal(R.coeffs.shape,[3 2]));
% % R = Q + P;
% % assert(isequal(R.coeffs.size,[3 2]));
% % assert(isequal(R.coeffs.shape,[3 2]));
% 
% % trivariate matrix-valued
% B1 = BSplineBasis([0 0 0 1 1 1],2); B2 = BSplineBasis([0 0 1 1],1);
% B3 = BSplineBasis([0 0 1 1],1);
% P = BSpline.sdpvar({B1,B2,B3},[3 2]); Q = sdpvar(3,2);
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 2 2]));
% assert(isequal(R.coeffs.shape,[3 2]));
% % R = Q + P;
% % assert(isequal(R.coeffs.size,[3 2 2]));
% % assert(isequal(R.coeffs.shape,[3 2]));
% 
% 
% %% BSpline SDPVAR + BSpline
% 
% % univariate matrix-valued
% BP = BSplineBasis([0 0 0 1 1 1],2); P = BSpline.sdpvar(BP,[3 2]); 
% a = parameter(1); a = a.to_bspline({[0,1]}); Q = [a, 3; 1, 2+a; 0, -a^2];
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 1]));
% assert(isequal(R.coeffs.shape,[3 2]));
% R = Q + P;
% assert(isequal(R.coeffs.size,[3 1]));
% assert(isequal(R.coeffs.shape,[3 2]));
% 
% % bivariate matrix-valued
% BP = {BSplineBasis([0 0 0 1 1 1],2),BSplineBasis([2 2 3 3],1)}; P = BSpline.sdpvar(BP,[3 2]); 
% a = parameter(2); a = a.to_bspline({[0,1],[2 3]}); Q = [a(1) + a(2), 3*a(2); 1, 2+a(1); 0, -a(2)^2];
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 3]));
% assert(isequal(R.coeffs.shape,[3 2]));
% R = Q + P;
% assert(isequal(R.coeffs.size,[3 3]));
% assert(isequal(R.coeffs.shape,[3 2]));
% 
% % trivariate matrix-valued
% BP = {BSplineBasis([0 0 0 1 1 1],2),BSplineBasis([2 2 3 3],1),BSplineBasis([4 4 5 5],1)}; P = BSpline.sdpvar(BP,[3 2]); 
% a = parameter(3); a = a.to_bspline({[0 1],[2 3],[4 5]}); Q = [a(1) + a(2), 3*a(2); a(3), 2+a(1); 0, 1-a(2)^2];
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 3 3]));
% assert(isequal(R.coeffs.shape,[3 2]));
% R = Q + P;
% assert(isequal(R.coeffs.size,[3 3 3]));
% assert(isequal(R.coeffs.shape,[3 2]));
% 
% 
% %% BSpline SDPVAR + BSpline SDPVAR
% 
% % univariate matrix-valued
% BP = BSplineBasis([0 0 0 1 1 1],2); BQ = BSplineBasis([0 0 1 1],1);
% P = BSpline.sdpvar(BP,[3 2]); Q = BSpline.sdpvar(BQ,[3 2]);
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 1]));
% assert(isequal(R.coeffs.shape,[3 2]));
% 
% % bivariate matrix-valued
% BP = {BSplineBasis([0 0 0 1 1 1],2),BSplineBasis([0 0 1 1],1)}; 
% BQ = {BSplineBasis([0 0 1 1],1),BSplineBasis([0 0 1 1],1)};
% P = BSpline.sdpvar(BP,[3 2]); Q = BSpline.sdpvar(BQ,[3 2]);
% R = P + Q;
% assert(isequal(R.coeffs.size,[3 2]));
% assert(isequal(R.coeffs.shape,[3 2]));
% 
% % trivariate matrix-valued
% BP = {BSplineBasis([0 0 0 0 1 1 1 1],3),BSplineBasis([0 0 0 1 1 1],2),BSplineBasis([0 0 1 1],1)}; 
% BQ = {BSplineBasis([0 0 1 1],1),BSplineBasis([0 0 1 1],1),BSplineBasis([0 0 1 1],1)};
% P = BSpline.sdpvar(BP,[3 2]); Q = BSpline.sdpvar(BQ,[3 2]);
% R = P + Q;
% assert(isequal(R.coeffs.size,[4 3 2]));
% assert(isequal(R.coeffs.shape,[3 2]));
% 
% %
% % test multiplication of B-splines
% %
% tol = 1e-12; % tolerance
% 
% %% Bspline * double
% 
% % univariate matrix-valued
% a = parameter(1); a = a.to_bspline({[0,6]});
% A = [a, 1; -3*a, 1+a]; B = [1 2; 3 4]; C = A*B;
% d = C - [3+a, 4+2*a; 3, 4-2*a];
% assert(max(max(abs(d.coeffs.data))) < tol,'univariate B-spline * double')
% C = B*A; d = C - [-5*a, 3+2*a; -9*a, 7+4*a];
% assert(max(max(abs(d.coeffs.data))) < tol,'double * univariate B-spline')
% 
% % bivariate matrix-valued 
% ab = parameter(2); ab = ab.to_bspline({[0,6],[1,2]}); a = ab(1); b = ab(2);
% A = [a, b; a+b, 1]; B = [1, 2; 3, 4]; C = A*B;
% d = C - [a+3*b, 2*a+4*b; a+b+3, 2*a+2*b+4];
% assert(max(max(abs(d.coeffs.data))) < tol,'bivariate B-spline * double')
% C = B*A; d = C - [3*a+2*b, b+2; 7*a+4*b, 3*b+4];
% assert(max(max(abs(d.coeffs.data))) < tol,'double * bivariate B-spline')
% 
% % trivariate matrix-valued 
% abc = parameter(3); abc = abc.to_bspline({[0,6],[1,2],[3,4]}); a = abc(1); b = abc(2); c = abc(3);
% A = [a, b; a+b, 1+c]; B = [1, 2; 3, 4]; C = A*B;
% d = C - [a+3*b, 2*a+4*b; a+b+3+3*c, 2*a+2*b+4+4*c];
% assert(max(max(abs(d.coeffs.data))) < tol,'trivariate B-spline * double')
% C = B*A; d = C - [3*a+2*b, b+2+2*c; 7*a+4*b, 3*b+4+4*c];
% assert(max(max(abs(d.coeffs.data))) < tol,'double * trivariate B-spline')
% 
% 
% %% Bspline * SDPVAR
% 
% % univariate matrix-valued
% a = parameter(1); a = a.to_bspline({[0,6]});
% A = [a, 1; -3*a, 1+a]; 
% % B = BSpline.sdpvar({BSplineBasis([0 6],0)},[2,2]);
% B = sdpvar(2,4);
% C = A*B; assert(isa(C,'BSpline'),'univariate B-spline * SDPVAR')
% % C = B*A; assert(isa(C,'BSpline'),'SDPVAR * univariate B-spline')
% 
% % bivariate matrix-valued
% a = parameter(2); a = a.to_bspline({[0,6],[1,2]});
% A = [a(1), -a(2); -3*a(1), 1+a(1)+a(2)]; 
% % B = BSpline.sdpvar({BSplineBasis([0 6],0)},[2,2]);
% B = sdpvar(2,4);
% C = A*B; assert(isa(C,'BSpline'),'bivariate B-spline * SDPVAR')
% % C = B*A; assert(isa(C,'BSpline'),'SDPVAR * univariate B-spline')
% 
% % trivariate matrix-valued
% a = parameter(3); a = a.to_bspline({[0,6],[1,2],[3,4]});
% A = [a(1), -a(2); -3*a(1), 1+a(1)+a(2); a(3)-a(1), a(3)*a(2)]; 
% B = sdpvar(2,4);
% C = A*B; assert(isa(C,'BSpline'),'trivariate B-spline * SDPVAR')
% % C = B*A; assert(isa(C,'BSpline'),'SDPVAR * univariate B-spline')
% 
% 
% %% Bspline * Bspline
% 
% % bivariate matrix-valued
% ab = parameter(2); ab = ab.to_bspline({[0,6],[1,2]}); a = ab(1); b = ab(2);
% A = [a, b; a+b, 1]; B = [1, a; b, a*b]; C = A*B;
% d = C - [a+b^2, a^2+a*b^2; a+2*b, a^2+2*a*b];
% assert(max(max(abs(d.coeffs.data))) < tol,'bivariate B-spline * bivariate B-spline')
% C = B*A; d = C - [a+a^2+a*b, a+b; a*b+a^2*b+a*b^2, a*b+b^2];
% assert(max(max(abs(d.coeffs.data))) < tol,'bivariate B-spline * bivariate B-spline')
% 
% % trivariate matrix-valued
% abc = parameter(3); abc = abc.to_bspline({[0,6],[1,2],[3,4]}); a = abc(1); b = abc(2); c = abc(3);
% A = [a, b; a+b, 1; 0, 3*c]; B = [1, a; b, a*b]; C = A*B;
% d = C - [a+b^2, a^2+a*b^2; a+2*b, a^2+2*a*b; 3*b*c, 3*a*b*c];
% assert(max(max(abs(d.coeffs.data))) < tol,'trivariate B-spline * bivariate B-spline')
% 
% 
% %% Bspline sdpvar * Bspline
% 
% % univariate matrix-valued
% a = parameter(1); a = a.to_bspline({[2 5]}); A = [1 + a + 2*a^2, 0; 3-a, 2];
% P = BSpline.sdpvar(BSplineBasis([2 2 2 5 5 5],2),[2 3]);
% Q = A*P;
% assert(isequal(Q.coeffs.size,[5 1]));
% assert(isequal(Q.coeffs.shape,[2 3]));
% P = BSpline.sdpvar(BSplineBasis([2 2 2 5 5 5],2),[4 2]);
% Q = P*A;
% assert(isequal(Q.coeffs.size,[5 1]));
% assert(isequal(Q.coeffs.shape,[4 2]));
% 
% % bivariate matrix-valued 
% ab = parameter(2); ab = ab.to_bspline({[2 5],[6 7]}); a = ab(1); b = ab(2);
% A = [1 + a + 2*b^2, 0; 3-a, 2*a*b];
% P = BSpline.sdpvar({BSplineBasis([2 2 2 5 5 5],2),BSplineBasis([6 6 7 7],1)},[2 3]);
% Q = A*P;
% assert(isequal(Q.coeffs.size,[5 4]));
% assert(isequal(Q.coeffs.shape,[2 3]));
% P = BSpline.sdpvar({BSplineBasis([2 2 2 5 5 5],2),BSplineBasis([6 6 7 7],1)},[4 2]);
% Q = P*A;
% assert(isequal(Q.coeffs.size,[5 4]));
% assert(isequal(Q.coeffs.shape,[4 2]));
% 
% % trivariate matrix-valued
% abc = parameter(3); abc = abc.to_bspline({[2 5],[6 7],[8,9]}); a = abc(1); b = abc(2); c = abc(3);
% A = [1 + a + 2*b^2, c^2; 3-a, 2*a*b];
% P = BSpline.sdpvar({BSplineBasis([2 2 2 5 5 5],2),BSplineBasis([6 6 7 7],1),BSplineBasis([8 8 9 9],1)},[2 3]);
% Q = A*P;
% assert(isequal(Q.coeffs.size,[5 4 4]));
% assert(isequal(Q.coeffs.shape,[2 3]));
% P = BSpline.sdpvar({BSplineBasis([2 2 2 5 5 5],2),BSplineBasis([6 6 7 7],1),BSplineBasis([8 8 9 9],1)},[4 2]);
% Q = P*A;
% assert(isequal(Q.coeffs.size,[5 4 4]));
% assert(isequal(Q.coeffs.shape,[4 2]));
% 


