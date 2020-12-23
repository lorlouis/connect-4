#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>


#define PORT 8080

/* returns -1 on error, check perror for more info */
int listen_for_sock_fd(int port) {
    int server_fd, new_sock;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if(!(server_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        return -1;
    }

    /* bind */
    if(setsockopt(
            server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
            &opt, sizeof(opt))) {
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        return -1;
    }
    if((new_sock = accept(server_fd, (struct sockaddr *)&address,
                          (socklen_t*)&addrlen))<0) {
        return -1;
    }
    return new_sock;
}
