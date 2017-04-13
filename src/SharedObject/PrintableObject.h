#ifndef PRINTABLE_OBJECT
#define PRINTABLE_OBJECT

#include <string>

namespace spline{

template<class T>
class PrintableObject {
public:
#ifndef SWIG
    virtual std::string to_string() const { return "Object"; }
    inline friend
        std::ostream& operator<<(std::ostream &stream, const PrintableObject<T>& obj) {
            return stream << obj.to_string();
    }
#endif

};

}
#endif   //  PRINTABLE_OBJECT
