/**
 * @file ignore_assert.c
 * @brief Ignore assertion
 * @author Light-City
 * @version v1
 * @date 2019-07-25
 */

#define NDEBUG // ignore assertion

#include<assert.h>

int main(){
    int x=7;
    assert(x==5);
    return 0;
}
