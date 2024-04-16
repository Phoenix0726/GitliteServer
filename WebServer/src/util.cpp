#include "util.h"
#include <cstdio>
#include <cstdlib>


void errif(bool cond, const char* errmsg) {
    if (cond) {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}
