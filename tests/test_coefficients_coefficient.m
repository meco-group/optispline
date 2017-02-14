%meco_binaries('cpp_splines','fill_in_the_branch_you_want')
%
%import Basis.*
%
%rng(0);
%a_ = randi([-10 10],2,3);
%
%a = Coefficient(a_);
%t1 = randi([-10 10],4,2);
%t2 = randi([-10 10],3);
%
%a_t1 = a.transform(t1,1);
%a_t1_ = a_t1.getData();
%a_t2 = a.transform(t2,2);
%a_t2_ = a_t2.getData();
%
%assert(isequal(a_t1_,t1*a_));
%assert(isequal(a_t2_,a*t2'));
%
%neg_test = false;
%try a.transform(t1,2);
%catch neg_test = true;
%end
%assert(neg_test)
%
%neg_test = false;
%try a.transform(t1',2);
%catch neg_test = true;
%end
%assert(neg_test)
%
%neg_test = false;
%try a.transform(t2,1);
%catch neg_test = true;
%end
%assert(neg_test)
