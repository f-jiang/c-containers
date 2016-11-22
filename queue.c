#include "list.h"
#include "queue.h"

queue *queue_init(size_t elem_size) {
    return list_init(elem_size);
}

void queue_del(queue **s) {
    list_del(s);
}

size_t queue_get_len(const queue * const s) {
    return list_get_len(s);
}

void *queue_front(const queue * const s) {
    return list_front(s);
}

void *queue_back(const queue * const s) {
    return list_back(s);
}

void queue_push(queue * const s, void *val) {
    list_push_front(s, val);
}

void queue_pop(queue * const s) {
    list_pop_back(s);
}

