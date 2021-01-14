#include <stdio.h>

#ifdef _WIN32
#include <io.h>
#include <conio.h>

#define clrscr() system("cls")

#else
#include <unistd.h>
#include <termios.h>

static struct termios old, new;

static void initTermios(int echo) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* apply terminal io settings */
}

/* Restore old terminal i/o settings */
static void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

void clrscr(void) {
    printf("\e[1;1H\e[2J");
}

/* grabs keyboard keypress */
char getch() {
    char ch;
    initTermios(0);
    ch = getchar();
    resetTermios();
    return ch;
}

#endif

#include "con4.h"
#include "tui.h"
#include "net.h"

int select_col_net(struct game *game, struct net_dat *net_dat) {
    char c;
    render_game(game, net_dat->col);
    c = getch();
    switch(c) {
        case 'h':
        case 'a':
            if(net_dat->col > 0) net_dat->col--;
            break;
        case 'l':
        case 'd':
            if(net_dat->col < COL_WIDTH-1) net_dat->col++;
            break;
        case 'q':
            return -1;
        case ' ':
            net_dat->is_push = 1;
    }
    return 0;
}

int select_col(struct game *game) {
    struct net_dat net_dat = {0};
    net_dat.col = COL_WIDTH / 2;
    while (net_dat.is_push == 0) {
        if(select_col_net(game, &net_dat) == -1) {
            return -1;
        }
    }
    return net_dat.col;
}

/* selected col is 0 indexed
 * if selected col is < 0 the selector isn't rendered */
void render_game(struct game *game, int selected_col) {
    clrscr();
    printf("Player%d's turn\n", game->current_gamer);
    if(selected_col >= 0) {
        for(int i = 0; i < selected_col; i++) printf("   ");
        printf(" %s\n", (game->current_gamer-1 ? PLAYER_O : PLAYER_X));
        for(int i = 0; i < selected_col; i++) printf("   ");
        puts(" \\/");
    }

    for(int y = 0; y < ROW_WIDTH; y++) {
        putchar('|');
        for(int x = 0; x < COL_WIDTH; x++) {
            switch(game->field[y][x]) {
                case(0):
                    printf("%s", EMPTY_STR);
                    break;
                case(1):
                    printf("%s", PLAYER_X);
                    break;
                case(2):
                    printf("%s", PLAYER_O);
                    break;
            }
            putchar('|');
        }
        putchar('\n');
    }
}
