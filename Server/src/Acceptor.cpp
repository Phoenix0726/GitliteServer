#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

#include <cstdio>


Acceptor::Acceptor(EventLoop* loop) : loop(loop) {
    sock = new Socket();
    InetAddress* addr = new InetAddress("127.0.0.1", 3333);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();

    accept_channel = new Channel(loop, sock->get_fd());
    std::function<void()> cb = std::bind(&Acceptor::accept_connection, this);
    accept_channel->set_read_callback(cb);
    accept_channel->enable_read();

    delete addr;
}

Acceptor::~Acceptor() {
    delete sock;
    delete accept_channel;
}

void Acceptor::accept_connection() {
    InetAddress* clnt_addr = new InetAddress();
    Socket* clnt_sock = new Socket(sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->get_fd(), inet_ntoa(clnt_addr->get_addr().sin_addr), ntohs(clnt_addr->get_addr().sin_port));
    clnt_sock->setnonblocking();

    new_connection_callback(clnt_sock);

    delete clnt_addr;
}

void Acceptor::set_new_connection_callback(std::function<void(Socket*)> cb) {
    new_connection_callback = cb;
}
