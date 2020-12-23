#ifndef NET_H
#define NET_H 1

#include "con4.h"

#define PORT 8080

struct net_game {
    char current_gamer;
    struct game game;
};

int connect_for_sock_fd(char *addr, int port);

int listen_for_sock_fd(int port);

#endif
