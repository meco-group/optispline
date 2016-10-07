#include <vector>
#include "operationsBasis.h"
#include "../Basis.h"
#include "../SubBasis.h"
#include "../../common.h"	

namespace spline{
       Basis plusBasis(const Basis& b1,const  Basis& b2 ){
        Basis returnBasis = Basis();
        if(b1.hasArguments() && b2.hasArguments()){
            // std::vector<Argument> 
        }else{
            spline_assert_message(b1.getDimension() == b2.getDimension(),"Mismatch dimension");
            for(int i = 0; i < b1.getDimension(); i++){
                SubBasis subBasis1 = b1.getSubBasis()[i];
                SubBasis subBasis2 = b2.getSubBasis()[i];
                SubBasis sum = subBasis1 + subBasis2;
                returnBasis.addBasis(sum);
            }
        }
        return returnBasis;
    }
}
