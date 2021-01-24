#ifndef NET_H
#define NET_H 1

#include "con4.h"

#define PORT 8080

struct net_dat {
    int col;
    int is_push;
    int is_accept;
};

int connect_for_sock_fd(char *addr, int port);

int listen_for_sock_fd(int port);

int client_lifetime(char *ip_str, int port);

#ifdef _WIN32
void client_game_loop(SOCKET sv_sock, char player_num);
#else
void client_game_loop(int sv_sock, char player_num);
#endif

#ifdef _WIN32
void server_game_loop(SOCKET player_socks[2]);
#else
void server_game_loop(int player_socks[2]);
#endif

int server_lifetime(int portnum);

#endif
