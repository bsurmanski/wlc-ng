#ifndef _SOURCELOCATION_HPP
#define _SOURCELOCATION_HPP

#include "common/string.hpp"

class Input;

struct SourceLocation {
    Input *input;
    int offset; // character offset into file
                // if column/char info is needed, it can be computed

    int line;
    int ch;

    SourceLocation();
    SourceLocation(Input *_input, int _offset);

    bool isValid() const;
    int getLine();
    int getLineOffset();
    String toString();
};

#endif
