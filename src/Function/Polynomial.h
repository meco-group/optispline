#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string>
#include <vector>
#include "Function.h"

namespace spline{

class Polynomial : public Function {
public:
    Polynomial(const std::vector< double >& coef);
    Polynomial(const std::vector< double >& coef, Argument a);
    Polynomial(const std::vector< double >& coef, const std::string &name);
    // virtual std::string getRepresentation() const ;
};
} // namespace spline
#endif /* POLYNOMIAL_H */
