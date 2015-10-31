#include "sourceLocation.hpp"
#include "common/char.hpp"

SourceLocation::SourceLocation() {
    input = NULL;
    offset = 0;
    line = 0;
    ch = 0;
}

SourceLocation::SourceLocation(Input *_input, int _offset) {
    input = _input;
    offset = _offset;
}

bool SourceLocation::isValid() {
    return input != NULL; //TODO: figure this out later
}

int SourceLocation::getLine() {
    if(line <= 0) {
        int seek = input->tell();
        input->set(0);
        line = 1;

        for(int i = 0; i < seek; i++) {
            if(Char::isEndOfLine(input->get())) {
                line++;
            }
        }
    }

    return line;
}

int SourceLocation::getLineOffset() {
    if(ch <= 0) {
        int seek = input->tell();
        int off = seek;
        ch = 1;

        while(off >= 0) {
            input->set(off);
            if(Char::isEndOfLine(input->peek())) {
                break;
            }
            ch++;
        }

        input->set(seek);
    }

    return ch;
}
