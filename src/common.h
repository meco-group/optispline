#ifndef COMMON_H
#define COMMON_H
#include <memory>
#include <string>
#include <exception>
#include <sstream>
#include <vector>

namespace spline {

class SplineException : public std::exception {
  public:
  //! \brief Default constructor
  SplineException() {
  }

  //! \brief Form message string
  explicit SplineException(const std::string& msg) : msg_(msg) {}

  //! \brief Destructor
  ~SplineException() throw() {}

  //! \brief Display error
  virtual const char* what() const throw() {
    return msg_.c_str();
  }

  //! \brief Append a message
  SplineException& operator<<(const std::string& msg) {
    msg_ += msg;
    return *this;
  }

  //! \brief Append an exception
  SplineException& operator<<(const std::exception& ex) {
    msg_ += " => ";
    msg_ += ex.what();
    return *this;
  }

  protected:
  std::string msg_;
};

// Convert to string
#define SPLINE_ASSERT_STR1(x) #x
#define SPLINE_ASSERT_STR(x) SPLINE_ASSERT_STR1(x)

// String denoting where the assertion is situated
#define SPLINE_ASSERT_WHERE " on line " SPLINE_ASSERT_STR(__LINE__) \
    " of file " SPLINE_ASSERT_STR(__FILE__)

#define spline_assert_message(x, msg) \
{ \
  bool is_ok; \
  try { \
    is_ok = x; \
  } catch(std::exception& ex) { \
      throw spline::SplineException(std::string("When trying to check the assertion \"" \
        SPLINE_ASSERT_STR(x) "\"" SPLINE_ASSERT_WHERE ", caught: \n")+ex.what()); \
  } \
 if (!is_ok) { \
  std::stringstream ss_internal_; \
  ss_internal_ << "The assertion \"" SPLINE_ASSERT_STR(x) "\"" SPLINE_ASSERT_WHERE \
      " failed. " << std::endl << msg  <<  std::endl;   \
  throw spline::SplineException(ss_internal_.str()); \
 }\
} \

#define spline_assert(x) spline_assert_message(x, "No description")

#define spline_error(msg) spline_assert_message(false, msg)

#ifndef SWIG
int product(const std::vector<int>& a);  // number of elements in vector
int sum(const std::vector<int>& a);  // sum of elements in vector
#endif

double pow(double a, int n);

} // namespace spline
#endif  // COMMON_H
