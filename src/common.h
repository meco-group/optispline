#ifndef COMMON_H
#define COMMON_H
#include <memory>
#include <string>
#include <exception>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#ifdef __linux__
#include <execinfo.h>
#endif

namespace spline {

class SplineException : public std::exception {
  public:
  //! \brief Default constructor
  SplineException() {
  }

  //! \brief Form message string
  explicit SplineException(const std::string& msg) : msg_(msg) {

#ifdef __linux__
    void *trace[256];
    char **messages = (char **)NULL;
    int i, trace_size = 0;

    trace_size = backtrace(trace, 256);
    messages = backtrace_symbols(trace, trace_size);
    /* skip first stack frame (points here) */
    msg_+="** start backtrace **\n";
    for (i=1; i<trace_size; ++i)
    {
      std::string message = messages[i];
      if (message.find("python")==std::string::npos) {

        std::string symbol = message.substr(message.find("(")+1,message.find(")")-message.find("(")-1);
        std::string symbol_name = symbol.substr(0,symbol.find("+"));
        std::string libname = message.substr(0,message.find("("));

        {
          std::string command = "eu-addr2line -e " + libname + " " + symbol + "> .temp.txt"; //last parameter is the name of this app
          int retcode = system(command.c_str());
          if (retcode) { msg_+="  install 'elfutils' to get a stacktrace\n"; break; }
          std::stringstream res; res << std::ifstream(".temp.txt").rdbuf();

       std::string line =  res.str();
              std::size_t found_begin = line.find_last_of("/\\");
              std::size_t found_end = line.find_first_of(':', found_begin);
              if (found_end!=std::string::npos && found_begin!=std::string::npos){
                  line.insert(found_end , "\x1b[0m");
                  line.insert(found_begin + 1, "\x1b[35m");
              }
              msg_ += line;

        }
        if (!symbol_name.empty()) {
          std::string command = "c++filt " + symbol_name + "> .temp.txt"; //last parameter is the name of this app
          int retcode = system(command.c_str());
          if (retcode) { msg_+="    install 'c++filt' to get a stacktrace\n"; break; }
          std::stringstream res; res << std::ifstream(".temp.txt").rdbuf();

              std::string line =  res.str();
              std::size_t found_end = line.find("(");
              std::size_t found_begin = line.rfind(':', found_end);
              if (found_end!=std::string::npos && found_begin!=std::string::npos){
                  if(line[found_end + 1] == ')') found_end += 2;
                  line.insert(found_end , "\x1b[0m");
                  line.insert(found_begin + 1, "\x1b[34m");
              }
              msg_ += line;
        }
      }
    }
    msg_+="** end backtrace **";
#endif

  }

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
