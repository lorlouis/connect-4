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
int insert_stuff(int gamer, int y);

#endif
