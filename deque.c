#include "list.h"
#include "deque.h"

deque *deque_init(size_t elem_size) {
    return list_init(elem_size);
}

void deque_del(deque **d) {
    list_del(d);
}

size_t deque_size(const deque * const d) {
    return list_size(d);
}

void *deque_front(const deque * const d) {
    return list_front(d);
}

void *deque_back(const deque * const d) {
    return list_back(d);
}

void deque_push_front(deque * const d, void *val) {
    list_push_front(d, val);
}

void deque_pop_front(deque * const d) {
    list_pop_front(d);
}

void deque_push_back(deque * const d, void *val) {
    list_push_back(d, val);
}

void deque_pop_back(deque * const d) {
    list_pop_back(d);
}

