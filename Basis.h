// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#ifndef BASIS_H_
#define BASIS_H_

namespace spline {
class Basis {
 public:
  explicit Basis(int degree) : degree(degree) { }
  int get_degree() const {
        return degree;
    }

 private:
  int degree;
};
}
#endif  // BASIS_H_
