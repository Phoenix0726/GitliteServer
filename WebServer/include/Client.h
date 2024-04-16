#pragma once

#include <fstream>
#include <iostream>
#include <string>

using std::istream;
using std::string;


class Socket;
class Buffer;

class Client {
private:
    Socket* sock;
    Buffer* send_buffer;
    Buffer* read_buffer;
public:
    Client();
    ~Client();
    void send(string str="");
    void sendfile(istream& fin=std::cin);
};