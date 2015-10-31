#include "exception.hpp"
#include "common/string.hpp"
#include "lex/sourceLocation.hpp"

class ParseException : public Exception {
    SourceLocation loc;
    public:
        ParseException(SourceLocation _loc, String _message);
        virtual const char* what() const throw();
};
