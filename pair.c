#include "pair.h"

#include <stdlib.h>     // malloc(), free()
#include <string.h>     // memcpy()

pair *pair_init() {
    struct pair_t *p = malloc(sizeof(*p));
    p->key = p->val = NULL;
    return p;
}

void pair_del(pair **p) {
    if (*p != NULL) {
        free((*p)->key);
        free((*p)->val);
        free(*p);
        *p = NULL;
    } 
}

