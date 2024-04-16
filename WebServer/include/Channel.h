#pragma once

#include <sys/epoll.h>
#include <functional>


class EventLoop;

class Channel {
private:
    EventLoop* loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool in_epoll;

    std::function<void()> read_callback;
    std::function<void()> write_callback;
public:
    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handle_event();
    void enable_read();
    int get_fd();
    uint32_t get_events();
    uint32_t get_ready();
    bool get_in_epoll();
    void set_in_epoll(bool in_epoll=true);
    void set_ET();
    void set_ready(uint32_t);
    void set_read_callback(std::function<void()>);
};
