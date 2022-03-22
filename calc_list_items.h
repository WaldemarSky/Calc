#ifndef CALC_LIST_ITEMS_SENTRY
#define CALC_LIST_ITEMS_SENTRY

typedef long double myfloat;

struct calc_item {
    char c;
    union {
        myfloat d;
        char name[sizeof(myfloat)];
    };
    struct calc_item *next;
};

struct calc_list {
    struct calc_item *first, *last;
};

struct calc_list *make_list_items(const char *buf);
#endif
