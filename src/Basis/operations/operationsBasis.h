#ifndef OPERATIONSBASISG
#define OPERATIONSBASISG

// #include <functional>

#include "plus.h"
#include "times.h"
// #include "../Basis.h"
// #include "../SubBasis.h"
// #include "../../common.h"

namespace spline{
    class Basis;
    //
    // Basis plusBasis(const Basis& b1,const  Basis& b2 );
    //
       Basis plusBasis(const Basis& b1,const  Basis& b2 );
    //
    //
    //
    //
    // 
    // template< class Basis1, class Basis2 >
    //     Basis genericOperation(const Basis1& b1,const  Basis2& b2, const std::function<Basis(const Basis1&, const Basis2&)>& bfun ){
    //         Basis returnBasis = Basis();
    //         if(b1.hasArgument() && b2.hasArgument()){
    //             // std::vector<Argument> 
    //         }else{
    //             spline_assert_message(b1.getDimension() != b2.getDimension(),"Mismatch dimension");
    //             for(int i = 0; i < b1.getDimention(); i++){
    //                 SubBasis subBasis1 = b1.getSubBasis()[i];
    //                 SubBasis subBasis2 = b2.getSubBasis()[i];
    //                 returnBasis.addBasis(bfun(subBasis1, subBasis2));
    //             }
    //         }
    //         return returnBasis;
    //     }
    //
    // template< class Basis1, class Basis2 >
    //     Basis plusBasis(const Basis1& b1,const  Basis2& b2){
    //         auto plusBasisOp = [](const Basis1& a, const Basis2& b) -> Basis { return plusSubBasis(a,b); };
    //         genericOperation<Basis1, Basis2>(b1, b2, plusBasisOp);
    //     }
    //
    // template< class Basis1, class Basis2 >
    //     Basis timesBasis(const Basis1& b1,const  Basis2& b2){
    //         auto timesBasisOp = [](const Basis1& a, const Basis2& b) -> Basis { return timesSubBasis(a,b); };
    //         genericOperation<Basis1, Basis2>(b1, b2, timesBasisOp);
    //     }
    //

}
#endif //  OPERATIONSBASISG

