#ifndef CALC_ERRORS_SENTRY
#define CALC_ERRORS_SENTRY

enum calc_errors { 
    unknow_err, 
    div_by_zero, 
    rem_div_by_zero, 
    unexp_symbol
};

extern enum calc_errors calc_err;
void error_handler();

#endif
