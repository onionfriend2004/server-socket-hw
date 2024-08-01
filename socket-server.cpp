#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int create_listener(char* service) {
    struct addrinfo* res = NULL;
    int gai_err;
    struct addrinfo hint = {};
    hint.ai_family = AF_INET6;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    
    if ((gai_err = getaddrinfo(NULL, service, &hint, &res))) {
        std::cerr << "gai error: " << gai_strerror(gai_err) << std::endl;
        return -1;
    }
    int sock = -1;
    for (addrinfo *ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, 0);
        if (sock < 0) {
            std::cerr << "socket" << std::endl;
            continue;
        }
        int one = 1;
        if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            std::cerr << "socket" << std::endl;
            close(sock);
            sock = -1;
            continue;
        }
        if (listen(sock, SOMAXCONN) < 0) {
            std::cerr << "listen" << std::endl;
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
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " SERVICE" << std::endl;
        return 1;
    }
    int sock = create_listener(argv[1]);
    if (sock < 0) {
        return 1;
    }   
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int connection = accept(sock, (struct sockaddr*)&address, &addrlen); 
    char buf[512] = {0};
    inet_ntop(address.sin_family, &address.sin_addr, buf, sizeof(buf));
    const char* msg = "OK\n";
    write(connection, msg, strlen(msg));
    close(sock);
    return 0;
}