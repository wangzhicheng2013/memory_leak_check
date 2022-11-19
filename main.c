#include <string.h>
#include "AlgoLib.h"
int main() {
    init_memory_check(1);
    size_t leaked_size = 0;
    char buf[64] = "hello alogo!";
    alogfun(buf, strlen(buf));
    check_memory_leak(&leaked_size);
    uninit_memory_check();
    printf("leaked size = %zd\n", leaked_size);

    return 0;
}