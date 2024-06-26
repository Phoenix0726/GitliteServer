#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <sys/epoll.h>
#include <vector>


class Channel;

class Epoll {
private:
    int epfd;
    struct epoll_event* events;
public:
    Epoll();
    ~Epoll();

    void update_channel(Channel*);
    void delete_channel(Channel*);
    std::vector<Channel*> poll(int timeout = -1);
};


#endif