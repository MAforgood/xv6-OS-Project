// rbtree.c

#include "rbtree.h"
#include "process.h" 
#include <stddef.h> // For NULL definition

// Define a sentinel node for leaves
struct rbtree_node rbtree_sentinel = {0, BLACK, NULL, NULL, NULL};

// Function prototypes
static void rbtree_left_rotate(struct rbtree* tree, struct rbtree_node* x);
static void rbtree_right_rotate(struct rbtree* tree, struct rbtree_node* y);
static void rbtree_insert_fixup(struct rbtree* tree, struct rbtree_node* z);
static void rbtree_delete_fixup(struct rbtree* tree, struct rbtree_node* x, struct rbtree_node* x_parent);

// Initialize a Red-Black Tree
void rbtree_init(struct rbtree* tree) {
    tree->root = &rbtree_sentinel;
}

// Perform a left rotation around the node x
static void rbtree_left_rotate(struct rbtree* tree, struct rbtree_node* x) {
    struct rbtree_node* y = x->right;
    x->right = y->left;

    if (y->left != &rbtree_sentinel) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == &rbtree_sentinel) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

// Perform a right rotation around the node y
static void rbtree_right_rotate(struct rbtree* tree, struct rbtree_node* y) {
    struct rbtree_node* x = y->left;
    y->left = x->right;

    if (x->right != &rbtree_sentinel) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == &rbtree_sentinel) {
        tree->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

void rbtree_insert(struct rbtree* tree, struct process* process) {
    struct rbtree_node* z = (struct rbtree_node*)malloc(sizeof(struct rbtree_node));
    z->key = process->pid; // Assuming the key is an integer, replace it with the actual type
    // Initialize other fields of z
    z->parent = NULL;
    z->left = &rbtree_sentinel;
    z->right = &rbtree_sentinel;
    z->color = RED;

    struct rbtree_node* y = &rbtree_sentinel;
    struct rbtree_node* x = tree->root;

    while (x != &rbtree_sentinel) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;

    if (y == &rbtree_sentinel) {
        tree->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    rbtree_insert_fixup(tree, z);
}

// Fix the Red-Black Tree properties after insertion
static void rbtree_insert_fixup(struct rbtree* tree, struct rbtree_node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            struct rbtree_node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rbtree_left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_right_rotate(tree, z->parent->parent);
            }
        } else {
            // Symmetric case
            struct rbtree_node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rbtree_right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_left_rotate(tree, z->parent->parent);
            }
        }
    }

    tree->root->color = BLACK;
}


void rbtree_delete(struct rbtree* tree, struct process* process) {
    struct rbtree_node* z = rbtree_search(tree, process->pid);
    struct rbtree_node* y = z;
    struct rbtree_node* x;
    enum rbtree_color y_original_color = y->color;

    if (z->left == &rbtree_sentinel) {
        x = z->right;
        rbtree_transplant(tree, z, z->right);
    } else if (z->right == &rbtree_sentinel) {
        x = z->left;
        rbtree_transplant(tree, z, z->left);
    } else {
        y = rbtree_minimum(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            rbtree_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rbtree_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == BLACK) {
        rbtree_delete_fixup(tree, x, x->parent);
    }
}

// Fix the Red-Black Tree properties after deletion
static void rbtree_delete_fixup(struct rbtree* tree, struct rbtree_node* x, struct rbtree_node* x_parent) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x_parent->left) {
            struct rbtree_node* w = x_parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x_parent->color = RED;
                rbtree_left_rotate(tree, x_parent);
                w = x_parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x_parent;
                x_parent = x_parent->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rbtree_right_rotate(tree, w);
                    w = x_parent->right;
                }
                w->color = x_parent->color;
                x_parent->color = BLACK;
                w->right->color = BLACK;
                rbtree_left_rotate(tree, x_parent);
                x = tree->root; // Break out of the loop
            }
        } else {
            struct rbtree_node* w = x_parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x_parent->color = RED;
                rbtree_right_rotate(tree, x_parent);
                w = x_parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x_parent;
                x_parent = x_parent->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rbtree_left_rotate(tree, w);
                    w = x_parent->left;
                }
                w->color = x_parent->color;
                x_parent->color = BLACK;
                w->left->color = BLACK;
                rbtree_right_rotate(tree, x_parent);
                x = tree->root; // Break out of the loop
            }
        }
    }

    x->color = BLACK;
}


// Search for a node with a specific key in the Red-Black Tree
struct rbtree_node* rbtree_search(struct rbtree* tree, int key) {
    struct rbtree_node* x = tree->root;

    while (x != &rbtree_sentinel && key != x->key) {
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    return x;
}


// Helper function to transplant a subtree
void rbtree_transplant(struct rbtree* tree, struct rbtree_node* u, struct rbtree_node* v) {
    if (u->parent == &rbtree_sentinel) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

// Helper function to find the minimum node in a subtree
struct rbtree_node* rbtree_minimum(struct rbtree_node* x) {
    while (x->left != &rbtree_sentinel) {
        x = x->left;
    }
    return x;
}
