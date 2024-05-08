#include "Epoll.h"
#include "Channel.h"
#include "util.h"
#include <unistd.h>
#include <cstring>

#define MAX_EVENTS 1024


Epoll::Epoll() {
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}

void Epoll::update_channel(Channel* channel) {
    int fd = channel->get_fd();

    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->get_events();
    
    if (!channel->get_in_epoll()) {
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->set_in_epoll();
    } else {
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

void Epoll::delete_channel(Channel* channel) {
    int fd = channel->get_fd();
    errif(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr) == -1, "epoll delete error");
    channel->set_in_epoll(false);
}

std::vector<Channel*> Epoll::poll(int timeout) {
    std::vector<Channel*> active_channels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for (int i = 0; i < nfds; i++) {
        Channel* channel = (Channel*)events[i].data.ptr;
        channel->set_ready(events[i].events);
        active_channels.push_back(channel);
    }
    return active_channels;
}
