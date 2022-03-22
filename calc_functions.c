#include <stdio.h>
#include <math.h>
#include "calc_functions.h"
#include "calc_errors.h"

static void print_item(struct calc_item **ptr)
{
    switch((*ptr)->c) {
    case 1:
        printf("%LG", (*ptr)->d);
        break;
    case 2: case 3: case 4: case 5: case 6:
        printf("%s", (*ptr)->name);
        break;
    default:
        printf("%c", (*ptr)->c);
    }
}

static void insert_print_item(struct calc_item **ptr)
{
    print_item(ptr);
    *ptr = (*ptr)->next;
}

static myfloat solve_primary(struct calc_item **ptr, struct trie_node **npr);
static myfloat solve_trig_function(struct calc_item **ptr, struct trie_node **npr);

static myfloat solve_item_name(struct calc_item **ptr, struct trie_node **npr)
{
    myfloat d;
    if((*ptr)->next && (*ptr)->next->c == '=') {
        struct calc_item *tmp = *ptr;
        insert_print_item(ptr);
        insert_print_item(ptr);
        d = solve_primary(ptr, npr);
        return add_trie_value(tmp->name, npr, d); 
    }
    d = get_trie_value((*ptr)->name, *npr);
    insert_print_item(ptr);
    return d;

}

static myfloat solve_trig_function(struct calc_item **ptr, struct trie_node **npr)
{
    myfloat res;
    char c = (*ptr)->c; 

    insert_print_item(ptr);
    res = solve_primary(ptr, npr);
    if((*ptr) && (*ptr)->c == '\'') {
        insert_print_item(ptr);
        res = res * M_PI / 180;
    }
    switch(c) {
    case 3:
        return sin(res);
    case 4:
        return cos(res);
    case 5:
        return tan(res);
    case 6:
        return cos(res)/sin(res);
    }
    return NAN;
}

static myfloat solve_primary(struct calc_item **ptr, struct trie_node **npr)
{
    myfloat d;
    if(*ptr) {
        switch((*ptr)->c) {
        case 1:
            d = (*ptr)->d;
            insert_print_item(ptr);
            return d;
        case 2:
            d = solve_item_name(ptr, npr);
            return d; 
        case 3: case 4: case 5: case 6:
            return solve_trig_function(ptr, npr);
        case '(':
            insert_print_item(ptr);
            return solve_expr(ptr, npr);
        }
    }

    calc_err = unexp_symbol;
    return NAN;
}

static myfloat solve_mult_div(struct calc_item **ptr, struct trie_node **npr)
{
    myfloat d = solve_primary(ptr, npr);
    while(*ptr && ((*ptr)->c == '*' || (*ptr)->c == '/' || (*ptr)->c == '%')) {
        switch((*ptr)->c) {
        case '*':
            insert_print_item(ptr);
            d = d * solve_primary(ptr, npr);
            break;
        case '/':
            insert_print_item(ptr);
            myfloat dvdr = solve_primary(ptr, npr);
            if(dvdr == 0)
                calc_err = div_by_zero;
            d = d / dvdr;
            break;
        case '%':
            insert_print_item(ptr);
            myfloat denum = solve_primary(ptr, npr);
            if(denum == 0)
                calc_err = rem_div_by_zero;
            d = fmod(d, denum);
            break;
        }
    }
    return d;
}

myfloat solve_expr(struct calc_item **ptr, struct trie_node **npr)
{
    myfloat d;

    if((*ptr)->c == '-') {
        insert_print_item(ptr);
        d = -solve_mult_div(ptr, npr);
    }
    else {
        d = solve_mult_div(ptr, npr);
    }

    while(*ptr) {
        switch((*ptr)->c) {
        case '+':
            insert_print_item(ptr);
            d = d + solve_mult_div(ptr, npr);
            break;
        case '-':
            insert_print_item(ptr);
            d = d - solve_mult_div(ptr, npr);
            break;
        case ')':
            insert_print_item(ptr);
            return d;
        case ';':
            *ptr = (*ptr)->next;
            return d;
        default:
            insert_print_item(ptr);
            calc_err = unexp_symbol;
            return NAN;
        }
    }
    return d;
}
