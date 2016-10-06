#ifndef OPERATIONUNIVARIAT
#define OPERATIONUNIVARIAT

#include "../Basis.h"
#include "timesUnivariate.h"
#include "plusUnivariate.h"
#include <functional>

namespace spline{
// #ifndef SWIG

    template< class Basis1, class Basis2 >
        Basis genericUnivariateUnivariate(const Basis1& b1,const  Basis2& b2, const std::function<Basis(const Basis1&, const Basis2&)>& bfun ){
            if(b1.getArgument() == b2.getArgument()){
                Basis univariat = bfun(b1, b2);
                univariat.setArgument(b1.getArgument());
                return univariat;
            }else{
                Basis tensor = Basis();
                tensor.addBasis(b1);
                tensor.addBasis(b2);
                return tensor;
            }
        }
            
    template< class Basis1, class Basis2 >
        Basis plusUnivariateUnivariate(const Basis1& b1,const  Basis2& b2){
            auto plusBasisOp = [](const Basis1& a, const Basis2& b) -> Basis { return plusBasis(a,b); };
            genericUnivariateUnivariate<Basis1, Basis2>(b1, b2, plusBasisOp);
        }
    
    template< class Basis1, class Basis2 >
        Basis timesUnivariateUnivariate(const Basis1& b1,const  Basis2& b2){
            auto timesBasisOp = [](const Basis1& a, const Basis2& b) -> Basis { return timesBasis(a,b); };
            genericUnivariateUnivariate<Basis1, Basis2>(b1, b2, timesBasisOp);
        }

// #endif // SWIG
}// namespace spline

#endif // OPERATIONUNIVARIAT
