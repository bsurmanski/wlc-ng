#include "prettyString.hpp"

PrettyString::~PrettyString() {
}

PrettyString::PrettyString() {
    _indent = 0;
    _newline = false;
}

void PrettyString::reset() {
    contents = String("");
    _indent = 0;
}

void PrettyString::indent() {
    _indent++;
}

void PrettyString::unindent() {
    _indent--;
    if(_indent < 0) _indent = 0;
}

void PrettyString::newline() {
    contents.append("\n");
    _newline = true;
}

void PrettyString::writeIndent() {
    for(int i = 0; i < _indent; i++) {
        contents.append("  ");
    }
}

void PrettyString::write(String str) {
    int lastWrite = -1;
    for(int i = 0; i < str.length(); i++) {
        if(_newline) {
            writeIndent();
            _newline = false;
        }

        if(str[i] == '\n') {
            contents.append(str.substring(lastWrite+1, i - lastWrite - 1));
            contents.append("\n");
            lastWrite = i;
            _newline = true;
        }
    }

    contents.append(str.substring(lastWrite+1));
}

void PrettyString::write(const char *c_str) {
    write(String(c_str));
}

String &PrettyString::toString() {
    return contents;
}
