#ifndef _INPUT_HPP
#define _INPUT_HPP

class Input {
    public:
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
