#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include "calc_list_items.h"

static int is_double_point(const char *buf)
{
    return isalnum(*buf) || *buf == '.' ||
        ((*buf == '+' || *buf == '-') && (*(buf-1) == 'e' || *(buf-1) == 'E'));

} 

static void insert_double(const char **buf, struct calc_list *list)
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

static void insert_name(const char **buf, struct calc_list *list)
{
    char name[sizeof(myfloat)];
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
    strcpy(tmp->name, name);
    if(0 == strcmp(tmp->name, "sin")) {
        tmp->c = 3;
    }
    else if(0 == strcmp(tmp->name, "cos")) {
        tmp->c = 4;
    }
    else if(0 == strcmp(tmp->name, "tan")) {
        tmp->c = 5;
    }
    else if(0 == strcmp(tmp->name, "ctg")) {
        tmp->c = 6;
    }
    else {
    tmp->c = 2;
    }
    tmp->next = NULL;
    if(list->last) {
        (*list).last->next = tmp;
        list->last = (*list).last->next;
    }
    else {
        list->first = list->last = tmp;
    }
    
}

static int is_terminator(char t)
{
    return t == '+' || t == '-' || t == '*' || t == '/' || t == ';' ||
        t == '%' || t == '=' || t == '!' || t == '(' || t == ')'|| t == '\'';

}

static void insert_term(const char **buf, struct calc_list *list)
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
