#ifndef _STRINGFORMATTER_HPP
#define _STRINGFORMATTER_HPP

#include "object.hpp"
#include "string.hpp"

class StringFormatter : Object {
    int _indent;
    bool _newline;
    String contents;

    void writeIndent();

    public:
    virtual ~StringFormatter();
    StringFormatter();

    void reset();
    void indent();
    void unindent();
    void newline();
    void write(const String &str);
    void write(const char *c_str);
    String &toString();
};

#endif
