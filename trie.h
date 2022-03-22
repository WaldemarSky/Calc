#ifndef TRIE_SENTRY
#define TRIE_SENTRY
#include "calc_list_items.h"

struct trie_node {
    char tag;
    int is_leaf;
    myfloat value;

    struct trie_node *this_lvl;
    struct trie_node *next_lvl;

};


struct trie_node *trie_node_init();
myfloat add_trie_value(const char *name, struct trie_node **ptr, myfloat value);
myfloat get_trie_value(const char *name, const struct trie_node *ptr);
void add_gen_value(struct trie_node **ptr);

#endif
