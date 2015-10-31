#include "parseException.hpp"

#include "common/string.hpp"

ParseException::ParseException(SourceLocation _loc, String _message) : Exception(_message), loc(_loc) {
    full_message.append(loc.toString());
    full_message.append(": ");
    full_message.append(message);
    what_str = full_message.c_str();
}

ParseException::~ParseException() throw() {}

const char* ParseException::what() const throw() {
    return what_str;
}
