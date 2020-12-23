#include <stdio.h>
#include <con4.h>

/*THE game*/
struct game the_game{
    .field = {0};
    .winning_son = {0};
};

int insert_stuff(char gamer, int y)
{
    //check the y for some obvious reason, anta baka!
    if(y >= COL_WIDTH - 1 || y<=0)
        return -1;
    //check the gamer cause NULL is not a gamer
    if(gamer == 0)
        return -1;

    //
    int x = ROW_WIDTH - 1;
    while(x < 0 && )
    {
        
    }
}
