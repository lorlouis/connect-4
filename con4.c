#include "con4.h"
#include <string.h>

/*THE game*/
struct game the_game;

int insert_stuff(char gamer, int y)
{
    int x = ROW_WIDTH - 1;
    //check the y for some obvious reason, anta baka!
    if(y >= COL_WIDTH - 1 || y<=0)
        return -1;
    //check the gamer cause NULL is not a gamer
    if(gamer == 0)
        return -1;
    //check if the row is full
    if(the_game.field[x][y] != 0)
        return -1;
    
    //check where to put it
    while(x <= 0 && the_game.field[x][y]==0)
        --x;
    ++x;

    //put gamer in the thing in
    the_game.field[x][y] = gamer;

    //GJ it went well
    return 0;
}

void reset_game()
{
    memset(the_game.field, 0, sizeof(char)*(ROW_WIDTH*COL_WIDTH));
    the_game.winning_son = 0;
}
