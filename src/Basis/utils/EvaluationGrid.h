#ifndef EVALUATIONGRID_H
#define EVALUATIONGRID_H

#include <vector>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "../Function/Argument.h"
#include "../Basis.h"
namespace spline{

#ifndef SWIG

    class EvaluationGridNode : public SharedObjectNode {
        public:


        EvaluationGridNode(Basis basis);
        // EvaluationGridNode() : listedGrid(std::vector< std::vector< std::vector< AnyScalar {}),
                               // argumentList(std::vector< Argument > {}) {}
        // void append( std::vector< std::vector< double > > subGrid );
        // void append( std::vector< std::vector< double > > subGrid, Argument argument);
        // void setArguments( std::vector< Argument > argumentList );
        std::vector< AnyTensor > evaluateEvaluationGrid() const;
        std::vector< AnyTensor > evaluateEvaluationGrid(Function f) const;

    private:
        Basis griddedBasis;
    };

#endif // SWIG

    class EvaluationGrid : public SharedObject {
    public:

        EvaluationGrid(Basis basis);
#ifndef SWIG

        EvaluationGridNode* get() const ;
        EvaluationGridNode* operator->() const ;

#endif // SWIG
        // void append( std::vector< std::vector< double > > subGrid );
        // void append( std::vector< std::vector< double > > subGrid, Argument argument);
        //
        // void setArguments( std::vector< Argument > argumentList );
        std::vector< AnyTensor > evaluateEvaluationGrid() const;
        std::vector< AnyTensor > evaluateEvaluationGrid(Function f) const;
    };
} // namespace spline

#endif
