#ifndef _FILE_HPP
#define _FILE_HPP

#include <stdio.h>

#include "common/string.hpp"
#include "input.hpp"

class File : Input {
    String filename;
    FILE *fp;
    long size;

    public:
    static bool exists(String filename);
    static File *findInPath(String name, String path);

    File(String _filename);
	~File();
    String getPath();
    String getName();
    String getBasename();
    String getExtension();

    long getSize();
    int seek(long sval);
    int set(long sval);
    int rset(long sval);
    long tell();
    int get();
    int peek(int ahead = 0);
    long read(void *buf, long sz, long nmemb);
    bool eof();
};

#endif
