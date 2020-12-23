#include <assert.h>

#include "tests.h"
#include "con4.h"

//check insert if row is full
void testing_insert_stuff_1()
{
    struct game game = {0};

    for(int i = 0; i < ROW_WIDTH; ++i)
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

    assert(insert_stuff(&game, 1, 0) == 0);
}

int main() {
    testing("Testing insert_stuff when ROW is full", testing_insert_stuff_1);

    testing("Testing insert_stuff out of bound", testing_out_of_bound);

    testing("Testing insert_stuff gamer validity", testing_gamer_validity);
    return 0;
}
