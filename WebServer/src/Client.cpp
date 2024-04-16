#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>

#include "Client.h"
#include "util.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Buffer.h"

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
    if (str != "") {
        send_buffer->set_buf(str.c_str());
    }
    
    int sockfd = sock->get_fd();
    ssize_t write_bytes = write(sockfd, send_buffer->c_str(), send_buffer->size());
    if (write_bytes == -1) {
        printf("socket already disconnected, can't write any more!\n");
        return;
    }

    int has_read = 0;
    char buf[BUFFER_SIZE];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            read_buffer->append(buf, read_bytes);
            has_read += read_bytes;
        } else if (read_bytes == 0) {
            printf("server disconnected!\n");
            exit(EXIT_SUCCESS);
        }

        if (has_read >= send_buffer->size()) {
            printf("message from server: %s\n", read_buffer->c_str());
            break;
        }
    }
    read_buffer->clear();
}

void Client::sendfile(istream& fin) {
    while (true) {
        send_buffer->getline(fin);
        if (send_buffer->size() == 0) {     // 文件读完了
            break;
        }
        send();
    }
}