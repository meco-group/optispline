#ifndef OPERATIONSBASISG
#define OPERATIONSBASISG

#include <functional>

#include "plus.h"
#include "times.h"
#include "../Basis.h"
#include "../../Common.h"	

namespace spline{

    template< class Basis1, class Basis2 >
        Basis genericOperation()(const Basis1& b1,const  Basis2& b2, const std::function<Basis(const Basis1&, const Basis2&)>& bfun ){
            if(b1.hasArgument() && b2.hasArgument()){
            }else{
                spline_assert(b1.getDimension() != b2.getDimension(),"Mismatch dimension");
                Basis() returnBasis;
                for(int i = 0; i < b1.getDimention(); i++){
                }
            }

            template< class Basis1, class Basis2 >
                Basis plusBasis(const Basis1& b1,const  Basis2& b2){
                    auto plusBasisOp = [](const Basis1& a, const Basis2& b) -> Basis { return plusSubBasis(a,b); };
                    genericOperation()<Basis1, Basis2>(b1, b2, plusBasisOp);
                }

            template< class Basis1, class Basis2 >
                Basis timesBasis(const Basis1& b1,const  Basis2& b2){
                    auto timesBasisOp = [](const Basis1& a, const Basis2& b) -> Basis { return timesSubBasis(a,b); };
                    genericOperation()<Basis1, Basis2>(b1, b2, timesBasisOp);
                }

        }
#endif //  OPERATIONSBASISG

