#include "EvaluationGrid.h"
#include "../SubBasis.h"
#include "../../common.h"

namespace spline{
    EvaluationGridNode* EvaluationGrid::get() const { return static_cast<EvaluationGridNode*>(SharedObject::get()); };
    EvaluationGridNode* EvaluationGrid::operator->() const { return get(); }

    // EvaluationGrid::EvaluationGrid (){
    //     assign_node(new EvaluationGridNode(Basis()));
    // }
    //
    EvaluationGrid::EvaluationGrid (Basis basis){
        assign_node(new EvaluationGridNode(basis));
    }

    EvaluationGridNode::EvaluationGridNode(Basis basis) : griddedBasis(basis) {}

    void EvaluationGrid::evaluateEvaluationGrid(std::vector< AnyTensor > * returnVector) const {return (*this)->evaluateEvaluationGrid(returnVector);}
    void EvaluationGridNode::evaluateEvaluationGrid(std::vector< AnyTensor > * returnVector) const {
        std::vector< AnyTensor > preStep { AnyTensor::unity() };
        std::vector< AnyTensor > postStep { AnyTensor::unity() };

        for(SubBasis b : griddedBasis.getSubBasis()){
            std::vector< std::vector< AnyScalar > > evaluationGrid;
            b.getEvaluationGrid(&evaluationGrid);
            for(auto const & subPoint : evaluationGrid){
                AnyTensor subEvaluation = b(subPoint);
                for( AnyTensor pre : preStep){
                    postStep.push_back(pre.outer_product( subEvaluation ));
                }
            }
            preStep = postStep;
        }
        *returnVector = preStep;
    }


    void EvaluationGrid::evaluateEvaluationGrid(std::vector< AnyTensor > * returnVector, Function f) const {return (*this)->evaluateEvaluationGrid(returnVector, f);}
    void EvaluationGridNode::evaluateEvaluationGrid(std::vector< AnyTensor > * returnVector, Function f) const {
        Basis basis = f.getBasis();
        std::vector< int > indexPermutation;
        getPermutation(&indexPermutation, basis);

        std::vector< AnyTensor > preStep { AnyTensor::unity() };
        std::vector< AnyTensor > postStep { AnyTensor::unity() };

        for(int i = 0; i < basis.getDimension(); i++){
            SubBasis subBasis = basis.getSubBasis()[i];
            SubBasis correspondingSubBasis = griddedBasis.getSubBasis()[indexPermutation[i]];
            std::vector< std::vector< AnyScalar > > evaluationGrid;
            correspondingSubBasis.getEvaluationGrid(&evaluationGrid);
            for(auto const & subPoint : evaluationGrid){
                AnyTensor subEvaluation = subBasis(subPoint);
                for( AnyTensor pre : preStep){
                    postStep.push_back(pre.outer_product( subEvaluation ));
                }
            }
            preStep = postStep;
        }

        postStep.clear();
        for( AnyTensor pre : preStep){
            postStep.push_back(pre.inner(f.getCoefficient().getData()));
        }

        *returnVector = postStep;
    }

    void EvaluationGridNode::getPermutation(std::vector< int > * indexPermutation, Basis basis) const{
        std::vector< int > index;
        if(griddedBasis.hasArguments() && basis.hasArguments()){
            for(auto & a : basis.getArguments()){
                index.push_back(griddedBasis.indexArgument(a));
            }
        }else{
            spline_assert(griddedBasis.getDimension() == basis.getDimension());
            for(int i = 0; i < basis.getDimension(); i++){
                index.push_back(i);
            }
        }
        *indexPermutation = index;
    }

    // void EvaluationGridNode::evaluateEvaluationGridWithArguments(std::vector< AnyTensor > returnVector, Function f) const {
    //     std::vector< AnyTensor > preStep { AnyTensor::unity() };
    //     std::vector< AnyTensor > postStep { AnyTensor::unity() };
    //
    //     Basis basis = f.getBasis();
    //
    //     // for(SubBasis b : griddedBasis.getSubBasis()){
    //     for(int i = 0; i < griddedBasis.getDimension(); i++){
    //
    //         std::vector< std::vector< AnyScalar > > evaluationGrid;
    //         b.getEvaluationGrid(&evaluationGrid);
    //         for(auto const & subPoint : evaluationGrid){
    //             AnyTensor subEvaluation = b(subPoint);
    //             for( AnyTensor pre : preStep){
    //                 postStep.push_back(pre.outer_product( subEvaluation ));
    //             }
    //         }
    //         preStep = postStep;
    //     }
    //     for( AnyTensor pre : preStep){
    //         postStep.push_back(pre.outer_product( subEvaluation ));
    //     }
    //     returnVector = preStep;
    //
    //
    // }
    //
    // void EvaluationGridNode::evaluateEvaluationGridWithNoArguments(std::vector< AnyTensor > returnVector, Function f) const {
    //     Basis basis = f.getBasis();
    //     spline_assert(griddedBasis.getDimension() == basis.getDimension());
    //
    //     std::vector< AnyTensor > preStep { AnyTensor::unity() };
    //     std::vector< AnyTensor > postStep { AnyTensor::unity() };
    //     // for(SubBasis b : griddedBasis.getSubBasis()){
    //     for(int i = 0; i < griddedBasis.getDimension(); i++){
    //
    //
    //
    //
    //         std::vector< std::vector< AnyScalar > > evaluationGrid;
    //         b.getEvaluationGrid(&evaluationGrid);
    //         for(auto const & subPoint : evaluationGrid){
    //             AnyTensor subEvaluation = b(subPoint);
    //             for( AnyTensor pre : preStep){
    //                 postStep.push_back(pre.outer_product( subEvaluation ));
    //             }
    //         }
    //         preStep = postStep;
    //     }
    //     for( AnyTensor pre : preStep){
    //         postStep.push_back(pre.outer_product( subEvaluation ));
    //     }
    //     returnVector = preStep;
    //
    //
    // }
    //
    std::string EvaluationGridNode::getRepresentation() const {
        std::stringstream s;
        // s << "EvaluationGrid" << std::endl;
        s << "EvaluationGrid";
        // for(SubBasis b : griddedBasis.getSubBasis()){
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
    // std::ostream& operator<<(std::ostream &stream, const EvaluationGrid& eg){
    //     return stream << eg.getRepresentation();
    // }
    }
