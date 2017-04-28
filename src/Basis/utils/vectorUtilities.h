#ifndef CPP_SPLINES_VECTORUTILITIES_H
#define CPP_SPLINES_VECTORUTILITIES_H

#include <vector>
#include <casadi/casadi.hpp>

namespace spline{


    template<class T>
    bool AnyScalarComp(const T& lhs, const T& rhs) { return casadi::casadi_limits<T>::is_one(lhs<rhs); }

    template<class T>
    bool AnyScalarEq(const T& lhs, const T& rhs) { return casadi::casadi_limits<T>::is_zero(lhs-rhs); }

    template<class T>
    const std::vector<T> unionKnots(const std::vector<T> & kn1,  const std::vector<T> & kn2, int degree, int degree1, int degree2) {
      // assume knots are sorted
      std::vector<T> uniqueKnots(kn1.size() + kn2.size());

      auto it=std::set_union (kn1.begin(), kn1.end(), kn2.begin(),kn2.end(), uniqueKnots.begin(), AnyScalarComp<T>);
      uniqueKnots.resize(it-uniqueKnots.begin());
      uniqueKnots.erase( unique( uniqueKnots.begin(), uniqueKnots.end(), AnyScalarEq<T>), uniqueKnots.end() );

      int count1, count2, count;
      std::vector<T> knots (0);
      std::vector<T> multiple (0);

      for (auto const& k: uniqueKnots) {
          count1 = std::count_if (kn1.begin(), kn1.end(), [&](const T& lhs) { return casadi::casadi_limits<T>::is_zero(lhs-k); });
          count2 = std::count_if (kn2.begin(), kn2.end(), [&](const T& lhs) { return casadi::casadi_limits<T>::is_zero(lhs-k); });

          count =  std::max(count1 + degree - degree1, count2 + degree - degree2);

          multiple.resize(count);
          std::fill(multiple.begin(), multiple.end(), k);

          knots.insert(knots.end(), multiple.begin(), multiple.end());
      }
      return knots;
    }

    template<class T>
    std::vector<T> addKnotsUnique(const std::vector<T>& knots,  const std::vector<T>& newKnots) {

      // assume knots are sorted
      // std::sort(knots.begin(),knots.end());
      // std::sort (newKnots.begin(),newKnots.end());

      std::vector<double> uniqueKnots(knots.size() + newKnots.size());
      std::vector<double>::iterator it;

      it=std::set_union (knots.begin(),knots.end(), newKnots.begin(),newKnots.end(), uniqueKnots.begin(), AnyScalarComp<T>);
      uniqueKnots.resize(it-uniqueKnots.begin());
      uniqueKnots.erase( unique( uniqueKnots.begin(), uniqueKnots.end(), AnyScalarEq<T>), uniqueKnots.end() );

      int count;
      std::vector<double> returnKnots (0);
      std::vector<double> multiple (0);

      for (auto const& k: uniqueKnots) {
          count = std::count_if (knots.begin(), knots.end(), [&](const T& lhs) { return casadi::casadi_limits<T>::is_zero(lhs-k); }) + std::count_if (newKnots.begin(), newKnots.end(), [&](const T& lhs) { return casadi::casadi_limits<T>::is_zero(lhs-k); });
          multiple.resize(count);
          std::fill(multiple.begin(), multiple.end(), k);

          returnKnots.insert(returnKnots.end(), multiple.begin(), multiple.end());
      }
      return returnKnots;
    }

    template<class T>
    std::vector<T> addKnotsNotUnique(const std::vector<T>& knots,  const std::vector<T>& newKnots) {
      // assume knots are sorted

      std::vector<double> returnKnots(knots.size() + newKnots.size());
      std::vector<double>::iterator it;

      it=std::set_union (knots.begin(),knots.end(), newKnots.begin(),newKnots.end(), returnKnots.begin(), AnyScalarComp<T>);
      returnKnots.resize(it-returnKnots.begin());

      return returnKnots;
    }

    template<class T>
    const std::vector<T> increaseMultiplicityKnots(const std::vector<T>& knots_,  int increase) {
      if (increase <= 0) { // noting happens
          return knots_;
      } else {
          int newSize = 0;
          std::vector<T> returnKnots(knots_.size() * (increase+1));

          for (int i = 0; i < knots_.size() - 1; ++i) {
              if (casadi::casadi_limits<T>::is_zero(knots_[i] - knots_[i+1])) {
                  returnKnots[newSize] = knots_[i];
                  newSize++;
              } else {
                  for (int j = 0; j < increase + 1; ++j) {
                      returnKnots[newSize] = knots_[i];
                      newSize++;
                  }
              }
          }

          for (int j = 0; j < increase + 1; ++j) {
              returnKnots[newSize] = knots_[knots_.size()-1];
              newSize++;
          }
          returnKnots.resize(newSize);
          return returnKnots;
      }
    }

    template<class T>
    const std::vector<T> increaseMultiplicityFirstLast(const std::vector<T>& knots_,  int increase) {
      if (increase <= 0) { // noting happens
          return knots_;
      } else {
          int newSize = 0;
          std::vector<T> returnKnots(knots_.size() + (increase*2));

          for (int j = 0; j < increase ; ++j) {
              returnKnots[newSize] = knots_[0];
              newSize++;
          }

          for (int i = 0; i < knots_.size(); ++i) {
              returnKnots[newSize] = knots_[i];
              newSize++;
          }

          for (int j = 0; j < increase ; ++j) {
              returnKnots[newSize] = knots_[knots_.size()-1];
              newSize++;
          }
          returnKnots.resize(newSize);
          return returnKnots;
      }
    }



}  // namespace spline

#endif //CPP_SPLINES_VECTORUTILITIES_H
