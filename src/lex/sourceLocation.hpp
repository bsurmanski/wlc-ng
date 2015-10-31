#include "io/file.hpp"

struct SourceLocation {
    Input *input;
    int offset; // character offset into file
                // if column/char info is needed, it can be computed

    int line;
    int ch;

    SourceLocation();
    SourceLocation(Input *_input, int _offset);

    bool isValid();
    int getLine();
    int getLineOffset();
};
