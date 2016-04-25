// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#include "Basis.h"

namespace spline {
    void Basis::setArguments (const std::vector <std::string> arguments) {
        Basis::arguments = arguments;
    }

    std::vector <std::string> Basis::getArguments () const {
        return arguments;
    }

    int Basis::getDimensions () const {
        return dimensions;
    }

    void Basis::setDimensions (int dimensions) {
        Basis::dimensions = dimensions;
    }
} // namespace spline