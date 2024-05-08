#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <string>
#include <fstream>
#include <iostream>

using std::istream;


class Buffer {
private:
    std::string buf;
public:
    Buffer();
    ~Buffer();
    void append(const char* str, int size);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline(istream& fin=std::cin);
    void set_buf(const char*);
};


#endif