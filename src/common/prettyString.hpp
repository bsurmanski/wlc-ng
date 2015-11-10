#ifndef _PRETTYSTRING_HPP
#define _PRETTYSTRING_HPP

#include "object.hpp"
#include "string.hpp"

class PrettyString : Object {
    int _indent;
    bool _newline;
    String contents;

    void writeIndent();

    public:
    virtual ~PrettyString();
    PrettyString();

    void reset();
    void indent();
    void unindent();
    void newline();
    void write(String str);
    void write(const char *c_str);
    String &toString();
};

#endif
