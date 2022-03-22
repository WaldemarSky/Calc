#include <stdio.h>
#include "calc_errors.h"


enum calc_errors calc_err;

static const char *error_list[] = {
    "Unknown error",
    "Dividing by zero",
    "Remainder of dividing by zero",
    "Unexcepted symbol"
};

void error_handler()
{
    printf(" Error: %s", error_list[calc_err]);
}
