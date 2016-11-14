#ifndef TENSOREXCEPTION_H
#define TENSOREXCEPTION_H
#include <memory>
#include <string>
#include <exception>
#include <sstream>

class TensorException : public std::exception {
  public:
  //! \brief Default constructor
  TensorException() {
  }

  //! \brief Form message string
  explicit TensorException(const std::string& msg) : msg_(msg) {}

  //! \brief Destructor
  ~TensorException() throw() {}

  //! \brief Display error
  virtual const char* what() const throw() {
    return msg_.c_str();
  }

  //! \brief Append a message
  TensorException& operator<<(const std::string& msg) {
    msg_ += msg;
    return *this;
  }

  //! \brief Append an exception
  TensorException& operator<<(const std::exception& ex) {
    msg_ += " => ";
    msg_ += ex.what();
    return *this;
  }

  protected:
  std::string msg_;
};

// Convert to string
#define TENSOR_ASSERT_STR1(x) #x
#define TENSOR_ASSERT_STR(x) TENSOR_ASSERT_STR1(x)

// String denoting where the assertion is situated
#define TENSOR_ASSERT_WHERE " on line " TENSOR_ASSERT_STR(__LINE__) \
    " of file " TENSOR_ASSERT_STR(__FILE__)
    
#define tensor_assert_message(x, msg) \
{ \
  bool is_ok; \
  try { \
    is_ok = x; \
  } catch(std::exception& ex) { \
      throw TensorException(std::string("When trying to check the assertion \"" \
        TENSOR_ASSERT_STR(x) "\"" TENSOR_ASSERT_WHERE ", caught: \n")+ex.what()); \
  } \
 if (!is_ok) { \
  std::stringstream ss_internal_; \
  ss_internal_ << "The assertion \"" TENSOR_ASSERT_STR(x) "\"" TENSOR_ASSERT_WHERE \
      " failed. " << std::endl << msg  <<  std::endl;   \
  throw TensorException(ss_internal_.str()); \
 }\
} \

#define tensor_assert(x) tensor_assert_message(x, "No description")


#endif  // TENSOREXCEPTION_H
