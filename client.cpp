//
// Created by alexander on 5/18/23.
//

#include "client.h"
int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        std::cout << "socket()" << std::endl;
        exit(1);
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        std::cout << "connect" << std::endl;
        exit(1);
    }

    char msg[] = "hello";
    write(fd, msg, strlen(msg));

    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        std::cout << "read" << std::endl;
        exit(1);
    }

    std::cout << "server says: " << rbuf << std::endl;
    close(fd);
}