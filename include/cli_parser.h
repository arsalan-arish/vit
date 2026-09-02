#pragma once
#include <stdbool.h>

typedef struct {
    bool is;
} parse_cli_err;

typedef struct {
    char* command;
    char** args;
    char** flags;
} parse_cli_ret; 

typedef struct {
    parse_cli_err err;
    parse_cli_ret val;
} parse_cli_err_ret;


parse_cli_err_ret parse_cli(int argc, char** argv);