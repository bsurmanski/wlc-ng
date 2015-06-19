#include "exception.hpp"


Exception::Exception(String _message) {
	message = _message;
}

Exception::~Exception() {
}

String &Exception::getMessage() {
	return message;
}