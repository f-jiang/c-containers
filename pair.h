#ifndef PAIR_H
#define PAIR_H

#include <stddef.h>     // size_t

typedef struct pair_t {
    void *key, *val;
} pair;

pair *pair_init();
void pair_del(pair **p);

#endif

