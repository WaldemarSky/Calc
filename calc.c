#include <stdio.h>
#include <string.h>
#include "calc_functions.h"
#include "calc_errors.h"
#include "calc_list_items.h"
#include "trie.h"

#define prompt ">>> "

void solve(const char *buf, struct trie_node **npr)
{
    struct calc_list *list;
    list = make_list_items(buf);
    struct calc_item *ptr = list->first;


    myfloat d;
    while(ptr) { 
        calc_err = 0;
        d = solve_expr(&ptr, npr);
        printf(" = %LG", d); 
        if(calc_err != 0)
            error_handler();
        puts("");
    }
}

void merge_args(char *buf, char **argv)
{
    argv++;
    while(*argv) {
        strcpy(buf, *argv);
        buf += strlen(*argv);
        argv++;
    }
}

int main(int argc, char **argv)
{
    char calc_buf[1024];

    struct trie_node *first;
    first = trie_node_init();
    add_gen_value(&first);

    if(argc == 1) {
        for(;;) {
            fputs(prompt, stderr);
            fgets(calc_buf, sizeof(calc_buf), stdin);
            solve(calc_buf, &first);
        }
    }
    else if(argc == 2) {
        solve(argv[1], &first);
    }
    else { 
        merge_args(calc_buf, argv);
        solve(calc_buf, &first);
    }
   
   return 0; 
}
