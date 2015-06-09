#ifndef _EXCEPTION_HPP
#define _EXCEPTION_HPP

#include "common/string.hpp"

class Exception {
	String message;
	
	public:
	Exception(String message);
	virtual ~Exception();
};

#endif