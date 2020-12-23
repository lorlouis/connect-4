#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>

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
    /* get game from server */

    /* select column */

}

void server_game_loop(int p2_sock) {
    /* THE game */
    struct game the_game = {0};
    int cur_player = 1;
    while(the_game.winning_son == 0) {
        int insert_rval;
        if(cur_player == 1) {
            do {
                int selected_col;
                /* draw the screen */
                if((selected_col = select_col(&the_game, cur_player)) < 0) {
                    printf("Player %d, cancelled the game\n", cur_player);
                    /* TODO tell the player on the other
                     * side that the game was closed */
                    return;
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
        }
        else {
            puts("waiting for Player2's turn to finish");
            struct net_game net_game = {0};
            /* TODO do the networking */
            net_game.current_gamer = cur_player;
            memcpy(&net_game.game, &the_game, sizeof(struct game));
            /* send stuff to client */
            send(p2_sock, &net_game, sizeof(struct net_game), 0);
            /* listen for client */
            puts("waiting for Player2's turn to finish");

            /* TODO READ SELECTED COL */

        }
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
        puts("args: [-s <port> | -c <ip:port>]");
        exit(EXIT_FAILURE);
    }
    if(!strcmp(argv[1], "-s")) {
        int portnum = atoi(argv[2]);
        if(portnum == 0) {
            printf("%d is not a valid port number\n", portnum);
            exit(EXIT_FAILURE);
        }
        puts("Waiting for Player2 to connect");
        int p2_sock = listen_for_sock_fd(portnum);
        if(p2_sock == -1) {
            puts("An error occured when waiting for player 2");
            exit(EXIT_FAILURE);
        }
        server_game_loop(p2_sock);
        close(p2_sock);
    }
}
