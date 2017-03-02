#ifndef OMAP_H
#define OMAP_H

#include <stddef.h>     // size_t
#include <stdbool.h>    // bool

#include "pair.h"

typedef struct ordered_map_t omap;

omap *omap_init(size_t key_size, size_t val_size, int (*comp)(void *a, void *b));
void omap_del(omap **m);

size_t omap_size(const omap * const m);

void *omap_get(omap * const m, void *key);
bool omap_insert(omap * const m, void *key, void *val);
bool omap_remove(omap * const m, void *key);
bool omap_contains(const omap * const m, void *key);

pair *omap_floor(const omap * const m);
pair *omap_ceil(const omap * const m);
pair *omap_lower(const omap * const m, void *key);
pair *omap_higher(const omap * const m, void *key);

void *omap_floor_key(const omap * const m);
void *omap_ceil_key(const omap * const m);
void *omap_lower_key(const omap * const m, void *key);
void *omap_higher_key(const omap * const m, void *key);

#endif

