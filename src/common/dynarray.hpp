#ifndef _DYNARRAY_HPP
#define _DYNARRAY_HPP

#define DYNARRAY_DEFAULT_CAP 16

#include <stdlib.h>
#include <string.h>

template<typename T>
class DynArray {
    T *ptr;
    int len;
    int cap;

    public:
    DynArray() {
        ptr = (T*) malloc(sizeof(T) * DYNARRAY_DEFAULT_CAP);
        len = 0;
        cap = DYNARRAY_DEFAULT_CAP;
    }

    ~DynArray() {
        free(ptr); // dealloc members?
    }

    T &operator[](int i) const {
        return at(i);
    }

    T &at(int i) const {
        return ptr[i];
    }

    int size() const {
        return len;
    }

    int capcity() const {
        return cap;
    }

    void reserve(int i) {
        if(cap < i) {
            ptr = realloc(ptr, i * sizeof(T));
            cap = i;
        }
    }

    void clear() {

    }

    void append(T &t) {
        if(len >= cap) {
            ptr = realloc(ptr, cap * 2);
            cap = cap * 2;
        }

        ptr[len++] = t;
    }

    T remove(int i) {

    }
};

#endif
