#include "AlgoLib.h"
void alogfun(char *p, size_t size) {
    int i = 0;
    for (;i < 100;i++) {
        char *p = (char *)CMemAlloc(2 * i + 1);
        if (i % 2) {
            CMemFree(p);
        }
    }
}