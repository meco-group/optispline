#ifndef SLICE_HPP_INCLUDE
#define SLICE_HPP_INCLUDE

#include "slice.hpp"
#include <limits>
#include <casadi/casadi.hpp>
#include <iostream>
/// Default constructor - all elements
AnySlice::AnySlice() : start_(0), stop_(std::numeric_limits<int>::max()), step_(1),
  has_indices_(false) { };
AnySlice::AnySlice(int i) : start_(i), stop_(i+1), step_(1), has_indices_(false) {};
AnySlice::AnySlice(int start, int stop, int step) : start_(start), stop_(stop), step_(step),
  has_indices_(false){};
AnySlice::AnySlice(const std::vector<int>& indices) : indices_(indices), has_indices_(true) {};

std::vector<int> AnySlice::indices(int len) const {
  if (has_indices_) return indices_;

  int start = start_;
  int stop = stop_;
  int step = step_;

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
  
  if ((stop>=start && step<0) || (stop<=start && step>0)) return std::vector<int>();
  
  std::cout << "start" << start << "stop" << stop << "step" << step << std::endl;

  return casadi::range(start, stop, step, len);
}

  
#endif
