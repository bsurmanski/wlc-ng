#ifndef _STRING_HPP
#define _STRING_HPP

#include <stdlib.h>

#define STRING_LONG 28

class String {
	int capacity;
	int len;

    // if Token data is > LONG bytes, then data is redirected through ptr
    // what the data contains depends on what type of token this is:
    // char literal: value in data[0]
    // string literal: value in data if *short*, else in *dptr
    // numeric literal: value in data
    // identifier: name in data if *short*, else in *dptr
    // keyword: null
    union {
        char data[STRING_LONG];
        char *dptr;
    };
	
	bool isLong() const;
	void resize(int cap);
	
	public:
	String();
	String(const char *str);
	~String();
	
	size_t length() const;
	char &operator[](int i);
	const char &operator[](int i) const;
	bool operator==(const String &o) const;
	bool operator!=(const String &o) const;
	bool operator<(const String &o) const;
	bool operator>(const String &o) const;
	bool operator<=(const String &o) const;
	bool operator>=(const String &o) const;
	void operator+=(char c);
	void operator+=(const String &o);
	const char &charAt(int i) const;
	char &charAt(int i);
	bool equals(const String &o) const;
	int compare(const String &o) const;
	void append(char c);
	void append(const String &o);
	char *c_str();
	char *dataPtr();
	const char *dataPtr() const;
	void copy(char *dst, size_t len, size_t pos = 0) const;
	bool empty() const;
};

#endif