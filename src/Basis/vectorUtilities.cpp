//
// Created by erik on 19/04/16.
//

#include <vector>
#include "vectorUtilities.h"


namespace spline{


    std::vector<double> unionKnots (const std::vector<double> kn1, const std::vector<double> kn2, int degree) {
        std::vector<double> knots;
        knots.resize(kn1.size() + kn2.size());
        int index = 0;
        int index1 = 0;
        int index2 = 0;
        double value;
        int repeat = 0;



        while(index1 < kn1.size() || index2 < kn2.size() ){
            if (kn1[index1] < kn2[index2]){
                value = kn1[index1];
                index1++;
                if (value == kn1[kn1.back()]){
                    index1 == kn1.size();
                }

            } else {
                value = kn1[index2];
                index2++;
                if (value == kn2[kn2.back()]){
                    index2 == kn2.size();
                }
            }

//            TODO
            if( index == 0 ){
                for (int i = 0; i < degree + 1; ++i) {
                    knots[index] = value;
                    index++;
                }
            } else if( value != knots[index - 1]) {
                knots[index] = value;
                index++;
            }
        }

        for (int i = 0; i < degree; ++i) {
            knots[index] = value;
            index++;
        }

        knots.resize(index);

        return knots;
    }

}  // namespace

