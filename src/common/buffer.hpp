#ifndef _BUFFER_HPP
#define _BUFFER_HPP

#define _BUFFER_DEFAULT_SIZE 16

#include <stdlib.h>
#include "exception/exception.hpp"

template<typename T>
class Buffer {
    T *ptr;
    int front;
    int len;
    int cap;

    public:
    Buffer() {
        ptr = (T*) malloc(sizeof(T) * _BUFFER_DEFAULT_SIZE);
        front = 0;
        len = 0;
        cap = _BUFFER_DEFAULT_SIZE;
    }

    ~Buffer() {
        if(len > 0) throw new Exception("Buffer must be empty to destroy");
        free(ptr);
    }

    T pop() {
        if(len <= 0) throw new Exception("Attempt to pop empty buffer");
        T tmp = ptr[front];
        len--;
        front = (front + 1) % cap;
        return tmp;
    }

    void push(T t) {
        ptr[(front + len) % cap] = t;
        len++;
        if(len >= cap) throw new Exception("Resize not implemented");
    }

    T &peek(int i = 0) {
        return ptr[(front + i) % cap];
    }

    void clear() {
        len = 0;
    }

    /*
    void clear() {
        for(int i = 0; i < len; i++) {
            ~ptr[i];
        }
    }*/

    int length() {
        return len;
    }

    bool empty() {
        return len <= 0;
    }
};

#endif
