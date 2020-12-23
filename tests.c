#include <assert.h>

#include "tests.h"
#include "con4.h"

//check insert if row is full
void testing_insert_stuff_1()
{
    struct game game = {0};

    for(int i = ROW_WIDTH - 1; i >= 0; --i)
    {
        game.field[i][0] = (i % 2) + 1;
    }

    assert(insert_stuff(&game, 1, 0) == -1);
}

//test out of bound
void testing_out_of_bound()
{
    struct game game = {0};

    assert(insert_stuff(&game, 1, -1) == -1);

    assert(insert_stuff(&game, 1, COL_WIDTH) == -1);
}

//test gamer validity
void testing_gamer_validity()
{
    struct game game = {0};

    assert(insert_stuff(&game, 0, 0) == -1);

    assert(insert_stuff(&game, -1, 0) == -1);

    assert(insert_stuff(&game, 18, 0) == 0);
}

void testing_max_y_value()
{
    struct game game = {0};

    assert(insert_stuff(&game, 1, COL_WIDTH - 1) == 0);
}

void testing_mid_y_value()
{
    struct game game = {0};

    assert(insert_stuff(&game, 1, COL_WIDTH / 2) == 0);
}

//testing writting capababilties of insert_stuff()
void testing_if_it_write()
{
    struct game game = {0};

    insert_stuff(&game, 1, 0);

    assert(game.field[ROW_WIDTH - 1][0] == 1);
}

//test if no winner when table empty
void testing_winning_empty_table()
{
    struct game game = {0};

    game.field[0][0] = 1;

    assert(r_ya_winning_son(&game, 0, 0, 1) == 0);
}

int main() {
    testing("Testing insert_stuff when ROW is full", testing_insert_stuff_1);

    testing("Testing insert_stuff out of bound", testing_out_of_bound);

    testing("Testing insert_stuff gamer validity", testing_gamer_validity);

    testing("Testing insert_stuff with max y value", testing_max_y_value);
    
    testing("Testing insert_stuff with a mid y value", testing_mid_y_value);

    testing("Testing insert stuff if it write", testing_if_it_write);

    testing("Testing r_ya_winning_son empty table", 
            testing_winning_empty_table);
    
    return 0;
}
