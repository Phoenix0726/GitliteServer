#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "Client.h"
#include "util.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Buffer.h"

#include "../../gitlite/include/Repository.h"
#include "../../gitlite/include/Utils.h"

using std::endl;
using std::ifstream;

#define BUFFER_SIZE 1024


Client::Client() {
    sock = new Socket();
    InetAddress* serv_addr = new InetAddress("127.0.0.1", 3333);
    sock->connect(serv_addr);

    send_buffer = new Buffer();
    read_buffer = new Buffer();

    delete serv_addr;
}

Client::~Client() {
    delete sock;
    delete read_buffer;
    delete send_buffer;
}

void Client::send(string str) {
    int sockfd = sock->get_fd();

    _write(str);

    char buf[BUFFER_SIZE];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            string str(buf);
            if (str.substr(0, 8) == "received") {
                if (str.size() > 8) {
                    // printf("server had received %s\n", str.substr(8).c_str());
                }
                break;
            }
        } else if (read_bytes == 0) {
            printf("server disconnected!\n");
            exit(EXIT_SUCCESS);
        }
    }
}

void Client::sendfile(string file) {
    send("file: " + file);
    ifstream fin(file);
    while (true) {
        send_buffer->getline(fin);      // 按行读取文件
        if (send_buffer->size() == 0) {     // 文件读完了
            send(":end");
            break;
        }
        send();
    }
}

void Client::clone(string path) {
    int sockfd = sock->get_fd();
    // 向服务器发送 clone 请求
    _write("clone: " + path);

    string project = getFileName(path);
    project = join(Repository::CWD, project);

    char buf[BUFFER_SIZE];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes == 0) {
            printf("server disconnected!\n");
            exit(EXIT_SUCCESS);
        }

        string str(buf);
        // printf("receive from server: %s\n", buf);
        if (str.substr(0, 5) == "file:") {
            const string filename = str.substr(6);
            // printf("filename: %s\n", filename.c_str());
            const string save_path = join(project, filename);
            mkdirOfPath(save_path);     // 如果父文件夹不存在，先创建父文件夹

            fout.open(save_path);

            _write("received");
        } else if (str == ":end") {
            fout.close();

            _write("received");
        } else if (str == ":allend") {
            break;
        } else {
            fout << str << endl;

            _write("received");
        }
    }
}

void Client::_write(string str) {
    if (str != "") {
        send_buffer->set_buf(str.c_str());
    }

    int sockfd = sock->get_fd();
    ssize_t write_bytes = write(sockfd, send_buffer->c_str(), send_buffer->size());
    if (write_bytes == -1) {
        printf("socket already disconnected, can't write any more!\n");
        return;
    }
}