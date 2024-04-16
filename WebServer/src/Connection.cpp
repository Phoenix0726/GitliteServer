#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "Buffer.h"
#include "util.h"

#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <cassert>

#define BUFFER_SIZE 1024


Connection::Connection(EventLoop* loop, Socket* sock) : loop(loop), sock(sock) {
    if (loop) {
        channel = new Channel(loop, sock->get_fd());
        channel->enable_read();
        channel->set_ET();
    }
    read_buffer = new Buffer();
    send_buffer = new Buffer();
    state = State::connected;
}

Connection::~Connection() {
    if (loop) {
        delete channel;
    }
    delete sock;
    delete read_buffer;
    delete send_buffer;
}

void Connection::_read() {
    assert(state == State::connected && "connection state is disconnected!");
    read_buffer->clear();
    if (sock->isnonblocking()) {
        readnonblocking();
    } else {
        readblocking();
    }
}

void Connection::_write() {
    assert(state == State::connected && "connection state is disconnected!");
    if (sock->isnonblocking()) {
        writenonblocking();
    } else {
        writeblocking();
    }
    send_buffer->clear();
}

void Connection::readnonblocking() {
    int sockfd = sock->get_fd();
    char buf[BUFFER_SIZE];
    while (true) {
        memset(buf, 0, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            read_buffer->append(buf, read_bytes);
        } else if (read_bytes == -1 && errno == EINTR) {
            printf("continue reading\n");
            continue;
        } else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            break;
        } else if (read_bytes == 0) {
            printf("read EOF, client fd %d disconnected\n", sockfd);
            state = State::closed;
            break;
        } else {
            printf("Ohter error on client fd %d\n", sockfd);
            state = State::closed;
            break;
        }
    }
}

void Connection::writenonblocking() {
    int sockfd = sock->get_fd();
    char buf[send_buffer->size()];
    memcpy(buf, send_buffer->c_str(), send_buffer->size());
    int data_size = send_buffer->size();
    int data_left = data_size;
    while (data_left > 0) {
        ssize_t write_bytes = write(sockfd, buf + data_size - data_left, data_left);
        if (write_bytes == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        } else if (write_bytes == -1 && errno == EAGAIN) {
            break;
        } else if (write_bytes == -1) {
            printf("Other error on client fd %d\n", sockfd);
            state = State::closed;
            break;
        }
        data_left -= write_bytes;
    }
}

void Connection::readblocking() {
    int sockfd = sock->get_fd();
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    char buf[rcv_size];
    ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
    if (read_bytes > 0) {
        read_buffer->append(buf, read_bytes);
    } else if (read_bytes == 0) {
        printf("read EOF, blocking client fd %d disconnected\n", sockfd);
        state = State::closed;
    } else if (read_bytes == -1) {
        printf("Other error on blocking client fd %d\n", sockfd);
        state = State::closed;
    }
}

void Connection::writeblocking() {
    int sockfd = sock->get_fd();
    ssize_t write_bytes = write(sockfd, send_buffer->c_str(), send_buffer->size());
    if (write_bytes == -1) {
        printf("Other error on blocking client fd %d\n", sockfd);
        state = State::closed;
    }
}

void Connection::_send(const char* msg) {
    set_send_buffer(msg);
    _write();
}

void Connection::business() {
    _read();
    on_message_callback(this);
}

void Connection::_close() {
    delete_connection_callback(sock);
}

Connection::State Connection::get_state() {
    return state;
}

void Connection::set_send_buffer(const char* str) {
    send_buffer->set_buf(str);
}

Buffer* Connection::get_read_buffer() {
    return read_buffer;
}

Buffer* Connection::get_send_buffer() {
    return send_buffer;
}

const char* Connection::read_buffer_val() {
    return read_buffer->c_str();
}

const char* Connection::send_buffer_val() {
    return send_buffer->c_str();
}

void Connection::set_delete_connection_callback(std::function<void(Socket*)> const& cb) {
    delete_connection_callback = cb;
}

void Connection::set_on_connect_callback(std::function<void(Connection*)> const& cb) {
    on_connect_callback = cb;
}

void Connection::set_on_message_callback(std::function<void(Connection*)> const& cb) {
    on_message_callback = cb;
    std::function<void()> busi = std::bind(&Connection::business, this);
    channel->set_read_callback(busi);
}

void Connection::send_buffer_getline() {
    send_buffer->getline();
}

Socket* Connection::get_socket() {
    return sock;
}

