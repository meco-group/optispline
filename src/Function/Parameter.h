#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <vector>
#include "Polynomial.h"

namespace spline{

class Parameter : public Polynomial {
public:
    Parameter(){};
    Parameter(const std::string& a);

    virtual std::string type() const override;
    virtual std::string to_string() const override;

    std::string name() const;
};
} // namespace spline
#endif /* PARAMETER_H */
