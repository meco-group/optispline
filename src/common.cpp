#include "common.h"

namespace spline{
    int product(const std::vector<int>& a) {
      int r = 1;
      for (int i=0;i<a.size();++i) r*=a[i];
      return r;
    }

    int sum(const std::vector<int>& a) {
      int r = 0;
      for (int i=0;i<a.size();++i) r+=a[i];
      return r;
    }
}