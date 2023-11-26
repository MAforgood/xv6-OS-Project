// rbtree.h

#ifndef RBTREE_H
#define RBTREE_H

// Define colors for Red-Black Tree nodes
enum rbtree_color {
    RED,
    BLACK
};

// Forward declaration of the Process structure
struct process;

// Define the Red-Black Tree node structure
struct rbtree_node {
    int key;  // You may need to replace this with the actual type of your key
    // Add other fields as needed for your use case

    enum rbtree_color color;
    struct rbtree_node* parent;
    struct rbtree_node* left;
    struct rbtree_node* right;
};

// Define the Red-Black Tree structure
struct rbtree {
    struct rbtree_node* root;
    int (*compare)(const void*, const void*);
    // Add any other necessary fields for your Red-Black Tree
};

extern struct rbtree_node rbtree_sentinel;

// Function prototypes
void rbtree_init(struct rbtree* tree);
void rbtree_insert(struct rbtree* tree, struct process* process);
void rbtree_delete(struct rbtree* tree, struct process* process);
struct rbtree_node* rbtree_search(struct rbtree* tree, int key);
// Add other utility functions as needed

#endif // RBTREE_H
