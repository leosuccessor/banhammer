#include "ht.h"
#include "salts.h"
#include "speck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// Creating Hash Table and allocating memory.

HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
    }
    return ht;
}

// Freeing allocated memory.

void ht_delete(HashTable **ht) {
    if (*ht && (*ht)->trees) {
        for (uint32_t i = 0; i < ht_size(*ht); i += 1) {
            bst_delete(&(*ht)->trees[i]);
        }
        free((*ht)->trees);
        free(*ht);
        *ht = NULL;
    }
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// If the node for the oldspeak is found, the node
// will be returned, otherwise NULL is returned.
// Code based off of Darrell Longs lecture slide
// on hashing. Page 22.

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups += 1;
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;

    if (ht->trees[index] == NULL) {
        return NULL;
    }
    return bst_find(ht->trees[index], oldspeak);
}

// Inserts oldspeak and newspeak into the Hash Table.
// Code based off of Darrell Longs lecture slide
// on hashing. Page 23.

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups += 1;
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;

    if (ht->trees[index] == NULL) {
        ht->trees[index] = node_create(oldspeak, newspeak);
        return;
    } else {
        bst_insert(ht->trees[index], oldspeak, newspeak);
        return;
    }
    return;
}

// Will return the number of non-NULL binary search
// trees that are in the Hash Table.

uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->trees[i]) {
            count += 1;
        }
    }
    return count;
}

// Will return the average binary search tree
// size.

double ht_avg_bst_size(HashTable *ht) {
    double avg_size = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        avg_size += bst_size(ht->trees[i]);
    }
    return avg_size / ((double) ht_count(ht));
}

// Will return the average binary search tree
// height.

double ht_avg_bst_height(HashTable *ht) {
    double avg_height = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        avg_height += bst_height(ht->trees[i]);
    }
    return avg_height / ((double) ht_count(ht));
}

// Prints out the Hash Table, all the BST's.

void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->trees[i]) {
            bst_print(ht->trees[i]);
        }
    }
}
