#include "bf.h"
#include "messages.h"
#include "parser.h"
#include "ht.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

// Used to print help information and default
// information.

static char help_print[] = "SYNOPSIS\n"
                           "  A word filtering program for the GPRSC.\n"
                           "  Filters out and reports bad words parsed from stdin.\n\n"

                           "USAGE\n"
                           "  ./banhammer [-hs] [-t size] [-f size]\n\n"

                           "OPTIONS\n"
                           "  -h           Program usage and help.\n"
                           "  -s           Print program statistics.\n"
                           "  -t size      Specify hash table size (default: 2^16).\n"
                           "  -f size      Specify Bloom filter size (default: 2^20).\n";

// Used in regex to get specific words.

#define WORD "[a-zA-Z0-9_]+((-|')[a-zA-Z0-9_]+)*"

int main(int argc, char **argv) {
    int opt = 0;
    bool stats = false;
    uint32_t table_size = 65536;
    uint32_t filter_size = 1048576;
    while ((opt = getopt(argc, argv, "ht:f:s")) != -1) {
        switch (opt) {
        case 'h': printf("%s", help_print); return 0;
        case 't': table_size = atoi(optarg); break;
        case 'f': filter_size = atoi(optarg); break;
        case 's': stats = true; break;
        default: printf("%s", help_print); return 0;
        }
    }

    BloomFilter *bf = bf_create(filter_size);
    HashTable *ht = ht_create(table_size);

    FILE *infile = fopen("badspeak.txt", "r");

    char buf[4096];

    // Reading in from 'badspeak.txt' and inserting
    // into the Bloom Filter and Hash Table.

    while (fscanf(infile, "%s\n", buf) != -1) {
        bf_insert(bf, buf);
        ht_insert(ht, buf, NULL);
    }
    fclose(infile);

    infile = fopen("newspeak.txt", "r");

    char old_buf[4096], new_buf[4096];

    // Reading in from 'newspeak.txt' and inserting
    // into the Bloom Filter and Hash Table.

    while (fscanf(infile, "%s %s\n", old_buf, new_buf) != -1) {
        bf_insert(bf, old_buf);
        ht_insert(ht, old_buf, new_buf);
    }

    fclose(infile);
    Node *bad_speak = bst_create();
    Node *new_speak = bst_create();
    regex_t regular_expression;

    // This is taken from the example usage of regex within
    // the assignment 7 document. Page 11. Will stop program
    // if it can not be compiled.

    if (regcomp(&regular_expression, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return -1;
    }

    char *input = NULL;

    infile = stdin;

    // Taken from assignment 7 document of example on how to
    // get words from input. Page 11.

    Node *node_holder;
    while ((input = next_word(infile, &regular_expression)) != NULL) {

        // Making the input string into lower case
        // for case sensitivity.

        //printf("%s\n", input);
        for (uint64_t i = 0; input[i]; i += 1) {
            input[i] = tolower(input[i]);
        }

        if (bf_probe(bf, input)) {
            node_holder = ht_lookup(ht, input);

            // This if statement is for if there is
            // a false positive. Nothing will happen.

            if (node_holder == NULL) {

                // Do nothing.

            } else if (node_holder->newspeak == NULL) {
                bad_speak = bst_insert(bad_speak, node_holder->oldspeak, NULL);
            } else if (node_holder->newspeak != NULL) {
                new_speak = bst_insert(new_speak, node_holder->oldspeak, node_holder->newspeak);
            }
        }
    }

    fclose(infile);

    // If stats is true, it will print out the
    // following, and stop the program afterwards.

    if (stats) {
        printf("Average BST size: %.6f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %.6f\n", ht_avg_bst_height(ht));
        printf("Average branches traversed: %.6f\n", (double) branches / (double) lookups);
        printf("Hash table load: %.6f%%\n", 100 * ((double) ht_count(ht) / (double) ht_size(ht)));
        printf("Bloom filter load: %.6f%%\n", 100 * ((double) bf_count(bf) / (double) bf_size(bf)));

        // Freeing any and all of the allocated memory

        clear_words();
        regfree(&regular_expression);
        bf_delete(&bf);
        ht_delete(&ht);
        bst_delete(&new_speak);
        bst_delete(&bad_speak);
        return 0;
    }

    // This is where the 3 messages can be printed out.

    if (bst_size(new_speak) > 0 && bst_size(bad_speak) > 0) {
        printf("%s", mixspeak_message);
        bst_print(bad_speak);
        bst_print(new_speak);
    } else if (bst_size(new_speak) > 0 && bst_size(bad_speak) == 0) {
        printf("%s", goodspeak_message);
        bst_print(new_speak);
    } else if (bst_size(new_speak) == 0 && bst_size(bad_speak) > 0) {
        printf("%s", badspeak_message);
        bst_print(bad_speak);
    }

    // Freeing any and all of the allocated memory

    clear_words();
    regfree(&regular_expression);
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&new_speak);
    bst_delete(&bad_speak);
}
