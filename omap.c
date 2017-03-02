#include "omap.h"

#include <stdlib.h>     // malloc(), free()
#include <string.h>     // memcpy()

struct node_t {
    void *key, *val;
    struct node_t *left, *right, *parent;
};

struct ordered_map_t {
    size_t count;
    size_t key_size, val_size;
    struct node_t *data;

    /*
     * < 0    *a before *b
     * 0      *a and *b equivalent
     * > 0    *a after *b
     */
    int (*comp)(void *a, void *b);
};

static struct node_t *node_init(const omap * const m, void *key, void *val) {
    struct node_t *n = malloc(sizeof(*n));
    n->left = n->right = n->parent = NULL;

    n->key = malloc(m->key_size);
    memcpy(n->key, key, m->key_size);

    n->val = malloc(m->val_size);
    memcpy(n->val, val, m->val_size);

    return n;
}

static void node_del(struct node_t *n) {
    if (n != NULL) {
        free(n->key);
        free(n->val);
        free(n);
    }
}

static void tree_del(struct node_t *root) {
    if (root != NULL) {
        tree_del(root->left);
        tree_del(root->right);
        node_del(root);
    }
}

static struct node_t *get_node(const omap * const m, struct node_t *root, void *key) {
    if (root == NULL || (*m->comp)(root->key, key) == 0) {
        return root;
    } else if ((*m->comp)(root->key, key) > 0) {
        return get_node(m, root->left, key);
    } else {
        return get_node(m, root->right, key);
    }
}

static struct node_t *get_floor_node(struct node_t *root) {
    if (root == NULL || root->left == NULL) {
        return root;
    } else {
        return get_floor_node(root->left);
    }
}

static struct node_t *get_ceil_node(struct node_t *root) {
    if (root == NULL || root->right== NULL) {
        return root;
    } else {
        return get_ceil_node(root->right);
    }
}

static struct node_t *get_lower_node(const omap * const m, void *key) {
    struct node_t *n = get_node(m, m->data, key);
    struct node_t *lo;

    if (n == get_floor_node(m->data)) {
        lo = NULL;
    } else if (n->left == NULL) {
        bool done = false;
        do {
            lo = n->parent;
            done = (n == lo->right);
            n = lo;
        } while (!done);
    } else {
        lo = get_ceil_node(n->left);
    }

    return lo;
}

static struct node_t *get_higher_node(const omap * const m, void *key) {
    struct node_t *n = get_node(m, m->data, key);
    struct node_t *hi;

    if (n == get_ceil_node(m->data)) {
        hi = NULL;
    } else if (n->right == NULL) {
        bool done = false;
        do {
            hi = n->parent;
            done = (n == hi->left);
            n = hi;
        } while (!done);
    } else {
        hi = get_floor_node(n->right);
    }

    return hi;
}

static bool add_node(omap * const m, struct node_t *root, void *key, void *val) {
    if (m->count == 0) {
        m->data = node_init(m, key, val);
        m->count = 1;
        return true;
    } else if ((*m->comp)(root->key, key) > 0) {
        if (root->left == NULL) {
            root->left = node_init(m, key, val);
            root->left->parent = root;
            m->count++;
            return true;
        } else {
            add_node(m, root->left, key, val);
        }
    } else if ((*m->comp)(root->key, key) < 0) {
        if (root->right == NULL) {
            root->right = node_init(m, key, val);
            root->right->parent = root;
            m->count++;
            return true;
        } else {
            add_node(m, root->right, key, val);
        }
    } else {
        return false;
    }
}

static bool remove_node(omap * const m, struct node_t *root, void *key) {
    struct node_t *remove = get_node(m, root, key);
    struct node_t *parent = (remove == NULL) ? NULL : remove->parent;

    if (remove == NULL) {
        return false;
    } else if (remove->left == NULL && remove->right == NULL) {    // no children
        if (parent != NULL) {
            if ((*m->comp)(remove->key, parent->key) < 0) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        } else {
            m->data = NULL;
        }

        node_del(remove);
        m->count--;
        return true;
    } else if (remove->right == NULL) {    // only has left child
        if (parent == NULL) {
            m->data = remove->left;
            m->data->parent = NULL;
        } else {
            if ((*m->comp)(remove->key, parent->key) < 0) {
                parent->left = remove->left;
            } else {
                parent->right = remove->left;
            }

            remove->left->parent = parent;
        }

        node_del(remove);
        m->count--;
        return true;
    } else if (remove->left == NULL) {    // only has right child
        if (parent == NULL) {
            m->data = remove->right;
            m->data->parent = NULL;
        } else {
            if ((*m->comp)(remove->key, parent->key) < 0) {
                parent->left = remove->right;
            } else {
                parent->right = remove->right;
            }

            remove->right->parent = parent;
        }

        node_del(remove);
        m->count--;
        return true;
    } else {    // two children
        struct node_t *floor = get_floor_node(remove->right);

        void *tmp_key = malloc(m->key_size);
        void *tmp_val = malloc(m->val_size);
        memcpy(tmp_key, floor->key, m->key_size);
        memcpy(tmp_val, floor->val, m->val_size);
        remove_node(m, remove->right, floor->key);
        memcpy(floor->key, tmp_key, m->key_size);
        memcpy(floor->val, tmp_val, m->val_size);
        free(tmp_key);
        free(tmp_val);
    }
}

omap *omap_init(size_t key_size, size_t val_size, int (*comp)(void *a, void *b)) {
    struct ordered_map_t *m = malloc(sizeof(*m));
    m->count = 0;
    m->key_size = key_size;
    m->val_size = val_size;
    m->data = NULL;
    m->comp = comp;
    return m;
}

void omap_del(omap **m) {
    if (*m != NULL) {
        tree_del((*m)->data);
        free(*m);
        *m = NULL;
    }
}

size_t omap_get_count(const omap * const m) {
    return m->count;
}

void *omap_get(omap * const m, void *key) {
    struct node_t *n = get_node(m, m->data, key);
    return (n == NULL) ? NULL : n->val;
}

bool omap_insert(omap * const m, void *key, void *val) {
    return add_node(m, m->data, key, val);
}

bool omap_remove(omap * const m, void *key) {
    return remove_node(m, m->data, key);
}

bool omap_contains(const omap * const m, void *key) {
    return get_node(m, m->data, key) != NULL;
}

pair *omap_floor(const omap * const m) {
    struct node_t *floor = get_floor_node(m->data);
    struct pair_t *p = NULL;

    if (floor != NULL) {
        p = pair_init();
        p->key = floor->key;
        p->val = floor->val;
    }

    return p; 
}

pair *omap_ceil(const omap * const m) {
    struct node_t *ceil = get_ceil_node(m->data);
    struct pair_t *p = NULL;

    if (ceil != NULL) {
        p = pair_init();
        p->key = ceil->key;
        p->val = ceil->val;
    }

    return p; 
}

pair *omap_lower(const omap * const m, void *key) {
    struct node_t *lo = get_lower_node(m, key);
    struct pair_t *p = NULL;

    if (lo != NULL) {
        p = pair_init();
        p->key = lo->key;
        p->val = lo->val;
    }

    return p;
}

pair *omap_higher(const omap * const m, void *key) {
    struct node_t *hi = get_higher_node(m, key);
    struct pair_t *p = NULL;

    if (hi != NULL) {
        p = pair_init();
        p->key = hi->key;
        p->val = hi->val;
    }

    return p;
}

void *omap_floor_key(const omap * const m) {
    struct node_t *floor = get_floor_node(m->data);
    return (floor == NULL) ? NULL : floor->key;
}

void *omap_ceil_key(const omap * const m) {
    struct node_t *ceil = get_ceil_node(m->data);
    return (ceil == NULL) ? NULL : ceil->key;
}

void *omap_lower_key(const omap * const m, void *key) {
    struct node_t *lo = get_lower_node(m, key);
    return (lo == NULL) ? NULL : lo->key;
}

void *omap_higher_key(const omap * const m, void *key) {
    struct node_t *hi = get_higher_node(m, key);
    return (hi == NULL) ? NULL : hi->key;
}

