#include <iostream>
#include <unistd.h>
#include <cstring>
#include <functional>
#include "util.h"
#include "Buffer.h"
#include "InetAddress.h"
#include "Socket.h"
#include "ThreadPool.h"

using namespace std;


void client(int msgs, int wait);


int main(int argc, char** argv) {
    int threads = 2;
    int msgs = 10;
    int wait = 0;

    int o;
    const char* optstring = "t:m:w:";
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 't':
                threads = stoi(optarg);
                break;
            case 'm':
                msgs = stoi(optarg);
                break;
            case 'w':
                wait = stoi(optarg);
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %d\n", opterr);
                break;
        }
    }

    ThreadPool* pool = new ThreadPool(threads);
    std::function<void()> func = std::bind(client, msgs, wait);
    for (int i = 0; i < threads; i++) {
        pool->add(func);
    }
    
    delete pool;

    return 0;
}


void client(int msgs, int wait) {
    Socket* sock = new Socket();
    InetAddress* serv_addr = new InetAddress("127.0.0.1", 3333);
    sock->connect(serv_addr);

    int sockfd = sock->get_fd();

    Buffer* send_buffer = new Buffer();
    Buffer* read_buffer = new Buffer();

    sleep(wait);

    int cnt = 0;
    while (cnt < msgs) {
        send_buffer->set_buf("can can need");
        ssize_t write_bytes = write(sockfd, send_buffer->c_str(), send_buffer->size());
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }

        int has_read = 0;
        char buf[1024];
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
                printf("count: %d, message from server: %s\n", cnt++, read_buffer->c_str());
                break;
            }
        }
        read_buffer->clear();
    }

    delete serv_addr;
    delete sock;
}
