#include "bst.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Taken from Trees slide from
// Darrell Long. Used to find the max.

static int max(int x, int y) {
    return x > y ? x : y;
}

uint64_t branches = 0;

Node *bst_create(void) {
    return NULL;
}

uint32_t bst_height(Node *root) {

    // Taken from Trees slide from
    // Darrell Long.

    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

uint32_t bst_size(Node *root) {
    if (root == NULL) {
        return 0;
    }
    uint32_t size = 0;
    size += bst_size(root->left);
    size += bst_size(root->right);
    return 1 + size;
}

Node *bst_find(Node *root, char *oldspeak) {

    // Taken from Trees slide from
    // Darrell Long.

    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            return bst_find(root->right, oldspeak);
        }
    }
    return root;
}

Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (bst_find(root, oldspeak)) {
        return root;
    }

    // Taken from Trees slide from
    // Darrell Long.
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        return root;
    }
    root = node_create(oldspeak, newspeak);
    return root;
}

void bst_print(Node *root) {

    // Taken from Trees slide from
    // Darrell Long.
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}

void bst_delete(Node **root) {

    // Taken from Trees slide from
    // Darrell Long.

    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
}
