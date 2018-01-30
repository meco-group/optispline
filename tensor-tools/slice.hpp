#ifndef ANYSLICE_HPP_INCLUDE
#define ANYSLICE_HPP_INCLUDE

#include <casadi/casadi.hpp>
#include <vector>

  class AnySlice {
  public:
    casadi_int start_;
    casadi_int stop_;
    casadi_int step_;

    std::vector<casadi_int> indices_;

    bool has_indices_;

    /// Default constructor - all elements
    AnySlice();
    AnySlice(casadi_int i);
    AnySlice(casadi_int start, casadi_int stop, casadi_int step=1);
    AnySlice(const std::vector<casadi_int>& indices);
    std::vector<casadi_int> indices(casadi_int len) const;

  };


#endif
