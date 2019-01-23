#ifndef SLICE_HPP_INCLUDE
#define SLICE_HPP_INCLUDE

#include "slice.hpp"
#include <limits>
#include <casadi/casadi.hpp>
#include <iostream>
/// Default constructor - all elements
AnySlice::AnySlice() : start_(0), stop_(std::numeric_limits<casadi_int>::max()), step_(1),
  has_indices_(false) { };
AnySlice::AnySlice(casadi_int i) : start_(i), stop_(i+1), step_(1), has_indices_(false) {};
AnySlice::AnySlice(casadi_int start, casadi_int stop, casadi_int step) : start_(start), stop_(stop), step_(step),
  has_indices_(false){};
AnySlice::AnySlice(const std::vector<casadi_int>& indices) : indices_(indices), has_indices_(true) {};


std::vector<casadi_int> AnySlice::indices(casadi_int len) const {
  if (has_indices_) return indices_;

  casadi_int start = start_;
  casadi_int stop = stop_;
  casadi_int step = step_;

  if (start==std::numeric_limits<int>::min()) {
    start = (step < 0) ? len - 1 : 0;
  } else if (start<0) {
    start+=len;
  }
  if (stop==std::numeric_limits<int>::max()) {
    stop = (step < 0) ? -1 : len;
  } else if (stop<0) {
    stop+=len;
  }

  if ((stop>=start && step<0) || (stop<=start && step>0)) return std::vector<casadi_int>();

  return casadi::range(start, stop, step, len);
}


#endif
