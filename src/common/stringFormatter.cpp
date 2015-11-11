#include "stringFormatter.hpp"

StringFormatter::~StringFormatter() {
}

StringFormatter::StringFormatter() {
    _indent = 0;
    _newline = false;
}

void StringFormatter::reset() {
    contents = String("");
    _indent = 0;
}

void StringFormatter::indent() {
    _indent++;
}

void StringFormatter::unindent() {
    _indent--;
    if(_indent < 0) _indent = 0;
}

void StringFormatter::newline() {
    contents.append("\n");
    _newline = true;
}

void StringFormatter::writeIndent() {
    for(int i = 0; i < _indent; i++) {
        contents.append("  ");
    }
}

void StringFormatter::write(String str) {
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

void StringFormatter::write(const char *c_str) {
    write(String(c_str));
}

String &StringFormatter::toString() {
    return contents;
}
