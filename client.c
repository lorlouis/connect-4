#ifdef _WIN32
#define _WIN32_WINNT 0x0601

#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>

#else

#include <termios.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif



#include <string.h>

#include "tui.h"
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

#ifdef _WIN32
void client_game_loop(SOCKET sv_sock, char player_num) {
#else
void client_game_loop(int sv_sock, char player_num) {
#endif
    struct game the_game = {0};
    while(1) {
        clrscr();
        /* get game from server */
        if(recv(sv_sock, &the_game, sizeof(struct game), MSG_WAITALL)
                != sizeof(struct game)) {
            puts("The connection was closed by the server");
            return;
        }
        /* select column */
        struct net_dat net_dat = {0};
        net_dat.col = COL_WIDTH / 2;
        render_game(&the_game, net_dat.col);
        if(the_game.winning_son != 0) {
            break;
        }
        do {
#ifdef _WIN32
            /* TODO implement the equivalent of tcflush for windows */
#else
            tcflush(stdin->_fileno, TCIFLUSH);
#endif
            if(the_game.current_gamer != player_num) {
                if(recv(sv_sock, &net_dat, sizeof(struct net_dat), MSG_WAITALL)
                        != sizeof(struct net_dat)) {
                    perror("");
                    return;
                }
                if(net_dat.col >= 0) {
                    render_game(&the_game, net_dat.col);
                }
            }
            else {
                if (select_col_net(&(the_game), &net_dat) < 0) {
                    /* TODO tell the player on the other
                    * side that the game was closed */
                    perror("The server closed the connection");
                    return;
                }
                if(send(sv_sock, &net_dat, sizeof(struct net_dat), 0) < 0) {
                    perror("");
                    return;
                }
                if(recv(sv_sock, &net_dat, sizeof(struct net_dat), MSG_WAITALL)
                        != sizeof(struct net_dat)) {
                    perror("");
                    return;
                }

                if(net_dat.is_push && net_dat.is_accept==0) {
                    net_dat.is_push = 0;
                    printf("%d\n", net_dat.col);
                    puts("Invalid placement, try another column\n"
                         "press any key to continue");
                    getch();
                }
            }
        } while (net_dat.is_accept==0);
    }
    render_game(&the_game, -1);

    if(the_game.winning_son == -1) {
        puts("Draw");
    }
    else {
        printf("Player%d won\n", the_game.winning_son);
    }
}

/* returns 0 in normal cases
 * returns < 0 on error
 * -1 could not start socket
 * -2 could not connect to server
 * -3 could not contact the server
 * -4 malformed ip packet */
int client_lifetime(char *ip_str, int port) {

#ifdef _WIN32
        WSADATA wsa_data;
        SOCKET sv_sock = INVALID_SOCKET;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            return -1;
        }
#else
        int sv_sock;
#endif
        sv_sock = connect_for_sock_fd(ip_str, port);
        if(sv_sock < 0) {
#ifdef _WIN32
            WSACleanup();
#endif
            return -2;
        }
        int player_num;
        if(recv(sv_sock, &player_num, sizeof(player_num), MSG_WAITALL) < 0) {
#ifdef _WIN32
            WSACleanup();
#endif
            return -3;
        }
        client_game_loop(sv_sock, player_num+1);
#ifdef _WIN32
        closesocket(sv_sock);
        WSACleanup();
#else
        close(sv_sock);
#endif
        return 0;
}
