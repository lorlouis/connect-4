#ifdef _WIN32

#include <winsock2.h>
#include <io.h>
#include <ws2tcpip.h>

#define SO_REUSEPORT 0

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#endif

#include <string.h>

#include <stdio.h>
#include "net.h"
#include "con4.h"

/* returns -1 on error, check perror for more info */
/* shit abi modeled after strtok so you know it is shit */
int listen_for_sock_fd(int port) {

    static int server_fd = 0xdeadbeef;
    static int lasport = 0;
    int new_sock;
    static struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if(server_fd == 0xdeadbeef && (port != 0 && port != lasport)) {

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
        if (listen(server_fd, 3) < 0)
        {
            return -1;
        }
        lasport = port;
    }
    if((new_sock = accept(server_fd, (struct sockaddr *)&address,
                          (socklen_t*)&addrlen))<0) {
        return -1;
    }
    return new_sock;
}

#ifdef _WIN32
void server_game_loop(SOCKET player_socks[2]) {
#else
void server_game_loop(int player_socks[2]) {
#endif
    /* THE game */
    struct game the_game = {0};
    the_game.current_gamer = 1;

    while(the_game.winning_son == 0) {
        struct net_dat net_dat = {0};
        for(int i=0; i<2; i++) {
            send(player_socks[i], &the_game, sizeof(struct game), 0);
        }
        do {
            int recv_val = recv(player_socks[the_game.current_gamer-1],
                                &net_dat,
                                sizeof(struct net_dat), MSG_WAITALL);
            /* wait for client to send col num*/
            if(recv_val < 0) {
                perror("");
                return;
            }
            if(recv_val == 0) {
                puts("The client closed the connection");
                return;
            }
            if(net_dat.is_push) {
                if(0 == (char)insert_stuff(
                            &the_game,
                            the_game.current_gamer,
                            net_dat.col)) {
                    net_dat.is_accept = 1;
                }
                else {
                    net_dat.is_accept = 0;
                }

            }

            /* send data to players */
            for(int i=0; i<2; i++) {
                if(send(player_socks[i], &net_dat,
                        sizeof(struct net_dat), 0) < 0) {
                    perror("");
                    return;
                }
            }
        } while(net_dat.is_accept == 0);
        the_game.current_gamer = (the_game.current_gamer % 2) + 1;
    }
    /* the game ended */
    for(int i=0; i<2; i++) {
        if(send(player_socks[i], &the_game,
                sizeof(struct game), 0) < 0) {
            perror("");
            return;
        }
    }
}
/* returns 0 when normal
 * returns < 0 on error
 * -1 could not start socket
 * -2 error when client connected */
int server_lifetime(int portnum) {
#ifdef _WIN32
        WSADATA wsa_data;
        SOCKET socks[2] = {INVALID_SOCKET, INVALID_SOCKET};
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            puts("Could not start socket");
            return -1;
    }
#else
        int socks[2] = {0};
#endif
        for(int i = 0; i < 2; i++) {
            socks[i] = listen_for_sock_fd(portnum);
            if(socks[i] < 0) {
                return -2;
            }
            if(send(socks[i], &i, sizeof(i), 0) < 0) {
                return -3;
            }
        }
        server_game_loop(socks);

        for(int i = 0; i < 2; i++) {
#ifdef _WIN32
            closesocket(socks[i]);
#else
            close(socks[i]);
#endif
        }
#ifdef _WIN32
        WSACleanup();
#endif
        return 0;
}
