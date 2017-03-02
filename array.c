#include "array.h"

#include <stdlib.h>     // malloc(), free()
#include <string.h>     // memcpy()

struct array_t {
    size_t size;
    size_t elem_size;
    void *data;
};

array *array_init(size_t elem_size, size_t size) {
    struct array_t *a = malloc(sizeof(*a));
    a->size = size;
    a->elem_size = elem_size;
    a->data = malloc(a->size * elem_size);
    return a;
}

void array_del(array **a) {
    if (*a != NULL) {
        free((*a)->data);
        free(*a);
        *a = NULL;
    }
}

size_t array_size(const array * const a) {
    return a->size;
}

void array_set(array * const a, size_t index, void *val) {
    memcpy(array_get(a, index), val, a->elem_size);
}

void *array_get(const array * const a, size_t index) {
    if (index < a->size) {
        return a->data + a->elem_size * index;
    } else {
        return NULL;
    }
}

void array_fill(array * const a, void *val) {
    void *p = a->data;
    void *end = a->data + a->elem_size * a->size;
    while (p != end) {
        memcpy(p, val, a->elem_size);
        p += a->elem_size;
    }
}

