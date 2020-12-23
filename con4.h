#ifndef CON4_H
#define CON4_H 1

#define ROW_WIDTH 7
#define COL_WIDTH 6

struct game {
    char field[ROW_WIDTH][COL_WIDTH];
    char winning_son;
};


/* gamer > 0
 * y >= 0
 * y <= 6
 * return -1 si pas capable d'insert  autrement 0 */
int insert_stuff(struct game *game, char gamer, int y);

/* reset the game to initial values (0s everywhere)*/
void reset_game(struct game *game);

/* check winning condition
 * 0 no one wins
 * -1 NULL game
 * >0 YA WINNING SON (return the winning gamer)*/
char r_ya_winning_son(struct game *game, int x, int y, char gamer);

#endif
