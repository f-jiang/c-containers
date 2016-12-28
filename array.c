#include "array.h"

#include <stdlib.h>     // malloc(), free()
#include <string.h>     // memcpy()

struct array_t {
    size_t len;
    size_t elem_size;
    void *data;
};

array *array_init(size_t elem_size, size_t len) {
    struct array_t *a = malloc(sizeof(*a));
    a->len = len;
    a->elem_size = elem_size;
    a->data = malloc(a->len * elem_size);
    return a;
}

void array_del(array **a) {
    free((*a)->data);
    free(*a);
    *a = NULL;
}

size_t array_get_len(const array * const a) {
    return a->len;
}

void array_set(array * const a, size_t index, void *val) {
    memcpy(array_get(a, index), val, a->elem_size);
}

void *array_get(const array * const a, size_t index) {
    if (index < a->len) {
        return a->data + a->elem_size * index;
    } else {
        return NULL;
    }
}

void array_fill(array * const a, void *val) {
    void *p = a->data;
    void *end = a->data + a->elem_size * a->len;
    while (p != end) {
        memcpy(p, val, a->elem_size);
        p += a->elem_size;
    }
}

