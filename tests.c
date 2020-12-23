#include <assert.h>

#include "tests.h"

void test_method() {
    assert(1 == 1);
}

void test_method2() {
    assert(1 == 2);
}


int main() {
    testing("Testing 1==1", test_method);

    testing("Testing 1==2", test_method2);
    return 0;
}
