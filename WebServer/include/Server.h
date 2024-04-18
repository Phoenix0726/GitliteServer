#ifndef _SERVER_H_
#define _SERVER_H_

#include <map>
#include <vector>
#include <functional>


class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;

class Server {
private:
    EventLoop* main_reactor;
    std::vector<EventLoop*> sub_reactors;
    Acceptor* acceptor;
    std::map<int, Connection*> connections;
    ThreadPool* thread_pool;
    
    std::function<void(Connection*)> on_connect_callback;
    std::function<void(Connection*)> on_message_callback;
    std::function<void(Connection*)> new_connect_callback;
public:
    Server(EventLoop*);
    ~Server();

    void new_connection(Socket* sock);
    void delete_connection(Socket* sock);
    void on_connect(std::function<void(Connection*)> fn);
    void on_message(std::function<void(Connection*)> fn);
    void new_connect(std::function<void(Connection*)> fn);
};


#endif