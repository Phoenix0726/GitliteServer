#include "EventLoop.h"
#include "Server.h"
#include "Connection.h"
#include "Socket.h"
#include <iostream>
#include <string>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::ofstream;


void receive(Connection* conn);

int main()
{
    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop);

    server->new_connect([](Connection* conn) {
        // std::cout << "New connection fd: " << conn->get_socket()->get_fd() << std::endl;
    });

    // server->on_message([](Connection* conn){
    //     if (conn->get_state() == Connection::State::closed) {
    //         conn->_close();
    //         return;
    //     }
    //     std::cout << "Message from client " << conn->get_socket()->get_fd() << ": "  << conn->read_buffer_val() << std::endl;
    //     conn->_send(conn->read_buffer_val());
    // });
    server->on_message(receive);
    
    loop->loop();

    delete server;
    delete loop;

    return 0;
}


void receive(Connection* conn) {
    if (conn->get_state() == Connection::State::closed) {
        conn->fout.rdbuf((std::cout).rdbuf());
        conn->_close();
        return;
    }
    // conn->fout << "Message from client " << conn->get_socket()->get_fd() << ": " << conn->read_buffer_val() << endl;
    // conn->_send(conn->read_buffer_val());
    string str(conn->read_buffer_val());
    // conn->_send(str.c_str());
    if (str.substr(0, 5) == "file:") {
        const string filename = str.substr(6);
        conn->file.open(filename);
        conn->fout.rdbuf(conn->file.rdbuf());
    } else if (str.substr(0, 4) == ":end") {
        conn->_send("received test.txt");
    } else {
        conn->fout << str.c_str() << endl;
    }
}