#include "bf.h"
#include "speck.h"
#include "salts.h"

#include <stdlib.h>
#include <stdio.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

// Creating the Bloom Filter and allocating memory.

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {

        // The salts are from the salts.h file.

        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

// Freeing all the allocated memory for the
// Bloom Filter.

void bf_delete(BloomFilter **bf) {
    if ((*bf)->filter && *bf) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
}

// Returns the size of the Bloom Filter.

uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// This is just setting the bits, hashing the oldspeak with all 3
// salts. This function does not return anything.

void bf_insert(BloomFilter *bf, char *oldspeak) {

    bv_set_bit(bf->filter, hash(bf->primary, oldspeak) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_size(bf));
}

// This function will check if all the bits are present
// for the oldspeak, will return true if they are all present,
// and false other wise.

bool bf_probe(BloomFilter *bf, char *oldspeak) {

    // Stored in variables to make code more clear.

    uint8_t first_bit = bv_get_bit(bf->filter, hash(bf->primary, oldspeak) % bf_size(bf));
    uint8_t second_bit = bv_get_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_size(bf));
    uint8_t third_bit = bv_get_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_size(bf));

    // If statement, if all the bits are set to 1, true will
    // be returned. Meaning it is likely in there. False is
    // returned otherwise.

    if (first_bit && second_bit && third_bit) {
        return true;
    } else {
        return false;
    }
}

// This function will return the number of bits
// in the Bloom Filter.

uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;

    // Loop that goes through and will add one
    // to the count if there is a bit in the index
    // position.

    for (uint32_t i = 0; i < bf_size(bf); i += 1) {
        if (bv_get_bit(bf->filter, i)) {
            count += 1;
        }
    }
    return count;
}

// This will print out the Bloom Filter.

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
