#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <vector>
#include "Polynomial.h"

namespace spline{

class Parameter : public Polynomial {
public:
    Parameter(const std::string& a);

    virtual std::string type() const override;
    virtual std::string to_string() const override;
};
} // namespace spline
#endif /* PARAMETER_H */
