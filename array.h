#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>     // size_t

typedef struct array_t array;

array *array_init(size_t elem_size, size_t len);
void array_del(array **a);

size_t array_get_len(const array * const a);

void array_set(array * const a, size_t index, void *val);
void *array_get(const array * const a, size_t index);

void array_fill(array * const a, void* val);

#endif
