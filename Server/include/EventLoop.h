#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <functional>


class Epoll;
class Channel;
class ThreadPool;

class EventLoop {
private:
    Epoll* ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void update_channel(Channel*);
};


#endif