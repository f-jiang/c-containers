#ifndef STACK_H
#define STACK_H

#include <stddef.h>     // size_t

#include "list.h"

typedef list stack;

stack *stack_init(size_t elem_size);
void stack_del(stack **s);

size_t stack_size(const stack * const s);

void *stack_top(const stack * const s);

void stack_push(stack * const s, void *val);
void stack_pop(stack * const s);

#endif
