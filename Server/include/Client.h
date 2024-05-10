#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <fstream>
#include <iostream>
#include <string>

using std::istream;
using std::ofstream;
using std::string;


class Socket;
class Buffer;

class Client {
private:
    Socket* sock;
    Buffer* send_buffer;
    Buffer* read_buffer;

    void _write(string str="");
public:
    ofstream fout;

    Client();
    ~Client();
    void send(string str="");
    void sendfile(string file);
    void clone(string path);
};


#endif