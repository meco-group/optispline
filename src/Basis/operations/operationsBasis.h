#ifndef OPERATIONSBASISG
#define OPERATIONSBASISG

// #include <functional>

#include "plus.h"
#include "times.h"
// #include "../TensorBasis.h"
// #include "../Basis.h"
// #include "../../common.h"

namespace spline{
    class TensorBasis;
    //
    // TensorBasis plusBasis(const TensorBasis& b1,const  TensorBasis& b2 );
    //
       TensorBasis plusBasis(const TensorBasis& b1,const  TensorBasis& b2 );
       TensorBasis timesBasis(const TensorBasis& b1,const  TensorBasis& b2 );
    //
    //
    //
    //
    //
    // template< class Basis1, class Basis2 >
    //     TensorBasis genericOperation(const Basis1& b1,const  Basis2& b2, const std::function<TensorBasis(const Basis1&, const Basis2&)>& bfun ){
    //         TensorBasis returnBasis = TensorBasis();
    //         if(b1.hasArgument() && b2.hasArgument()){
    //             // std::vector<Argument>
    //         }else{
    //             spline_assert_message(b1.getNumberOfSubBasis() != b2.getNumberOfSubBasis(),"Mismatch dimension");
    //             for(int i = 0; i < b1.getDimention(); i++){
    //                 Basis subBasis1 = b1.getSubBasis()[i];
    //                 Basis subBasis2 = b2.getSubBasis()[i];
    //                 returnBasis.addBasis(bfun(subBasis1, subBasis2));
    //             }
    //         }
    //         return returnBasis;
    //     }
    //
    // template< class Basis1, class Basis2 >
    //     TensorBasis plusBasis(const Basis1& b1,const  Basis2& b2){
    //         auto plusBasisOp = [](const Basis1& a, const Basis2& b) -> TensorBasis { return plusSubBasis(a,b); };
    //         genericOperation<Basis1, Basis2>(b1, b2, plusBasisOp);
    //     }
    //
    // template< class Basis1, class Basis2 >
    //     TensorBasis timesBasis(const Basis1& b1,const  Basis2& b2){
    //         auto timesBasisOp = [](const Basis1& a, const Basis2& b) -> TensorBasis { return timesSubBasis(a,b); };
    //         genericOperation<Basis1, Basis2>(b1, b2, timesBasisOp);
    //     }
    //

}
#endif //  OPERATIONSBASISG

