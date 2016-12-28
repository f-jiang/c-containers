#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>     // size_t

typedef list deque;

deque *deque_init(size_t elem_size);
void deque_del(deque **d);

size_t deque_get_len(const deque * const d);

void *deque_front(const deque * const d);
void *deque_back(const deque * const d);

void deque_push_front(deque * const d, void *val);
void deque_pop_front(deque * const d);

void deque_push_back(deque * const d, void *val);
void deque_pop_back(deque * const d);

#endif
