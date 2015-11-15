#include "common/string.hpp"
#include "exception/exception.hpp"
#include "lex/sourceLocation.hpp"

class ParseException : public Exception {
    SourceLocation loc;
    String full_message;
    const char *what_str;

    public:
        ParseException(SourceLocation _loc, const String &_message);
    	virtual ~ParseException() throw();
        virtual const char* what() const throw();
};
