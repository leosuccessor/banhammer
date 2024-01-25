#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Creating the node and allocating memory.

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = malloc(sizeof(Node));
    if (n) {
        n->left = NULL;
        n->right = NULL;
        if (oldspeak != NULL) {
            n->oldspeak = strdup(oldspeak);
        } else {
            n->oldspeak = NULL;
        }
        if (newspeak != NULL) {
            n->newspeak = strdup(newspeak);
        } else {
            n->newspeak = NULL;
        }
    }
    return n;
}

// Freeing the allocated memory.

void node_delete(Node **n) {
    if (*n) {
        free((*n)->oldspeak);
        free((*n)->newspeak);
        free(*n);
        *n = NULL;
    }
}

// This is to print the node.

void node_print(Node *n) {
    if (n->newspeak == NULL) {
        printf("%s\n", n->oldspeak);
    } else {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
}
