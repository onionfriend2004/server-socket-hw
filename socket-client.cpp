#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#define PORT 8080

int create_connection(char* node, char* service) {
    struct addrinfo* res = NULL;
    int gai_err;
    struct addrinfo hint = {};
    hint.ai_family = AF_UNSPEC;    
    hint.ai_socktype = SOCK_STREAM; 

    if (gai_err = getaddrinfo(node, service, &hint, &res)) {
        std::cerr << "gai error: " << gai_strerror(gai_err) << std::endl;
        return 1;
    }
    int sock = -1;
    for (addrinfo *ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, 0);
        if (sock < 0) {
            std::cerr << "socket" << std::endl;
            continue;
        }
        if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            std::cerr << "connect" << std::endl;
            close(sock);
            sock = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    return sock;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " NODE SERVICE" << std::endl;
        return 1;
    }
    int sock = create_connection(argv[1], argv[2]);
    if (sock < 0) {
        return 1;
    }
    char buf[1024] = {0};
    if (read(sock, &buf, sizeof(buf) - 1) > 0) {
        if (strcmp(buf, "OK\n") == 0) {
            std::cout << "Received OK" << std::endl;
        } 
        else {
            std::cout << "Did not receive OK. Received: " << buf << std::endl;
        }
    }
    close(sock);
    return 0;
}