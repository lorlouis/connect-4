#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <io.h>
#define clrscr() system("cls")
#else

#include <sys/socket.h>
#include <unistd.h>
#endif

#include "con4.h"
#include "tui.h"
#include "net.h"

void couch_game_loop() {
    /* THE game */
    struct game the_game = {0};
    int cur_player = 1;
    while(the_game.winning_son == 0) {
        int insert_rval;
        do {
            int selected_col;
            /* draw the screen */
            if((selected_col = select_col(&the_game, cur_player)) < 0) {
                printf("Player %d, cancelled the game\n", cur_player);
                exit(EXIT_SUCCESS);
            }
            /* make sure the move is valid */
            insert_rval = insert_stuff(&the_game, cur_player, selected_col);
            if(insert_rval == -1) {
                printf("%d\n", selected_col);
                puts("Invalid placement, try another column\n"
                     "press any key to continue");
                getch();
            }
        } while(insert_rval == -1);
        cur_player = (cur_player % 2) + 1;
    }

    render_game(&the_game, -1, -1);

    if(the_game.winning_son == -1) {
        puts("Draw");
    }
    else {
        printf("Player%d won\n", the_game.winning_son);
    }
}


void client_game_loop(int sv_sock) {
    struct game the_game = {0};
    /* get game from server */
    while(1) {
        clrscr();
        puts("Waiting for Player1 to finish");
        if(recv(sv_sock, &the_game, sizeof(struct game), MSG_WAITALL) != sizeof(struct game)) {
            puts("The connection was closed by the server");
            exit(EXIT_FAILURE);
        }
        if(the_game.winning_son != 0) {
            break;
        }
        int sv_ack_move_valid=0;
        /* select column */
        do {
            int selected_col;
            /* draw the screen */
            if ((selected_col = select_col(&the_game, 2)) < 0) {
                puts("Player 2 cancelled the game");
                /* TODO tell the player on the other
                * side that the game was closed */
                /* FIXME might come back and bite me */
                close(sv_sock);
                return;
            }
            /* TODO handle error */
            if(send(sv_sock, &selected_col, sizeof(selected_col), 0) < 0) {
                perror("");
                return;
            }

            if(recv(sv_sock, &sv_ack_move_valid, sizeof(sv_ack_move_valid), MSG_WAITALL) < 0) {
                perror("");
                return;
            }
            if(sv_ack_move_valid != 0) {
                printf("%d\n", selected_col);
                puts("Invalid placement, try another column\n"
                     "press any key to continue");
                getch();
            }
        } while (sv_ack_move_valid!=0);
    }
    render_game(&the_game, -1, -1);

    if(the_game.winning_son == -1) {
        puts("Draw");
    }
    else {
        printf("Player%d won\n", the_game.winning_son);
    }
}

void server_game_loop(int p2_sock) {
    /* THE game */
    struct game the_game = {0};
    while(the_game.winning_son == 0) {
        int insert_rval;
        int selected_col;
            do {
                /* draw the screen */
                if((selected_col = select_col(&the_game, 1)) < 0) {
                    printf("Player %d, cancelled the game\n", 1);
                    /* TODO tell the player on the other
                     * side that the game was closed */
                    return;
                }
                /* make sure the move is valid */
                insert_rval = insert_stuff(&the_game, 1, selected_col);
                if(insert_rval == -1) {
                    printf("%d\n", selected_col);
                    puts("Invalid placement, try another column\n"
                         "press any key to continue");
                    getch();
                }
            } while(insert_rval == -1);
            if(the_game.winning_son != 0) break;
            clrscr();
            puts("waiting for Player2's turn to finish");
            /* send stuff to client */
            if(send(p2_sock, &the_game, sizeof(struct game), 0) < 0) {
                perror("");
                return;
            }
            do {
                int recv_val = recv(p2_sock, &selected_col, sizeof(selected_col), MSG_WAITALL);
                /* wait for client to send col num*/
                if(recv_val < 0) {
                    perror("");
                    return;
                }
                if(recv_val == 0) {
                    puts("The client closed the connection");
                    return;
                }
                /* test the placement */
                insert_rval = insert_stuff(&the_game, 2, selected_col);
                /* send if it worked or not */
                if(send(p2_sock, &insert_rval, sizeof(insert_rval), 0) < 0) {
                    perror("");
                    return;
                }
            } while(insert_rval == -1);
    }
    send(p2_sock, &the_game, sizeof(struct game), 0);
    render_game(&the_game, -1, -1);

    if(the_game.winning_son == -1) {
        puts("Draw");
    }
    else {
        printf("Player%d won\n", the_game.winning_son);
    }
}

int main(int argc, const char **argv) {
    /* args: [-s <port> | -c <ip:port>] */
    /* couch game */
    if(argc == 1) {
        do {
            couch_game_loop();
            puts("Do you want to play another game? [y/N]");
        } while(getch() == 'y');
        exit(EXIT_SUCCESS);
    }
    if(argc != 3) {
        puts("Usage: [-s <port> | -c <ip:port>]");
        exit(EXIT_FAILURE);
    }
    if(!strcmp(argv[1], "-s")) {
        int portnum = atoi(argv[2]);
        if(portnum == 0) {
            printf("%d is not a valid port number\n", portnum);
            exit(EXIT_FAILURE);
        }
        puts("Waiting for Player2 to connect");
#ifdef _WIN32
        WSADATA wsa_data;
        SOCKET p2_sock = INVALID_SOCKET;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            puts("Could not start socket");
            return EXIT_FAILURE;
    }
#else
        int p2_sock;
#endif
        p2_sock = listen_for_sock_fd(portnum);
        if(p2_sock < 0) {
            puts("An error occured when waiting for player 2");
            perror("");
            exit(EXIT_FAILURE);
        }
        server_game_loop(p2_sock);
#ifdef _WIN32
        closesocket(p2_sock);
        WSACleanup();
#else
        close(p2_sock);
#endif

    }
    else if(!strcmp(argv[1], "-c")) {

        if (strlen(argv[2]) > 20) {
            puts("A the server's ip must be 20 characters max");
            return EXIT_FAILURE;
        }
        char str[21] = {0};
        memcpy(str, argv[2], (strlen(argv[2])+1) * sizeof(char));
        /* TODO remove the malloc and use an array since the size io the input is known */
        char *ip_str, *port_str;
        int port;
        ip_str = strtok(str, ":");
        port_str = strtok(0, ":");
        printf("%s, %s\n", ip_str, port_str);
        if(ip_str == 0 || port_str == 0) {
            printf("Could not parse \"%s\"\n"
                    "Use the format \"ipv4:port\" with the -c option\n",
                    argv[2]);
            return EXIT_FAILURE;
        }
        if((port = atoi(port_str)) == 0 || port < 1024) {
            printf("%s, is not a valid port number between 1024 and 65535\n",
                   port_str);
            return EXIT_FAILURE;
        }
#ifdef _WIN32
        WSADATA wsa_data;
        SOCKET sv_sock = INVALID_SOCKET;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            puts("Could not start socket");
            return EXIT_FAILURE;
        }
#else
        int sv_sock;
#endif
        sv_sock = connect_for_sock_fd(ip_str, port);
        if(sv_sock < 0) {
            printf("Could not connect to server at %s\n", argv[2]);
            perror("");
#ifdef _WIN32
            WSACleanup();
#endif
            return EXIT_FAILURE;
        }
        client_game_loop(sv_sock);
#ifdef _WIN32
        closesocket(sv_sock);
        WSACleanup();
#else
        close(sv_sock);
#endif
    }
    else {
        printf("invalid argument %s\n", argv[1]);
        puts("Usage: [-s <port> | -c <ip:port>]");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
