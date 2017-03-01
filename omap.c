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

static struct node_t *get_min_node(struct node_t *root) {
    if (root == NULL || root->left == NULL) {
        return root;
    } else {
        return get_min_node(root->left);
    }
}

static struct node_t *get_max_node(struct node_t *root) {
    if (root == NULL || root->right== NULL) {
        return root;
    } else {
        return get_max_node(root->right);
    }
}

static struct node_t *get_prev_node(const omap * const m, struct node_t *root, void *key) {
    struct node_t *n = get_node(m, m->data, key);
    struct node_t *pr;

    if (n == get_min_node(m->data)) {
        pr = NULL;
    } else if (n->left == NULL) {
        bool done = false;
        do {
            pr = n->parent;
            done = (n == pr->right);
            n = pr;
        } while (!done);
    } else {
        pr = get_max_node(n->left);
    }

    return pr;
}

static struct node_t *get_next_node(const omap * const m, struct node_t *root, void *key) {
    struct node_t *n = get_node(m, m->data, key);
    struct node_t *sc;

    if (n == get_max_node(m->data)) {
        sc = NULL;
    } else if (n->right == NULL) {
        bool done = false;
        do {
            sc = n->parent;
            done = (n == sc->left);
            n = sc;
        } while (!done);
    } else {
        sc = get_min_node(n->right);
    }

    return sc;
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
        struct node_t *min = get_min_node(remove->right);

        void *tmp_key = malloc(m->key_size);
        void *tmp_val = malloc(m->val_size);
        memcpy(tmp_key, min->key, m->key_size);
        memcpy(tmp_val, min->val, m->val_size);
        remove_node(m, remove->right, min->key);
        memcpy(min->key, tmp_key, m->key_size);
        memcpy(min->val, tmp_val, m->val_size);
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

pair *omap_first(const omap * const m) {
    struct node_t *first = get_min_node(m->data);
    struct pair_t *p = NULL;

    if (first != NULL) {
        p = pair_init();
        p->key = first->key;
        p->val = first->val;
    }

    return p; 
}

pair *omap_last(const omap * const m) {
    struct node_t *last = get_max_node(m->data);
    struct pair_t *p = NULL;

    if (last != NULL) {
        p = pair_init();
        p->key = last->key;
        p->val = last->val;
    }

    return p; 
}

pair *omap_lower(const omap * const m, void *key) {
    struct node_t *pr = get_prev_node(m, m->data, key);
    struct pair_t *p = NULL;

    if (pr != NULL) {
        p = pair_init();
        p->key = pr->key;
        p->val = pr->val;
    }

    return p;
}

pair *omap_higher(const omap * const m, void *key) {
    struct node_t *sc = get_next_node(m, m->data, key);
    struct pair_t *p = NULL;

    if (sc != NULL) {
        p = pair_init();
        p->key = sc->key;
        p->val = sc->val;
    }

    return p;
}

void *omap_first_key(const omap * const m) {
    struct node_t *first = get_min_node(m->data);
    return (first == NULL) ? NULL : first->key;
}

void *omap_last_key(const omap * const m) {
    struct node_t *last = get_max_node(m->data);
    return (last == NULL) ? NULL : last->key;
}

void *omap_lower_key(const omap * const m, void *key) {
    struct node_t *pr = get_prev_node(m, m->data, key);
    return (pr == NULL) ? NULL : pr->key;
}

void *omap_higher_key(const omap * const m, void *key) {
    struct node_t *sc = get_next_node(m, m->data, key);
    return (sc == NULL) ? NULL : sc->key;
}

bool omap_insert(omap * const m, void *key, void *val) {
    return add_node(m, m->data, key, val);
}

bool omap_remove(omap * const m, void *key) {
    return remove_node(m, m->data, key);
}

void *omap_get(omap * const m, void *key) {
    struct node_t *n = get_node(m, m->data, key);
    return (n == NULL) ? NULL : n->val;
}

bool omap_contains(const omap * const m, void *key) {
    return get_node(m, m->data, key) != NULL;
}

