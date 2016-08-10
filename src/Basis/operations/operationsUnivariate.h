#ifndef OPERATIONUNIVARIAT
#define OPERATIONUNIVARIAT

#include "../Basis.h"
#include "timesUnivariate.h"
#include "plusUnivariate.h"

namespace spline{
// #ifndef SWIG
    
    template< class Basis1, class Basis2 >
        Basis plusUnivariateUnivariate(const Basis1& b1,const  Basis2& b2){
            if(b1.getArgument() == b2.getArgument()){
                Basis univariat = plusBasis(b1, b2);
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
        Basis timesUnivariateUnivariate(const Basis1& b1,const  Basis2& b2){
            if(b1.getArgument() == b2.getArgument()){
                Basis univariat = timesBasis(b1, b2);
                univariat.setArgument(b1.getArgument());
                return univariat;
            }else{
                Basis tensor = Basis();
                tensor.addBasis(b1);
                tensor.addBasis(b2);
                return tensor;
            }
        }

// #endif // SWIG
}// namespace spline
#endif // OPERATIONUNIVARIAT
