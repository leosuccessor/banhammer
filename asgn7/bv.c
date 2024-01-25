#include "bv.h"

#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// Creating the bit vector and allocating memory.
// Taken from code comments repo.

BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) calloc(1, sizeof(BitVector));
    if (bv) {
        bv->length = length;
        bv->vector = calloc((length / 8) + (length % 8), sizeof(uint8_t));
        if (!bv->vector) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

// Freeing the allocated memory.

void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
    return;
}

// Returns the length of the bit vector.

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// Sets the bit in the bit vector. Will return true
// if successful and return false otherwise.
// Taken from resources repo, Darrell Long.

bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] |= (0x1 << (i % 8));
    return true;
}

// Clears the bit in the bit vector. Will return true
// if successful and return false otherwise.
// Taken from resources repo, Darrell Long.

bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] &= ~(0x1 << (i % 8));
    return true;
}

// Gets the bit in the bit vector. Will return true
// if successful and return false otherwise.
// Taken from resources repo, Darrell Long.

bool bv_get_bit(BitVector *bv, uint32_t i) {
    return (bv->vector[i / 8] >> (i % 8)) & 0x1;
}

// This is to print out the bit vector.

void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv_length(bv); i += 1) {
        printf("%d ", bv_get_bit(bv, i));
    }
}
