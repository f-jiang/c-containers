#ifndef OSET_H
#define OSET_H

#include <stddef.h>     // size_t
#include <stdbool.h>    // bool

typedef struct ordered_set_t oset;

oset *oset_init(size_t elem_size, int (*comp)(void *a, void *b));
void oset_del(oset **s);

size_t oset_get_count(const oset * const s);

bool oset_insert(oset * const s, void *val);
bool oset_remove(oset * const s, void *val);
void *oset_first(const oset * const s);
void *oset_last(const oset * const s);
void *oset_lower(const oset * const s, void *val);
void *oset_higher(const oset * const s, void *val);

bool oset_contains(const oset * const s, void *val);

oset *oset_union(const oset * const a, const oset * const b);
oset *oset_intxn(const oset * const a, const oset * const b);
oset *oset_diff(const oset * const a, const oset * const b);

#endif

