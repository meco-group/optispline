#ifndef TENSOREXCEPTION_H
#define TENSOREXCEPTION_H
#include <memory>
#include <string>
#include <exception>
#include <sstream>
#include <iostream>
#include <fstream>
#ifdef __linux__
#include <execinfo.h>
#endif

class TensorException : public std::exception {
  public:
  //! \brief Default constructor
  TensorException() {
  }

  //! \brief Form message string
  explicit TensorException(const std::string& msg) : msg_(msg) {


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
                  line.insert(found_begin + 1, "\x1b[33m");
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
                  line.insert(found_begin + 1, "\x1b[32m");
              }
              msg_ += line;
            }
          }
        }
        msg_+="** end backtrace **\n";
    #endif

  }

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
