#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>     // size_t

typedef struct vector_t vector;

vector *vector_init(size_t elem_size);
void vector_del(vector **v);

size_t vector_get_len(const vector * const v);

void vector_set(vector * const v, size_t index, void *val);
void *vector_get(const vector * const v, size_t index);

void vector_push_back(vector * const v, void *val);
void vector_pop_back(vector * const v);

void vector_fill(vector * const v, void* val);

#endif
