#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>     // size_t

typedef list queue;

queue *queue_init(size_t elem_size);
void queue_del(queue **s);

size_t queue_get_len(const queue * const s);

void *queue_front(const queue * const s);
void *queue_back(const queue * const s);

void queue_push(queue * const s, void *val);
void queue_pop(queue * const s);

#endif
