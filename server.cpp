//
// Created by alexander on 5/18/23.
//

#include "server.h"


int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof (val));

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0);
    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));

    if (rv) {
        std::cout << "bind()" << std::endl;
        exit(1);
    }

    rv = listen(fd, SOMAXCONN);
    if (rv) {
        std::cout << "listen()" << std::endl;
        exit(1);
    }

    while (true) {
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof (client_addr);

        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0) {
            continue;
        }

        while (true) {
            int32_t err = one_request(connfd);
            if (err) {
                break;
            }
        }
        close(connfd);
    }

    return 0;
}

static int32_t read_full(int fd, char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = read(fd, buf, n);
        if (rv <= 0) {
            return -1;
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

static int32_t write_all(int fd, const char *buf, size_t n) {
    while (n > 0){
        ssize_t rv = write(fd, buf, n);
        if (rv <= 0) {
            return -1;
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}


const size_t k_max_msg = 4096;

static int32_t one_request(int connfd) {
    char rbuf[4+ k_max_msg + 1];
    errno = 0;
    int32_t err = read_full(connfd, rbuf, 4);

    if (err) {
        if (errno == 0) {
            //msg("EOF");
        }
    }
}


static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) -1);
    if (n < 0) {
        //msg("read() error");
        std::cout << "read() error" << std::endl;
        return;
    }
    std::cout << "client says: " << rbuf << std::endl;

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}