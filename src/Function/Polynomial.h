#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string>
#include <vector>
#include "Function.h"

namespace spline{

class Polynomial : public Function {
public:
    Polynomial(){};
    Polynomial(const std::vector< double >& coef);
    Polynomial(const std::vector< double >& coef, std::string& a);
    Polynomial(const std::vector< double >& coef, const std::string &name);

    virtual std::string type() const override;
    virtual std::string to_string() const override;
};
} // namespace spline
#endif /* POLYNOMIAL_H */
