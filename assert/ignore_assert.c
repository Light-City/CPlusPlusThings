/**
 * @file ignore_assert.c
 * @brief 忽略断言
 * @author 光城
 * @version v1
 * @date 2019-07-25
 */

# define NDEBUG

#include<assert.h>



int main(){
    int x=7;
    assert(x==5);
    return 0;
}
