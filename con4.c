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

    //check winning condition
    the_game.winning_son = r_ya_winning_son(x,y, gamer);
    
    //GJ it went well
    return 0;
}

void reset_game()
{
    memset(the_game.field, 0, sizeof(char)*(ROW_WIDTH*COL_WIDTH));
    the_game.winning_son = 0;
}

char r_ya_winning_son(int x, int y, char gamer)
{
    int nb_connected = 1;
    
    //horizontal
    {
        int pointing_y = y - 1;
        while(pointing_y >= 0 && nb_connected < 4 
                && the_game.field[x][pointing_y] == gamer)
        {
            --pointing_y;
            ++nb_connected;
        }
        pointing_y = y + 1;
        while(pointing_y < COL_WIDTH && nb_connected < 4 
                && the_game.field[x][pointing_y] == gamer)
        {
            ++pointing_y;
            ++nb_connected;
        }
    }
    if(nb_connected >= 4)
        return gamer;
    nb_connected = 1;

    //vertical
    {
        int pointing_x = x - 1;
        while(pointing_x >=0 && nb_connected < 4
                && the_game.field[pointing_x][y] == gamer)
        {
            --pointing_x;
            ++nb_connected;
        }
    }
    if(nb_connected >= 4)
        return gamer;
    nb_connected = 1;

    //diagonalluuu
    {
        int pointing_y = y - 1;
        int pointing_x = x - 1;
        while(pointing_x >= 0 && pointing_y >= 0 && nb_connected < 4
                && the_game.field[pointing_x][pointing_y] == gamer)
        {
            --pointing_y;
            --pointing_x;
            ++nb_connected;
        }

        pointing_y = y + 1;
        pointing_x = x + 1;
        while(pointing_x < ROW_WIDTH 
                && pointing_y < COL_WIDTH 
                && nb_connected < 4 
                && the_game.field[pointing_x][pointing_y] == gamer)
        {
            ++pointing_y;
            ++pointing_x;
            ++nb_connected;
        }
        if(nb_connected == 4)
            return gamer;
        nb_connected = 1;


        pointing_y = y + 1;
        pointing_x = x - 1;
        while(pointing_x >= 0 && pointing_y < COL_WIDTH && nb_connected < 4
                && the_game.field[pointing_x][pointing_y] == gamer)
        {
            ++pointing_y;
            --pointing_x;
            ++nb_connected;
        }

        pointing_y = y - 1;
        pointing_x = x + 1;
        while(pointing_y >= 0 && pointing_x < ROW_WIDTH && nb_connected < 4
                && the_game.field[pointing_x][pointing_y] == gamer)
        {
            ++pointing_x;
            --pointing_y;
            ++nb_connected;
        }
    }
    if(nb_connected <= 4)
        return gamer;

    //check NULL matches
    {
        char null_check = 1;
        int i = 0;
        while(null_check == 1 && i < COL_WIDTH)
        {
            if(the_game.field[ROW_WIDTH-1][i] == 0)
                null_check = 0;
            ++i;
        }

        if(null_check == 1)
            return -1;
    }

    //THE GAME CONTINUES
    //SHINJI GET IN THE ROBOT
    return 0;
}
