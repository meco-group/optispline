#ifndef ANYSLICE_HPP_INCLUDE
#define ANYSLICE_HPP_INCLUDE

#include <vector>

  class AnySlice {
  public:
    int start_;
    int stop_;
    int step_;

    std::vector<int> indices_;

    bool has_indices_;

    /// Default constructor - all elements
    AnySlice();
    AnySlice(int i);
    AnySlice(int start, int stop, int step=1);
    AnySlice(const std::vector<int>& indices);

    std::vector<int> indices(int len) const;

  };
  
  
#endif
