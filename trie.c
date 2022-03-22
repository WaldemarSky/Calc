#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "trie.h"

struct trie_node *trie_node_init()
{
    struct trie_node *ptr;
    ptr = malloc(sizeof(struct trie_node));
    ptr->tag = '\0';
    ptr->is_leaf = 0;

    ptr->this_lvl = NULL;
    ptr->next_lvl = NULL;

    return ptr;
}

static void add_trie_node (const char *name, struct trie_node **ptr, myfloat value)
{
    int is_leaf = *(name+1) == 0;
    if(is_leaf) {
        (*ptr)->is_leaf = 1;
        (*ptr)->value = value;
    }
    else {
        (*ptr)->is_leaf = 0;
        add_trie_value(name+1, &(*ptr)->next_lvl, value);
    }
}

myfloat add_trie_value(const char *name, struct trie_node **ptr, myfloat value)
{
    if(!*name) 
        return NAN;
    if(!(*ptr)) {
        *ptr = malloc(sizeof(struct trie_node));
        (*ptr)->tag = *name;
        (*ptr)->this_lvl = NULL;
        (*ptr)->next_lvl = NULL;
        add_trie_node(name, ptr, value);
        return value;
    }
    else {
        if((*ptr)->tag == *name) {
            add_trie_node(name, ptr, value);
        }
        else 
            return add_trie_value(name, &(*ptr)->this_lvl, value);
    }
    return NAN;
}


void add_gen_value(struct trie_node **ptr)
{
    add_trie_value("pi", ptr, M_PI);
    add_trie_value("e", ptr, M_E);
}

myfloat get_trie_value(const char *name, const struct trie_node *ptr)
{
    if(!ptr) 
        return NAN;
    if(ptr->tag == *name) {
        int is_leaf = *(name+1) == 0;
        if(is_leaf) {
            return ptr->value;
        }
        else
            return get_trie_value(name+1, ptr->next_lvl);
    }
    else {
        return get_trie_value(name, ptr->this_lvl);
    }
}


#if 0
    struct trie_node **tmp = ptr;
    tmp = &(*tmp)->next_lvl->this_lvl;
    while(*tmp) {
        printf("%c: ", (*tmp)->tag);
        if((*tmp)->is_leaf)
            printf("%f", (*tmp)->value);
        puts("");
        tmp = &(*tmp)->next_lvl;
    struct trie_node **tmp = ptr;
    tmp = &(*ptr)->this_lvl;
    tmp = &(*tmp)->this_lvl;
    while(*tmp) {
        printf("%c: ", (*tmp)->tag);
        if((*tmp)->is_leaf)
            printf("%f", (*tmp)->value);
        puts("");
        tmp = &(*tmp)->next_lvl;
    }

    }
#endif
