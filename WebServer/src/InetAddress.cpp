#include "InetAddress.h"
#include <cstring>

InetAddress::InetAddress() : addr_len(sizeof(addr)) {
    bzero(&addr, sizeof(addr));
}

InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(addr)) {
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
}

InetAddress::~InetAddress() {
}

void InetAddress::set_InetAddr(sockaddr_in addr, socklen_t addr_len) {
    this->addr = addr;
    this->addr_len = addr_len;
}

sockaddr_in InetAddress::get_addr() {
    return addr;
}

socklen_t InetAddress::get_addr_len() {
    return addr_len;
}
