#ifndef _FILE_HPP
#define _FILE_HPP

#include "common/string.hpp"
#include "input.hpp"

class File : Input {
    String name;
    long size;

    public:
    File(String path);
	~File();
    String getName();

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
