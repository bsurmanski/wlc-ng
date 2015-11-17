#ifndef _DYNARRAY_HPP
#define _DYNARRAY_HPP

#define DYNARRAY_DEFAULT_CAP 16

#include <stdlib.h>
#include <string.h>

template<typename T>
struct _ArrayData {
    int refcount;
    int len;
    int cap;

    T *ptr;

    _ArrayData<T>(int n = DYNARRAY_DEFAULT_CAP) {
        refcount = 1;
        len = 0;
        cap = n;
        ptr = (T*) malloc(sizeof(T) * n);
    }

    ~_ArrayData<T>() {
        for(int i = 0; i < len; i++) {
            ptr[i].~T();
        }
        free(ptr);
    }

    _ArrayData *retain() {
        refcount++;
        return this;
    }

    bool release() {
        refcount--;
        if(refcount <= 0) {
            delete this;
            return true;
        }
        return false;
    }

    _ArrayData *dup() {
        _ArrayData *ret = new _ArrayData(cap);
        memcpy(ret->ptr, ptr, sizeof(T) * len);
        return ret;
    }

    void append(const T &t) {
        if(len >= cap) {
            ptr = (T*) realloc(ptr, cap * 2);
            cap = cap * 2;
        }

        ptr[len++] = t;
    }

    T pop() {
        len--;
        T tmp = ptr[len];
        ptr[len].~T();
        return tmp;
    }
};

template<typename T>
class DynArray {
    _ArrayData<T> *data;

    public:
    DynArray<T>() {
        data = new _ArrayData<T>;
    }

    DynArray<T>(const DynArray<T> &o) {
        data = o.data->retain();
    }

    ~DynArray() {
        data->release();
    }

    const DynArray<T>& operator=(const DynArray<T> &o) {
        data->release();
        data = o.data.retain();
        return *this;
    }

    T &operator[](int i) const {
        return at(i);
    }

    T &at(int i) const {
        return data->ptr[i];
    }

    int size() const {
        return data->len;
    }

    bool empty() const {
        return size() <= 0;
    }

    int capcity() const {
        return data->cap;
    }

    // DOES NOT DELETE ANY ALLOCATED ITEMS
    void clear() {
        while(!empty()) {
            data->pop();
        }
    }

    void append(const T &t) {
        data->append(t);
    }

    T pop() {
        return data->pop();
    }

    T remove(int i) {

    }
};

#endif
