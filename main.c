#include <stdio.h>
#include <stdlib.h>

#include "con4.h"
#include "tui.h"

int main(int argc, const char **argv) {
    /* THE game */
    struct game the_game = {0};
    int cur_player = 1;
    /* TEMPORARY FIXME */
    while(the_game.winning_son == 0 || 1) {
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
