#include <vector>
#include <algorithm>
#include "operationsBasis.h"
#include "../Basis.h"
#include "../SubBasis.h"
#include "../../Function/Argument.h"
#include "../../common.h"

namespace spline{

    Basis plusBasis(const Basis& b1,const  Basis& b2 ){
        Basis returnBasis = Basis();
        if(b1.hasArguments() && b2.hasArguments()){
            std::vector< Argument > allArguments = b1.getArguments();
            for(auto &a : b2.getArguments()){
                if(std::find(allArguments.begin(), allArguments.end(), a) == allArguments.end()) {
                    /* v does not contain x */
                    allArguments.push_back(a);
                }
            }

            for(auto &a : allArguments){
                SubBasis subBasis1 = b1.getSubBasis(a);
                SubBasis subBasis2 = b2.getSubBasis(a);
                std::cout << subBasis1.getRepresentation() << "   " << subBasis2.getRepresentation() << std::endl;
                SubBasis sum = subBasis1 + subBasis2;
                returnBasis.addBasis(sum);
            }

            returnBasis.setArguments(allArguments);

        }else{
            spline_assert_message(b1.getDimension() == b2.getDimension(),"Mismatch dimension");
            for(int i = 0; i < b1.getDimension(); i++){
                SubBasis subBasis1 = b1.getSubBasis()[i];
                SubBasis subBasis2 = b2.getSubBasis()[i];
                SubBasis sum = subBasis1 + subBasis2;
                returnBasis.addBasis(sum);
            }

            if(b1.hasArguments()){
                returnBasis.setArguments(b1.getArguments());
            }

            if(b2.hasArguments()){
                returnBasis.setArguments(b2.getArguments());
            }

        }
        return returnBasis;
    }

    Basis timesBasis(const Basis& b1,const  Basis& b2 ){
        Basis returnBasis = Basis();
        if(b1.hasArguments() && b2.hasArguments()){
            std::vector< Argument > allArguments = b1.getArguments();
            for(auto &a : b2.getArguments()){
                if(std::find(allArguments.begin(), allArguments.end(), a) == allArguments.end()) {
                    /* v does not contain x */
                    allArguments.push_back(a);
                }
            }

            for(auto &a : allArguments){
                SubBasis subBasis1 = b1.getSubBasis(a);
                SubBasis subBasis2 = b2.getSubBasis(a);
                std::cout << subBasis1.getRepresentation() << "   " << subBasis2.getRepresentation() << std::endl;
                SubBasis sum = subBasis1 * subBasis2;
                returnBasis.addBasis(sum);
            }

            returnBasis.setArguments(allArguments);

        }else{
            spline_assert_message(b1.getDimension() == b2.getDimension(),"Mismatch dimension");
            for(int i = 0; i < b1.getDimension(); i++){
                SubBasis subBasis1 = b1.getSubBasis()[i];
                SubBasis subBasis2 = b2.getSubBasis()[i];
                SubBasis sum = subBasis1 * subBasis2;
                returnBasis.addBasis(sum);
            }

            if(b1.hasArguments()){
                returnBasis.setArguments(b1.getArguments());
            }

            if(b2.hasArguments()){
                returnBasis.setArguments(b2.getArguments());
            }

        }
        return returnBasis;
    }
}
