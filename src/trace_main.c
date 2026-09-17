#include "../include/trace.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t cap = 4096, len = 0;
    char *buf = malloc(cap);
    int c;
    while ((c = fgetc(stdin)) != EOF) {
        if (len + 1 >= cap) buf = realloc(buf, cap *= 2);
        buf[len++] = (char)c;
    }
    buf[len] = '\0';
    int rc = run_trace(buf);
    free(buf);
    return rc;
}
