#include "Channel.h"
#include "EventLoop.h"

#include <unistd.h>


Channel::Channel(EventLoop* loop, int fd) : loop(loop), fd(fd), events(0), ready(0), in_epoll(false) {}

Channel::~Channel() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Channel::handle_event() {
    if (ready & (EPOLLIN | EPOLLPRI)) {
        read_callback();
    }
    if (ready & (EPOLLOUT)) {
        write_callback();
    }
}

void Channel::enable_read() {
    events |= EPOLLIN | EPOLLPRI;
    loop->update_channel(this);
}

void Channel::set_ET() {
    events |= EPOLLET;
    loop->update_channel(this);
}

int Channel::get_fd() {
    return fd;
}

uint32_t Channel::get_events() {
    return events;
}

uint32_t Channel::get_ready() {
    return ready;
}

bool Channel::get_in_epoll() {
    return in_epoll;
}

void Channel::set_in_epoll(bool in_epoll) {
    this->in_epoll = in_epoll;
}

void Channel::set_ready(uint32_t ready) {
    this->ready = ready;
}

void Channel::set_read_callback(std::function<void()> cb) {
    read_callback = cb;
}
