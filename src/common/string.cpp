#include "string.hpp"
#include "exception/exception.hpp"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) > (B) ? (B) : (A))

String::String() {
	len = 0;
	capacity = STRING_LONG;
}

String::String(const char *str) {
	size_t strl = strlen(str);
	len = strl;
	capacity = MAX(len, STRING_LONG);
	if(len > STRING_LONG) {
		dptr = (char*) malloc(capacity);
		memcpy(dptr, str, len);
	} else {
		memcpy(data, str, len);
	}
}

String::~String() {
	if(isLong()) {
		free(dptr);
	}
}

bool String::isLong() const {
	return capacity > STRING_LONG;
}

void String::resize(int cap) {
	if(cap > capacity && cap > STRING_LONG) {
		char *ndata = (char*) malloc(cap);
		assert(ndata);
		if(isLong()) {
			memcpy(ndata, this->dptr, len);
			free(dptr);
		} else {
			memcpy(ndata, &this->data[0], len);
		}
		capacity = cap;
		dptr = ndata;
	} else if(cap < capacity && capacity < STRING_LONG) {
	} else {
		throw new Exception("Cannot shrink string");
	}
}

size_t String::length() const {
	return len;
}

char & String::operator[](int i) {
	return charAt(i);
}

const char &String::operator[](int i) const {
	return charAt(i);
}

bool String::operator==(const String &o) const {
	return equals(o);
}


bool String::operator!=(const String &o) const {
	return !equals(o);
}

bool String::operator<(const String &o) const {
	return compare(o) < 0;
}

bool String::operator>(const String &o) const {
	return compare(o) > 0;
}

bool String::operator<=(const String &o) const {
	return compare(o) <= 0;
}

bool String::operator>=(const String &o) const {
	return compare(o) >= 0;
}

void String::operator+=(char c) {
	append(c);
}

void String::operator+=(const String &o) {
	append(o);
}

bool String::equals(const String &o) const {
	return !compare(o);
}

const char &String::charAt(int i) const {
	if(i >= capacity) {
		throw new Exception("index out of bounds");
	}
	
	if(isLong()) {
		return dptr[i];
	} else {
		return data[i];
	}
}

char &String::charAt(int i) {
	if(i >= capacity) {
		throw new Exception("index out of bounds");
	}
	
	if(isLong()) {
		return dptr[i];
	} else {
		return data[i];
	}
}

int String::compare(const String &o) const {
	size_t l1 = length();
	size_t l2 = o.length();
	int cmp = memcmp(dataPtr(), o.dataPtr(), MIN(l1, l2));
	
	if(!cmp && length() < o.length()) {
		return -1;
	} else if(!cmp && length() > o.length()) {
		return 1;
	}
	
	return cmp;
}


void String::append(char c) {
	if(len == capacity) {
		resize(capacity+1);
	}
	charAt(len) = c;
	len++;
}

void String::append(const String &o) {
	if(length() + o.length() > capacity) {
		resize(length() + o.length());
	}
	
	for(int i = 0; i < o.length(); i++) {
		charAt(len + i) = o.charAt(i);
	}
	
	len += o.length();
}

char *String::c_str() {
	if(len + 1 >= capacity) {
		resize(len+1);
	}
	charAt(len) = '\0';
	return dataPtr();
}

char *String::dataPtr() {
	if(isLong()) {
		return dptr;
	} else {
		return data;
	}
}

const char *String::dataPtr() const {
	if(isLong()) {
		return dptr;
	} else {
		return data;
	}
}

void String::copy(char *dst, size_t len, size_t pos) const {
	if(isLong()) {
		memcpy(dst, &dptr[pos], len);
	} else {
		memcpy(dst, &data[pos], len);
	}
}

bool String::empty() const {
	return len <= 0;
}