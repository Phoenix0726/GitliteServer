#pragma once

#include "InetAddress.h"


class Socket {
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();

    void setnonblocking();
    bool isnonblocking();

    int accept(InetAddress*);

    void connect(InetAddress*);
    void connect(const char* ip, uint16_t port);

    int get_fd();
};
