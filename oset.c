#include "oset.h"

#include <stdlib.h>     // malloc(), free()
#include <string.h>     // memcpy()

struct node_t {
    void *val;
    struct node_t *left, *right, *parent;
};

struct ordered_set_t {
    size_t count;
    size_t elem_size;
    struct node_t *data;

    /*
     * < 0    *a before *b
     * 0      *a and *b equivalent
     * > 0    *a after *b
     */
    int (*comp)(void *a, void *b);
};

static struct node_t *node_init(const oset * const s, void *val) {
    struct node_t *n = malloc(sizeof(*n));
    n->left = n->right = n->parent = NULL;
    n->val = malloc(s->elem_size);
    memcpy(n->val, val, s->elem_size);
    return n;
}

static void node_del(struct node_t *n) {
    if (n != NULL) {
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

static struct node_t *get_node(const oset * const s, struct node_t *root, void *val) {
    if (root == NULL || (*s->comp)(root->val, val) == 0) {
        return root;
    } else if ((*s->comp)(root->val, val) > 0) {
        return get_node(s, root->left, val);
    } else {
        return get_node(s, root->right, val);
    }
}

static struct node_t *get_min_node(struct node_t *root) {
    if (root->left == NULL) {
        return root;
    } else {
        return get_min_node(root->left);
    }
}

static struct node_t *get_max_node(struct node_t *root) {
    if (root->right== NULL) {
        return root;
    } else {
        return get_max_node(root->right);
    }
}

static bool add_node(oset * const s, struct node_t *root, void *val) {
    if (s->count == 0) {
        s->data = node_init(s, val);
        s->count = 1;
        return true;
    } else if ((*s->comp)(root->val, val) > 0) {
        if (root->left == NULL) {
            root->left = node_init(s, val);
            root->left->parent = root;
            s->count++;
            return true;
        } else {
            add_node(s, root->left, val);
        }
    } else if ((*s->comp)(root->val, val) < 0) {
        if (root->right == NULL) {
            root->right = node_init(s, val);
            root->right->parent = root;
            s->count++;
            return true;
        } else {
            add_node(s, root->right, val);
        }
    } else {
        return false;
    }
}

static bool remove_node(oset * const s, struct node_t *root, void *val) {
    struct node_t *remove = get_node(s, root, val);
    struct node_t *parent = remove->parent;

    if (remove == NULL) {
        return false;
    } else if (remove->left == NULL && remove->right == NULL) {    // no children
        if (parent != NULL) {
            if (remove->val < parent->val) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        }

        node_del(remove);
        s->count--;
        return true;
    } else if (remove->right == NULL) {    // only has left child
        if (parent == NULL) {
            s->data = remove->left;
        } else {
            if (remove->val < parent->val) {
                parent->left = remove->left;
            } else {
                parent->right = remove->left;
            }
        }

        node_del(remove);
        s->count--;
        return true;
    } else if (remove->left == NULL) {    // only has right child
        if (parent == NULL) {
            s->data = remove->right;
        } else {
            if (remove->val < parent->val) {
                parent->left = remove->right;
            } else {
                parent->right = remove->right;
            }
        }

        node_del(remove);
        s->count--;
        return true;
    } else {    // two children
        struct node_t *min = get_min_node(remove->right);
        void *temp = malloc(s->elem_size);
        memcpy(temp, min->val, s->elem_size);
        remove_node(s, remove->right, min->val);
        memcpy(min->val, temp, s->elem_size);
        free(temp);
    }
}

oset *oset_init(size_t elem_size, int (*comp)(void *a, void *b)) {
    struct ordered_set_t *s = malloc(sizeof(*s));
    s->count = 0;
    s->elem_size = elem_size;
    s->data = NULL;
    s->comp = comp;
    return s;
}

void oset_del(oset **s) {
    if (*s != NULL) {
        tree_del((*s)->data);
        free(*s);
        *s = NULL;
    }
}

size_t oset_get_count(const oset * const s) {
    return s->count;
}

void *oset_first(const oset * const s) {
    return get_min_node(s->data)->val;
}

void *oset_last(const oset * const s) {
    return get_max_node(s->data)->val;
}

void *oset_lower(const oset * const s, void *val) {
    struct node_t *n = get_node(s, s->data, val);
    struct node_t *pr;

    if (n == get_min_node(s->data)) {
        pr = NULL;
    } else if (n->left== NULL) {
        bool done = false;
        do {
            pr = n->parent;
            done = (n == pr->right);
            n = pr;
        } while (!done);
    } else {
        pr = get_max_node(n->left);
    }

    return (pr == NULL) ? NULL : pr->val;
}

void *oset_higher(const oset * const s, void *val) {
    struct node_t *n = get_node(s, s->data, val);
    struct node_t *sc;

    if (n == get_max_node(s->data)) {
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

    return (sc == NULL) ? NULL : sc->val;
}

bool oset_insert(oset * const s, void *val) {
    return add_node(s, s->data, val);
}

bool oset_remove(oset * const s, void *val) {
    return remove_node(s, s->data, val);
}

bool oset_contains(const oset * const s, void *val) {
    return get_node(s, s->data, val) != NULL;
}

oset *oset_union(const oset * const a, const oset * const b) {
    struct ordered_set_t *result = oset_init(a->elem_size, a->comp);
    void *cur;
    size_t i;

    cur = oset_first(a);
    for (i = 0; i < a->count; i++) {
        oset_insert(result, cur);
        cur = oset_higher(a, cur);
    }

    cur = oset_first(b);
    for (i = 0; i < b->count; i++) {
        oset_insert(result, cur);
        cur = oset_higher(b, cur);
    }

    return result;
}

oset *oset_intxn(const oset * const a, const oset * const b) {
    struct ordered_set_t *result = oset_init(a->elem_size, a->comp);
    const struct ordered_set_t *base, *other;

    if (a->count < b->count) {
        base = a;
        other = b;
    } else {
        base = b;
        other = a;
    }

    void *cur = oset_first(base);
    size_t i;
    for (i = 0; i < base->count; i++) {
        if (oset_contains(other, cur)) {
            oset_insert(result, cur);
        }
        cur = oset_higher(base, cur);
    }

    return result;
}

oset *oset_diff(const oset * const a, const oset * const b) {
    struct ordered_set_t *result = oset_init(a->elem_size, a->comp);

    void *cur = oset_first(a);
    size_t i;
    for (i = 0; i < a->count; i++) {
        if (!oset_contains(b, cur)) {
            oset_insert(result, cur);
        }
        cur = oset_higher(a, cur);
    }

    return result;
}

