#include "EvaluationGrid.h"
#include "../Basis.h"
#include "../../common.h"

namespace spline{
    EvaluationGrid::EvaluationGrid(TensorBasis basis) : griddedBasis(basis) {}

    std::vector< AnyTensor > EvaluationGrid::evaluateEvaluationGrid() const {
        std::vector< AnyTensor > preStep { AnyTensor::unity() };
        std::vector< AnyTensor > postStep ;

        for(Basis b : griddedBasis.getSubBasis()){
            std::vector< std::vector< AnyScalar > > evaluationGrid;
            b.getEvaluationGrid(&evaluationGrid);
            for(auto const & subPoint : evaluationGrid){
                AnyTensor subEvaluation = b(subPoint);
                for( AnyTensor pre : preStep){
                    postStep.push_back(pre.outer_product( subEvaluation ));
                }
            }
            preStep = postStep;
            postStep.clear();
        }
        return preStep;
    }

        TensorBasis basis = f.getTensorBasis();
        std::vector< int > indexPermutation = getPermutation(basis);

        std::vector< AnyTensor > preStep { AnyTensor::unity() };
        std::vector< AnyTensor > postStep ;

        for(int i = 0; i < griddedBasis.getDimension(); i++){
            Basis subBasis = griddedBasis.getSubBasis()[i];
            std::vector< std::vector< AnyScalar > > evaluationGrid;
            subBasis.getEvaluationGrid(&evaluationGrid);
            for(auto const & subPoint : evaluationGrid){
                if(indexPermutation[i] < 0){
                    for( AnyTensor pre : preStep){
                        postStep.push_back(pre);
                    }
                }else{
                    Basis correspondingSubBasis = basis.getSubBasis()[indexPermutation[i]];
                    AnyTensor subEvaluation = correspondingSubBasis(subPoint);
                    for( AnyTensor pre : preStep){
                        postStep.push_back(pre.outer_product( subEvaluation ));
                    }
                }
            }
            preStep = postStep;
            postStep.clear();
        }

        for( AnyTensor pre : preStep){
            postStep.push_back(pre.inner(f.getCoefficient().getData()));
        }

        return postStep;
    }

    std::vector< int > EvaluationGrid::getPermutation(TensorBasis basis) const{
        std::vector< int > index;
        if(griddedBasis.hasArguments() && basis.hasArguments()){
            for(auto & a : griddedBasis.getArguments()){
                index.push_back(basis.indexArgument(a));
            }
        }else{
            spline_assert(griddedBasis.getDimension() == basis.getDimension());
            for(int i = 0; i < griddedBasis.getDimension(); i++){
                index.push_back(i);
            }
        }

        return index;
    }

    std::string EvaluationGrid::getRepresentation() const {
        std::stringstream s;
        // s << "EvaluationGrid" << std::endl;
        s << "EvaluationGrid";
        // for(Basis b : griddedBasis.getSubBasis()){
        //     std::vector< std::vector< AnyScalar > > evalutionGrid = b.getEvaluationGrid();
        //     for(auto const & subPoint : evalutionGrid){
        //         for(auto const & sP : subPoint){
        //             std::cout << sP.is_double() << std::endl;
        //         s << std::to_string(sP.is_double());
        //         s << " ";
        //
        //         }
        //     }
        //         s << "|";
        // }
        return s.str();
    };

}
