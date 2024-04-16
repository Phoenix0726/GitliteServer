#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>


EventLoop::EventLoop() : quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> chs;
        chs = ep->poll();
        for (auto it = chs.begin(); it != chs.end(); it++) {
            (*it)->handle_event();
        }
    }
}

void EventLoop::update_channel(Channel* channel) {
    ep->update_channel(channel);
}
