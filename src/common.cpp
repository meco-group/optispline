#include "common.h"
#include <math.h>

namespace spline{

    double pow(double a, casadi_int n) {
      return ::pow(a, static_cast<double>(n) );
    }
}
