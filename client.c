#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "net.h"
#include "con4.h"

/* Returns:
 * -1 on general error,
 * -2 on connection error,
 *  a socket fd otherwise
 * check perror for more info */
int connect_for_sock_fd(char *addr, int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    /* check for invalid address */
    if(inet_pton(AF_INET, addr, &serv_addr.sin_addr) <= 0) {
        return -1;
    }

    /* try and connect */
    if(connect(sock, (struct sockaddr*)&serv_addr,
               sizeof(serv_addr)) < 0) {
        return -2;
    }
    return sock;
}

int update_game_from_server(struct game *game, int sock_fd) {
    size_t read_val = 0;
    struct net_game net_game = {0};
    read_val = read(sock_fd, &net_game, sizeof(net_game));
}
