#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "calc_functions.h"
#include <math.h>
struct calc_item {
    char c;
    union {
        double d;
        char name[sizeof(double)];
    };
    struct calc_item *next;
};

struct calc_list {
    struct calc_item *first, *last;
};

enum errors { unknow_err = 1, div_by_zero, unexp_symbol };
static enum errors err_c;

static const char *error_list[] = {
    "---",
    "Unknown error",
    "Divide by zero",
    "Unexcepted symbol"
};

void error_handler(struct calc_item **ptr)
{
    printf(" Error: %s\n", error_list[err_c]);
}

int is_double_point(const char *buf)
{
    return isalnum(*buf) || *buf == '.' ||
        ((*buf == '+' || *buf == '-') && (*(buf-1) == 'e' || *(buf-1) == 'E'));

} 

void insert_double(const char **buf, struct calc_list *list)
{
    char double_str[1024];
    char *err_buf;
    double d;
    
    char *str_ptr = double_str;
    for(;is_double_point(*buf); str_ptr++, (*buf)++)
        *str_ptr = **buf;
    *str_ptr = '\0';
    d = strtod(double_str, &err_buf);
    if(*err_buf != '\0') { 
        printf("%s cannot be converted to a number\n", double_str);
        exit(1);
    }
    struct calc_item *tmp;
    tmp = malloc(sizeof(struct calc_item));
    tmp->c = 1;
    tmp->d = d;
    tmp->next = NULL;
    if(list->last) {
        (*list).last->next = tmp;
        list->last = (*list).last->next;
    }
    else {
        list->first = list->last = tmp;
    }
}

void insert_name(const char **buf, struct calc_list *list)
{
    char name[8];
    char *name_ptr = name;
    for(int i = 1; i < sizeof(name) && isalnum(**buf);
            name_ptr++, (*buf)++, i++) { 
        *name_ptr = **buf;
    }
    while(isalnum(**buf))
        (*buf)++;
    *name_ptr = '\0';
    struct calc_item *tmp;
    tmp = malloc(sizeof(struct calc_item));
    tmp->c = 2;
    strcpy(tmp->name, name);
    tmp->next = NULL;
    if(list->last) {
        (*list).last->next = tmp;
        list->last = (*list).last->next;
    }
    else {
        list->first = list->last = tmp;
    }
    
}

int is_terminator(char t)
{
    return t == '+' || t == '-' || t == '*' || t == '/' || t == ';' ||
        t == '%' || t == '=' || t == '!' || t == '(' || t == ')';

}

void insert_term(const char **buf, struct calc_list *list)
{
    if(is_terminator(**buf)) {
        struct calc_item *tmp;
        tmp = malloc(sizeof(struct calc_item));
        tmp->c = **buf;
        tmp->next = NULL;
        if(list->last) {
            (*list).last->next = tmp;
            list->last = (*list).last->next;
        }
        else {
            list->first = list->last = tmp;
        }
        (*buf)++;
    }
    else if(**buf != '\0') {
        (*buf)++;
    }
}

struct calc_list *make_list_items(const char *buf)
{
    struct calc_list *list;
    list = malloc(sizeof(struct calc_list));

    while(*buf) {
        if(isdigit(*buf))
            insert_double(&buf, list);
        if(isalpha(*buf))
            insert_name(&buf, list);
        insert_term(&buf, list);
    }

    return list;
}


void print_item(struct calc_item **ptr)
{
    switch((*ptr)->c) {
    case 1:
        printf("%G", (*ptr)->d);
        break;
    case 2:
        printf("%s", (*ptr)->name);
        break;
    default:
        printf("%c", (*ptr)->c);
    }
}

void insert_print_item(struct calc_item **ptr)
{
    print_item(ptr);
    *ptr = (*ptr)->next;
}

double solve_expr(struct calc_item **ptr);

double solve_primary(struct calc_item **ptr)
{
    double d;
    if(*ptr) {
        switch((*ptr)->c) {
        case 1:
            d = (*ptr)->d;
            insert_print_item(ptr);
            return d;
        case '(':
            insert_print_item(ptr);
            return solve_expr(ptr);
        }
    }

    err_c = unexp_symbol;
    return NAN;
}

double solve_mult_div(struct calc_item **ptr)
{
    double d = solve_primary(ptr);
    while(*ptr && ((*ptr)->c == '*' || (*ptr)->c == '/')) {
        switch((*ptr)->c) {
        case '*':
            insert_print_item(ptr);
            d = d * solve_primary(ptr);
            break;
        case '/':
            insert_print_item(ptr);
            double dvdr = solve_primary(ptr);
            if(dvdr == 0)
                err_c = div_by_zero;
            d = d / dvdr;
            break;
        }
    }
    return d;
}

double solve_expr(struct calc_item **ptr)
{
    double d;

    if((*ptr)->c == '-') {
        insert_print_item(ptr);
        d = -solve_mult_div(ptr);
    }
    else {
        d = solve_mult_div(ptr);
    }

    while(*ptr) {
        switch((*ptr)->c) {
        case '+':
            insert_print_item(ptr);
            d = d + solve_mult_div(ptr);
            break;
        case '-':
            insert_print_item(ptr);
            d = d - solve_mult_div(ptr);
            break;
        case ')':
            insert_print_item(ptr);
            return d;
        case ';':
            *ptr = (*ptr)->next;
            return d;
        }
    }
    return d;
}

void solve(const char *buf)
{
    struct calc_list *list;
    list = make_list_items(buf);
    struct calc_item *tmp = list->first;

    double d;

    while(tmp) { 
        err_c = 0;
        d = solve_expr(&tmp);
        printf("=%G", d); 
        if(err_c != 0)
            error_handler(&tmp);
        puts("");
    }
    
}
