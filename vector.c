#include "vector.h"

#include <stdlib.h>     // malloc(), free()
#include <string.h>     // memcpy()

#define RESIZE_INCREMENT 10

struct vector_t {
    size_t len, cap;
    size_t elem_size;
    void *data;
};

static void vector_resize(vector * const v, size_t new_len) {
    size_t new_cap = (new_len / RESIZE_INCREMENT + 1) * RESIZE_INCREMENT;

    if (new_cap != v->cap) {
        void *tmp = realloc(v->data, new_cap * v->elem_size); 

        if (tmp) {
            v->data = tmp;
            v->len = new_len;
            v->cap = new_cap;
        }
    } else {
        v->len = new_len;
    }
}

vector *vector_init(size_t elem_size) {
    struct vector_t *v = malloc(sizeof(*v));
    v->len = 0;
    v->cap = RESIZE_INCREMENT;
    v->elem_size = elem_size;
    v->data = malloc(v->cap * elem_size);
    return v;
}

void vector_del(vector **v) {
    free((*v)->data);
    free(*v);
    *v = NULL;
}

size_t vector_get_len(const vector * const v) {
    return v->len;
}

void vector_set(vector * const v, size_t index, void *val) {
    memcpy(vector_get(v, index), val, v->elem_size);
}

void *vector_get(const vector * const v, size_t index) {
    if (index < v->len) {
        return v->data + v->elem_size * index;
    } else {
        return NULL;
    }
}

void vector_push_back(vector * const v, void *val) {
    vector_resize(v, v->len + 1); 
    vector_set(v, v->len - 1, val);
}

void vector_pop_back(vector * const v) {
    vector_resize(v, v->len - 1);
}

void vector_fill(vector * const v, void *val) {
    void *p = v->data;
    void *end = v->data + v->elem_size * v->len;
    while (p != end) {
        memcpy(p, val, v->elem_size);
        p += v->elem_size;
    }
}

