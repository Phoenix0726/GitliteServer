#include "EventLoop.h"
#include "Server.h"
#include "Connection.h"
#include "Socket.h"

#include "../../gitlite/include/Utils.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <queue>

using std::string;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::queue;


void handle_request(Connection* conn);

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
    server->on_message(handle_request);
    
    loop->loop();

    delete server;
    delete loop;

    return 0;
}


void handle_request(Connection* conn) {
    const string CWD = "../htdc";

    if (conn->get_state() == Connection::State::closed) {
        conn->fout.rdbuf((std::cout).rdbuf());
        conn->_close();
        return;
    }
    
    string str(conn->read_buffer_val());

    if (str.substr(0, 9) == "username:") {
        conn->username = str.substr(10);
        // 如果用户目录不存在，给该用户创建一个目录
        string dir = join(CWD, conn->username);
        if (access(dir.c_str(), 0) == -1) {
            mkdir(dir.c_str(), S_IRWXU);
        }
        conn->_send("received");
        return;
    }
    if (str.substr(0, 8) == "project:") {
        conn->project = str.substr(9);
        // 如果项目目录不存在，给该项目创建一个目录
        if (conn->username == "") {
            Exit("User is not exists!");
        }
        string dir = join(CWD, join(conn->username, conn->project));
        if (access(dir.c_str(), 0) == -1) {
            mkdir(dir.c_str(), S_IRWXU);
        }
        conn->_send("received");
        return;
    }

    /* clone start */
    if (str.substr(0, 6) == "clone:") {
        string dir_path = str.substr(7);
        dir_path = join(CWD, dir_path);
        printf("clone project path: %s\n", dir_path.c_str());
        queue<string> fileq;
        fileq.push(dir_path);
        while (!fileq.empty()) {
            string file = fileq.front();
            fileq.pop();
            if (isDir(file)) {  // 如果是文件夹，继续展开
                vector<string> files = plainFilenamesIn(file);
                for (auto it : files) {
                    fileq.push(join(file, it));
                }
            } else {    // 发送文件
                ifstream fin(file);
                string line = "file: " + getRelativePath(dir_path, file);
                while (true) {
                    if (line == "") getline(fin, line);
                    // printf("line %s\n", line.c_str());
                    if (line.size() == 0) {     // 该文件发完了
                        line = ":end";
                    }
                    conn->_send(line.c_str());
                    while (true) {      // 等待客户端接收
                        if (conn->get_state() == Connection::State::closed) {
                            conn->_close();
                            return;
                        }
                        conn->_read();
                        string resp(conn->read_buffer_val());
                        // printf("resp: %s\n", resp.c_str());
                        if (resp == "received") {
                            break;
                        }
                    }
                    if (line == ":end") {
                        break;
                    }
                    line = "";
                }
            }
        }
        
        conn->_send(":allend");

        printf("client fd %d disconnected\n", conn->get_socket()->get_fd());
        conn->_close();
        return;
    }
    /* clone end */

    if (str.substr(0, 5) == "file:") {      // 收到文件名，把输出重定向到文件
        const string filename = str.substr(6);
        string save_path = join(join(join(CWD, conn->username), conn->project), filename);
        printf("file save path: %s\n", save_path.c_str());
        mkdirOfPath(save_path);     // 如果父文件夹不存在，先创建文件夹

        conn->file.open(save_path);
        conn->fout.rdbuf(conn->file.rdbuf());

        conn->_send("received");
    } else if (str.substr(0, 4) == ":end") {    // 该文件结束
        conn->file.close();
        conn->_send("received test.txt");
    } else {    // 把收到的内容存到文件
        conn->fout << str.c_str() << endl;

        conn->_send("received");
    }
}