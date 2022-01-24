#include <stdio.h>
#include <string.h>
#include "calc_functions.h"

#define prompt ">>> "

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

    if(argc == 1) {
        for(;;) {
            fputs(prompt, stdout);
            fgets(calc_buf, sizeof(calc_buf), stdin);
            solve(calc_buf);
        }
    }
    else if(argc == 2) {
        solve(argv[1]);
    }
    else { 
        merge_args(calc_buf, argv);
        solve(calc_buf);
    }
   
   return 0; 
}
