#include "parseException.hpp"
#include "common/string.hpp"

ParseException::ParseException(SourceLocation _loc, String _message) : Exception(_message), loc(_loc) {
}

const char* ParseException::what() const throw() {
    SourceLocation loc_ = loc;

    String str;
    str.append(loc_.toString());
    str.append(": ");
    str.append(message);
    return str.c_str();
}
