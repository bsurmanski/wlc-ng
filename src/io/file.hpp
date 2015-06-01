#ifndef _FILE_HPP
#define _FILE_HPP

#include <string>

#include "input.hpp"

class File : Input {
    std::string name;
    long size;

    public:
    File(std::string path);
    std::string getName();

    long getSize();
    int seek(long sval);
    int set(long sval);
    int rset(long sval);
    long tell();
    int get();
    int peek();
    long read(void *buf, long sz, long nmemb);
    bool eof();
};

#endif
