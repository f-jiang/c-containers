#ifndef OMAP_H
#define OMAP_H

#include <stddef.h>     // size_t
#include <stdbool.h>    // bool

#include "pair.h"

typedef struct ordered_map_t omap;

omap *omap_init(size_t key_size, size_t val_size, int (*comp)(void *a, void *b));
void omap_del(omap **m);

size_t omap_get_count(const omap * const s);

bool omap_insert(omap * const m, void *key, void *val);
bool omap_remove(omap * const m, void *key);
void *omap_get(omap * const m, void *key);
pair *omap_first(const omap * const m);
pair *omap_last(const omap * const m);
pair *omap_lower(const omap * const m, void *key);
pair *omap_higher(const omap * const m, void *key);
void *omap_first_key(const omap * const m);
void *omap_last_key(const omap * const m);
void *omap_lower_key(const omap * const m, void *key);
void *omap_higher_key(const omap * const m, void *key);

bool omap_contains(const omap * const m, void *key);

#endif

