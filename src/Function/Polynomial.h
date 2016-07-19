#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string>
#include <vector>
#include "Function.h"

namespace spline{

class Polynomial : public Function {
public:
    Polynomial(const std::vector< double >& coef);
    // virtual std::string getRepresentation() const ;
    Polynomial(const std::vector< double >& coef, const Argument argument);
    Polynomial(const std::vector< double >& coef, const std::string& argumentName);
};
} // namespace spline
#endif /* POLYNOMIAL_H */
