#include "list.h"
#include "stack.h"

stack *stack_init(size_t elem_size) {
    return list_init(elem_size);
}

void stack_del(stack **s) {
    list_del(s);
}

size_t stack_get_len(const stack * const s) {
    return list_get_len(s);
}

void *stack_top(const stack * const s) {
    return list_back(s);
}

void stack_push(stack * const s, void *val) {
    list_push_back(s, val);
}

void stack_pop(stack * const s) {
    list_pop_back(s);
}

