#include "sourceLocation.hpp"
#include "common/char.hpp"
#include "io/input.hpp"

SourceLocation::SourceLocation() {
    input = NULL;
    offset = 0;
    line = 0;
    ch = 0;
}

SourceLocation::SourceLocation(Input *_input, int _offset) {
    input = _input;
    offset = _offset;
    line = 0;
    ch = 0;
}

bool SourceLocation::isValid() const {
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
        int off = seek-1;
        ch = 1;

        while(off > 0 && !Char::isEndOfLine(input->peek())) {
            input->set(--off);
            ch++;
        }

        input->set(seek);
    }

    return ch;
}

String SourceLocation::toString() {
    String str;
    str.append(input->getName());
    str.append(":");
    str.append(String::fromInt(getLine()));
    str.append(":");
    str.append(String::fromInt(getLineOffset()));
    return str;
}
