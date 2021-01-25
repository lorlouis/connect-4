#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#include <io.h>
#include <Windows.h>
#else

#include <unistd.h>
#include <signal.h>

#endif

#include "con4.h"
#include "tui.h"
#include "net.h"

void couch_game_loop() {
    /* THE game */
    struct game the_game = {0};
    the_game.current_gamer = 1;
    while(the_game.winning_son == 0) {
        int insert_rval;
        do {
            int selected_col;
            /* draw the screen */
            if((selected_col = select_col(&the_game)) < 0) {
                printf("Player %d, cancelled the game\n", the_game.current_gamer);
                return;
            }
            /* make sure the move is valid */
            insert_rval = insert_stuff(&the_game, the_game.current_gamer, selected_col);
            if(insert_rval == -1) {
                printf("%d\n", selected_col);
                puts("Invalid placement, try another column\n"
                     "press any key to continue");
                getch();
            }
        } while(insert_rval == -1);
        the_game.current_gamer = (the_game.current_gamer % 2) + 1;
    }

    render_game(&the_game, -1);

    if(the_game.winning_son == -1) {
        puts("Draw");
    }
    else {
        printf("Player%d won\n", the_game.winning_son);
    }
}

#ifdef _WIN32
void win32_create_thread_client_lifetime_wrapper(LPVOID portnum) {

    Sleep(500);
    switch (client_lifetime("127.0.0.1", (int)portnum)) {
        case(-1):
        perror("Could not start socket");
        break;
        case(-2):
        perror("Could not connect to local server");
        break;
        case(-3):
        perror("an error occured when contacting the server");
        break;
    }
}
#endif

int main(int argc, const char **argv) {
    /* args: [-s <port> | -c <ip:port>] */
    /* couch game */
    if(argc == 1) {
        do {
            couch_game_loop();
            puts("Do you want to play another game? [y/N]");
        } while(getch() == 'y');
        return EXIT_SUCCESS;
    }
    if(argc != 3) {
        puts("Usage: [-s <port> | -c <ip:port>]");
        return EXIT_FAILURE;
    }
    if(!strcmp(argv[1], "-s") || !strcmp(argv[1], "-d")) {
        int portnum = atoi(argv[2]);
        if(portnum == 0) {
            printf("%d is not a valid port number\n", portnum);
            return EXIT_FAILURE;
        }
        puts("Waiting for Players to connect");
#ifdef _WIN32
        /* win32 thread stuff */
        HANDLE client_thread;
        DWORD thread_id;
        if (!strcmp(argv[1], "-s")) {
            client_thread = CreateThread(
                NULL,
                0,
                win32_create_thread_client_lifetime_wrapper,
                portnum,
                0,
                &thread_id
            );

            if (thread_id == 0) {
                perror("could not start client");
                return EXIT_FAILURE;
            }
        }

        switch(server_lifetime(portnum)) {
            case(-1):
            perror("Could not start socket");
            break;
            case(-2):
            perror("An error occured when a player connected");
            break;
            case(-3):
            perror("An error occured when contacting a player");
            break;
        }
        if (!strcmp(argv[1], "-s")) {
            TerminateThread(thread_id, 0);
        }
#else
        /* fork magic */
        /* FIXME *nix only */
        int i = 1;
        if(!strcmp(argv[1], "-s")) {
            if((i = fork()) < 0) {
                perror("Could not fork the process");
                return EXIT_FAILURE;
            }
        }
        if(i > 0) {
            /* parent */
            switch(server_lifetime(portnum)) {
                case(-1):
                perror("Could not start socket");
                break;
                case(-2):
                perror("An error occured when a player connected");
                break;
                case(-3):
                perror("An error occured when contacting a player");
                break;
            }
            kill(i, SIGTERM);
        }
        else {
            /* child */
            switch(client_lifetime("127.0.0.1", portnum)) {
                case(-1):
                perror("Could not start socket");
                break;
                case(-2):
                printf("Could not connect to server at %s\n", argv[2]);
                perror("");
                break;
                case(-3):
                perror("an error occured when contacting the server");
                break;
            }
        }
#endif
    }
    /* code to start a server */
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
        switch(client_lifetime(ip_str, port)) {
            case(-1):
            perror("Could not start socket");
            break;
            case(-2):
            printf("Could not connect to server at %s\n", argv[2]);
            perror("");
            break;
            case(-3):
            perror("an error occured when contacting the server");
            break;
        }
    }
    else {
        printf("invalid argument %s\n", argv[1]);
        puts("Usage: [-s <port> | -c <ip:port>]");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
