#include "Buffer.h"
#include <cstring>
#include <iostream>


Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::append(const char* str, int size) {
    for (int i = 0; i < size; i++) {
        if (str[i] == '\0') break;
        buf.push_back(str[i]);
    }
}

ssize_t Buffer::size() {
    return buf.size();
}

const char* Buffer::c_str() {
    return buf.c_str();
}

void Buffer::clear() {
    buf.clear();
}

void Buffer::getline(istream& fin) {
    buf.clear();
    std::getline(fin, buf);
}

void Buffer::set_buf(const char* buf) {
    this->buf.clear();
    this->buf.append(buf);
}
