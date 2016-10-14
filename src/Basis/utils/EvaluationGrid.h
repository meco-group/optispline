#ifndef EVALUATIONGRID_H
#define EVALUATIONGRID_H

#include <vector>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "../Function/Argument.h"
#include "../Function/Function.h"
#include "../Basis.h"
namespace spline{

#ifndef SWIG

    class EvaluationGridNode : public SharedObjectNode {
    public:
        std::string getRepresentation() const ;

        EvaluationGridNode(Basis basis);
        std::vector< AnyTensor > evaluateEvaluationGrid() const;
        std::vector< AnyTensor > evaluateEvaluationGrid(const Function & f) const;

        void getPermutation(std::vector< int > * indexPermutation, Basis basis) const;
    private:
        Basis griddedBasis;
    };

#endif // SWIG

    class EvaluationGrid : public SharedObject {
    public:
        std::string getRepresentation() const ;
        // EvaluationGrid();
        EvaluationGrid(Basis basis);

#ifndef SWIG

        EvaluationGridNode* get() const ;
        EvaluationGridNode* operator->() const ;

        inline friend
            std::ostream& operator<<(std::ostream &stream, const EvaluationGrid& obj) {
                return stream << obj.getRepresentation();
            }
        std::vector< AnyTensor > evaluateEvaluationGrid() const;
        std::vector< AnyTensor > evaluateEvaluationGrid(const Function & f) const;
#endif // SWIG
    };
} // namespace spline

#endif
