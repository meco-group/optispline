#include "EvaluationGrid.h"

namespace spline{
    EvaluationGridNode* EvaluationGrid::get() const { return static_cast<EvaluationGridNode*>(SharedObject::get()); };
    EvaluationGridNode* EvaluationGrid::operator->() const { return get(); }

    EvaluationGrid::EvaluationGrid (){
        // assign_node(new EvaluationGridNode(std::vector< SubEvaluationGrid >{}));
        assign_node(new EvaluationGridNode());
    }

        void EvaluationGrid::append( std::vector< std::vector< double > > subGrid ){(*this)->append(subGrid);}
        void EvaluationGridNode::append( std::vector< std::vector< double > > subGrid ){
            listedGrid.push_back(subGrid);
        }

        void EvaluationGrid::append( std::vector< std::vector< double > > subGrid, Argument argument){(*this)->append(subGrid);}
        void EvaluationGridNode::append( std::vector< std::vector< double > > subGrid, Argument argument){
            listedGrid.push_back(subGrid);
            argumentList.push_back(argument);
        }

        void EvaluationGrid::setArguments( std::vector< Argument > argumentList ){(*this)->setArguments(argumentList);}
        void EvaluationGridNode::setArguments( std::vector< Argument > argList ){
            argumentList = argList;
        }
}
