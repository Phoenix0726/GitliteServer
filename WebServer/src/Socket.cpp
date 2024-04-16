#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>


Socket::Socket() {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}

Socket::Socket(int fd) : fd(fd) {
    errif(fd == -1, "scoket create error");
}

Socket::~Socket() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress* addr) {
    errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "socket bind error");
}

void Socket::listen() {
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

bool Socket::isnonblocking() {
    return (fcntl(fd, F_GETFL) & O_NONBLOCK) != 0;
}

int Socket::accept(InetAddress* addr) {
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    return clnt_sockfd;
}

void Socket::connect(InetAddress* addr) {
    struct sockaddr_in serv_addr = addr->get_addr();
    socklen_t addr_len = addr->get_addr_len();
    errif(::connect(fd, (sockaddr*)&serv_addr, addr_len) == -1, "socket connect error");
}

void Socket::connect(const char* ip, uint16_t port) {
    InetAddress* addr = new InetAddress(ip, port);
    connect(addr);
    delete addr;
}

int Socket::get_fd() {
    return fd;
}
