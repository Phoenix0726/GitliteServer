#pragma once

#include <functional>


class EventLoop;
class Socket;
class Channel;

class Acceptor {
private:
    EventLoop* loop;
    Socket* sock;
    Channel* accept_channel;
    std::function<void(Socket*)> new_connection_callback;
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();
    void accept_connection();
    void set_new_connection_callback(std::function<void(Socket*)>);
};
