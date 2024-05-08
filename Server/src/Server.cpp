#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "ThreadPool.h"

#include <functional>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <cerrno>

#define BUFFER_SIZE 1024


Server::Server(EventLoop* loop) : main_reactor(loop) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::new_connection, this, std::placeholders::_1);
    acceptor->set_new_connection_callback(cb);

    int size = static_cast<int>(std::thread::hardware_concurrency());
    thread_pool = new ThreadPool(size);

    for (int i = 0; i < size; i++) {
        sub_reactors.push_back(new EventLoop());
    }

    for (int i = 0; i < size; i++) {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, sub_reactors[i]);
        thread_pool->add(std::move(sub_loop));
    }
}

Server::~Server() {
    delete acceptor;
    delete thread_pool;
}

void Server::new_connection(Socket* sock) {
    int sockfd = sock->get_fd();
    if (sockfd != -1) {
        int idx = sockfd % sub_reactors.size();
        Connection* conn = new Connection(sub_reactors[idx], sock);
        std::function<void(Socket*)> cb = std::bind(&Server::delete_connection, this, std::placeholders::_1);
        conn->set_delete_connection_callback(cb);
        conn->set_on_message_callback(on_message_callback);
        connections[sockfd] = conn;

        if (new_connect_callback) {
            new_connect_callback(conn);
        }
    }
}

void Server::delete_connection(Socket* sock) {
    int sockfd = sock->get_fd();
    if (sockfd != -1) {
        auto it = connections.find(sockfd);
        if (it != connections.end()) {
            Connection* conn = connections[sockfd];
            connections.erase(sockfd);
            delete conn;
            conn = nullptr;
        }
    }
}

void Server::on_connect(std::function<void(Connection*)> fn) {
    on_connect_callback = std::move(fn);
}

void Server::on_message(std::function<void(Connection*)> fn) {
    on_message_callback = std::move(fn);
}

void Server::new_connect(std::function<void(Connection*)> fn) {
    new_connect_callback = std::move(fn);
}
