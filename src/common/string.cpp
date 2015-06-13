#include "string.hpp"
#include "exception/exception.hpp"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) > (B) ? (B) : (A))

_StringData *_StringData::retain() {
	refcount++;
	return this;
}

bool _StringData::release() {
	refcount--;
	if(refcount <= 0) {
		free(this);
		return true;
	}
	return false;
}

String::String() {
	len = 0;
	capacity = STRING_LONG;
}

String::String(const char *str) {
	size_t strl = strlen(str);
	len = strl;
	capacity = MAX(len, STRING_LONG);
	if(len > STRING_LONG) {
		refdata = (_StringData*) malloc(capacity + sizeof(_StringData));
		refdata->refcount = 1;
		memcpy(refdata->data, str, len);
	} else {
		memcpy(data, str, len);
	}
}


String::String(const String& o) {
	len = o.len;
	capacity = o.capacity;
	if(o.isLong()) {
		refdata = o.refdata->retain();
	} else {
		memcpy(data, o.data, o.len);
	}
}

String::~String() {
	if(isLong()) {
		refdata->release();
	}
}

bool String::isLong() const {
	return capacity > STRING_LONG;
}

void String::resize(int cap) {
	if(cap > capacity && cap > STRING_LONG) {
		_StringData *ndata = (_StringData*) malloc(cap + sizeof(_StringData));
		assert(ndata);
		ndata->refcount = 1;
		if(isLong()) {
			memcpy(ndata->data, refdata->data, len);
			refdata->release();
		} else {
			memcpy(ndata->data, &this->data[0], len);
		}
		capacity = cap;
		refdata = ndata;
	} else if(capacity <= STRING_LONG) {
	} else {
		throw new Exception("Cannot shrink string");
	}
}

size_t String::length() const {
	return len;
}

String &String::operator=(const String &o) {
	if(isLong()) {
		refdata->release();
	}
	
	len = o.len;
	capacity = o.capacity;
	if(o.isLong()) {
		refdata = o.refdata->retain();
	} else {
		memcpy(data, o.data, o.len);
	}
	
	return *this;
}

char &String::operator[](int i) {
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
		return refdata->data[i];
	} else {
		return data[i];
	}
}

char &String::charAt(int i) {
	if(i >= capacity) {
		throw new Exception("index out of bounds");
	}
	
	if(isLong()) {
		return refdata->data[i];
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
		resize(capacity+STRING_APPEND_PAD);
	}
	charAt(len) = c;
	len++;
}

void String::append(const String &o) {
	if(length() + o.length() > capacity) {
		resize(length() + o.length()+STRING_APPEND_PAD);
	}
	
	for(int i = 0; i < o.length(); i++) {
		charAt(len + i) = o.charAt(i);
	}
	
	len += o.length();
}

char *String::c_str() {
	if(len + 1 >= capacity) {
		resize(len+STRING_APPEND_PAD);
	}
	charAt(len) = '\0';
	return dataPtr();
}

char *String::dataPtr() {
	if(isLong()) {
		return refdata->data;
	} else {
		return data;
	}
}

const char *String::dataPtr() const {
	if(isLong()) {
		return refdata->data;
	} else {
		return data;
	}
}

void String::copy(char *dst, size_t len, size_t pos) const {
	if(isLong()) {
		memcpy(dst, &refdata->data[pos], len);
	} else {
		memcpy(dst, &data[pos], len);
	}
}

bool String::empty() const {
	return len <= 0;
}

String String::dup() const {
	String o;
	o.resize(capacity);
	copy(o.dataPtr(), length(), 0);
	o.len = length();
	return o;
}