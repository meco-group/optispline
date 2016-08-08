#include "plusMultivariate.h"

namespace spline{


        Basis plusMultivariate(const Basis& b1, const MonomialeBasis& b2){
        // Basis plusMultivariateUnivariate(const Basis& b1, BasisT b2){
            Basis returnBasis = Basis();
            bool matchingArguments = false;

            // for (int i = 0; i < b1.getSubBasis().size(); ++i) {
            // auto subBasis = b1.getSubBasis()[i];  
            for (Basis & subBasis : b1.getSubBasis()) {
                if(subBasis.getArgument() == b2.getArgument()){
                    Basis sum = b2 + subBasis;
                    // Basis sum = subBasis->operator+(b2);
                    // sum.setArgument(subBasis.getArgument());
                    // returnBasis.addBasis(sum);
                    // returnBasis.addBasis(b2);
                    matchingArguments = true;
                }else{
                    returnBasis.addBasis(subBasis);
                }
            }

            if(! matchingArguments){
                returnBasis.addBasis(b1);
            }
            return returnBasis;
        }

        Basis plusMultivariate(const Basis& b1, const Basis& b2){
            Basis returnBasis = b1;

            for (Basis & subBasis : b2.getSubBasis()) {
                // returnBasis = returnBasis + subBasis;
            }

            return returnBasis;
        }
}  // namespace spline
