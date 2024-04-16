#pragma once

#include <functional>
#include <fstream>
#include <iostream>

using std::ostream;
using std::ofstream;
using std::cout;


class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection {
public:
    enum State {
        invalid = 1,
        handshaking,
        connected,
        closed,
        failed,
    };
private:
    EventLoop* loop;
    Socket* sock;
    Channel* channel;
    State state;
    Buffer* read_buffer;
    Buffer* send_buffer;

    

    std::function<void(Socket*)> delete_connection_callback;
    std::function<void(Connection*)> on_connect_callback;
    std::function<void(Connection*)> on_message_callback;

    void readnonblocking();
    void writenonblocking();
    void readblocking();
    void writeblocking();
public:
    ostream& fout = std::cout;
    ofstream file;

    Connection(EventLoop* loop, Socket* sock);
    ~Connection();

    void _read();
    void _write();
    void _send(const char* msg);

    void set_delete_connection_callback(std::function<void(Socket*)> const& cb);
    void set_on_connect_callback(std::function<void(Connection*)> const& cb);
    void set_on_message_callback(std::function<void(Connection*)> const& cb);

    State get_state();
    void business();
    void _close();
    void set_send_buffer(const char* str);
    Buffer* get_send_buffer();
    Buffer* get_read_buffer();
    const char* read_buffer_val();
    const char* send_buffer_val();
    void send_buffer_getline();
    Socket* get_socket();
};
