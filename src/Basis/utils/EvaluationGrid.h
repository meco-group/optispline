#ifndef EVALUATIONGRID_H
#define EVALUATIONGRID_H

#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "../Function/Argument.h"
namespace spline{

#ifndef SWIG

    class EvaluationGridNode : public SharedObjectNode {
        public:


        EvaluationGridNode() : listedGrid(std::vector< std::vector< std::vector< double > > > {}),
                               argumentList(std::vector< Argument > {}) {}
        void append( std::vector< std::vector< double > > subGrid );
        void append( std::vector< std::vector< double > > subGrid, Argument argument);
        void setArguments( std::vector< Argument > argumentList );

    private:

        std::vector< std::vector< std::vector< double > > > listedGrid;
        std::vector< Argument > argumentList;

    };

#endif // SWIG

    class EvaluationGrid : public SharedObject {
    public:

        EvaluationGrid();
#ifndef SWIG

        EvaluationGridNode* get() const ;
        EvaluationGridNode* operator->() const ;

#endif // SWIG
        void append( std::vector< std::vector< double > > subGrid );
        void append( std::vector< std::vector< double > > subGrid, Argument argument);

        void setArguments( std::vector< Argument > argumentList );
    };
} // namespace spline

#endif
