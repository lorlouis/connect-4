#include <stdio.h>

#include "con4.h"

#define EMPTY_STR "  "
#define PLAYER_X "🮥🮤"
#define PLAYER_O "()"


/* selected col is 0 indexed
 * if selected col is < 0 the selector isn't rendered */
void render_game(struct game *game, int selected_col) {
    if(selected_col >= 0) {
        for(int i = 0; i < selected_col; i++) printf("   ");
        puts(" ╲╱");
    }

    for(int y = 0; y < COL_WIDTH; y++) {
        putchar('|');
        for(int x = 0; x < ROW_WIDTH; x++) {
            switch(game->field[x][y]) {
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
