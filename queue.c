#include "queue.h"

queue *queue_init(size_t elem_size) {
    return list_init(elem_size);
}

void queue_del(queue **q) {
    list_del(q);
}

size_t queue_size(const queue * const q) {
    return list_size(q);
}

void *queue_front(const queue * const q) {
    return list_front(q);
}

void *queue_back(const queue * const q) {
    return list_back(q);
}

void queue_push(queue * const q, void *val) {
    list_push_front(q, val);
}

void queue_pop(queue * const q) {
    list_pop_back(q);
}

