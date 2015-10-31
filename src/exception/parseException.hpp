#include "exception.hpp"
#include "common/string.hpp"
#include "lex/sourceLocation.hpp"

class ParseException : public Exception {
    SourceLocation loc;
    String full_message;
    const char *what_str;

    public:
        ParseException(SourceLocation _loc, String _message);
    	virtual ~ParseException() throw();
        virtual const char* what() const throw();
};
