#include "EvaluationGrid.h"
#include "../SubBasis.h"

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

    EvaluationGridNode::EvaluationGridNode(Basis basis) : griddedBasis(basis) {
       // griddedBasis = basis;
       // for(SubBasis b : basis.getSubBasis()){
       //     listedGrid.push_back( b.getEvaluationGrid());
       //  }
       // std::cout << listedGrid << std::endl;
        // argumentList = basis.getArguments();
    }

    // void EvaluationGrid::append( std::vector< std::vector< double > > subGrid ){(*this)->append(subGrid);}
    // void EvaluationGridNode::append( std::vector< std::vector< double > > subGrid ){
    //     listedGrid.push_back(subGrid);
    // }
    //
    // void EvaluationGrid::append( std::vector< std::vector< double > > subGrid, Argument argument){(*this)->append(subGrid);}
    // void EvaluationGridNode::append( std::vector< std::vector< double > > subGrid, Argument argument){
    //     listedGrid.push_back(subGrid);
    //     argumentList.push_back(argument);
    // }
    //
    // void EvaluationGrid::setArguments( std::vector< Argument > argumentList ){(*this)->setArguments(argumentList);}
    // void EvaluationGridNode::setArguments( std::vector< Argument > argList ){
    //     argumentList = argList;
    // }
    //

    std::vector< AnyTensor > EvaluationGrid::evaluateEvaluationGrid() const {return (*this)->evaluateEvaluationGrid();}
    std::vector< AnyTensor > EvaluationGridNode::evaluateEvaluationGrid() const {
        std::vector< AnyTensor > preStep { AnyTensor::unity() };
        std::vector< AnyTensor > postStep { AnyTensor::unity() };

        for(SubBasis b : griddedBasis.getSubBasis()){
            std::vector< std::vector< AnyScalar > > evalutionGrid = b.getEvaluationGrid();
            for(auto const & subPoint : evalutionGrid){
                AnyTensor subEvaluation = b(subPoint);
                for( AnyTensor pre : preStep){
                    postStep.push_back(pre.outer_product( subEvaluation ));
                }
            }
            preStep = postStep;
        }
        return preStep;
    }


    std::vector< AnyTensor > EvaluationGrid::evaluateEvaluationGrid(Function f) const {return (*this)->evaluateEvaluationGrid(f);}
    std::vector< AnyTensor > EvaluationGridNode::evaluateEvaluationGrid(Function f) const {



    }

    // EvaluationGrid Basis::getEvaluationGrid() const {(*this)->getEvaluationGrid();}
    // EvaluationGrid BasisNode::getEvaluationGrid() const {
    //     EvaluationGrid evaluationGrid = EvaluationGrid();
    //
    //
    // }
}
