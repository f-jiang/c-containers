#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>     // size_t

typedef list queue;

queue *queue_init(size_t elem_size);
void queue_del(queue **q);

size_t queue_get_len(const queue * const q);

void *queue_front(const queue * const q);
void *queue_back(const queue * const q);

void queue_push(queue * const q, void *val);
void queue_pop(queue * const q);

#endif
