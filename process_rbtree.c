// process_rbtree.c

#include "rbtree.h"
#include "process.h"
#include <stdlib.h>

// Define the key comparison function for processes based on pid
static int compare_processes(const void* a, const void* b) {
    const struct process* process_a = (const struct process*)a;
    const struct process* process_b = (const struct process*)b;
    return process_a->pid - process_b->pid;
}

// Initialize a Red-Black Tree for processes
void process_rbtree_init(struct rbtree* tree) {
    rbtree_init(tree);
}

// Insert a process into the Red-Black Tree
void process_rbtree_insert(struct rbtree* tree, struct process* process) {
    struct rbtree_node* node = (struct rbtree_node*)malloc(sizeof(struct rbtree_node));
    if (node == NULL) {
        // Handle memory allocation failure
        return;
    }

    node->key = process->pid;  // Use pid as the key
    node->parent = NULL;
    node->left = &rbtree_sentinel;
    node->right = &rbtree_sentinel;
    node->color = RED;

    rbtree_insert(tree, (struct process*)node);
}

// Retrieve the process with the smallest pid
struct process* process_rbtree_min(struct rbtree* tree) {
    struct rbtree_node* min_node = rbtree_minimum(tree->root);
    return (struct process*)min_node;
}
