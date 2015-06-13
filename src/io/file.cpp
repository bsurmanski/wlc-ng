#include "file.hpp"

File::File(String path) {
}

File::~File() {
}

String File::getName() {
    return name;
}

long File::getSize() {
    return size;
}

int File::seek(long sval) {
}

int File::set(long sval) {
}

int File::rset(long sval) {
}

long File::tell() {
}

int File::get() {
}

int File::peek() {
}

long File::read(void *buf, long sz, long nmemb) {
}

bool File::eof() {

}
