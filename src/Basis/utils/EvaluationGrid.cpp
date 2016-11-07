#include "EvaluationGrid.h"
#include "../Basis.h"
#include "../../common.h"

namespace spline{
    EvaluationGridNode* EvaluationGrid::get() const { return static_cast<EvaluationGridNode*>(SharedObject::get()); };
    EvaluationGridNode* EvaluationGrid::operator->() const { return get(); }

    // EvaluationGrid::EvaluationGrid (){
    //     assign_node(new EvaluationGridNode(TensorBasis()));
    // }
    //
    EvaluationGrid::EvaluationGrid (TensorBasis basis){
        assign_node(new EvaluationGridNode(basis));
    }

    EvaluationGridNode::EvaluationGridNode(TensorBasis basis) : griddedBasis(basis) {}

    std::vector< AnyTensor > EvaluationGrid::evaluateEvaluationGrid() const {return (*this)->evaluateEvaluationGrid();}
    std::vector< AnyTensor > EvaluationGridNode::evaluateEvaluationGrid() const {
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

    std::vector< AnyTensor > EvaluationGrid::evaluateEvaluationGrid(const Function & f) const {return (*this)->evaluateEvaluationGrid(f);}
    std::vector< AnyTensor > EvaluationGridNode::evaluateEvaluationGrid(const Function & f) const {
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

    std::string EvaluationGridNode::getRepresentation() const {
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

    std::string EvaluationGrid::getRepresentation() const { return (*this)->getRepresentation() ;};
}
