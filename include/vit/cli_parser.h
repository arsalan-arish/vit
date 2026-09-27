#pragma once
#include <vit/types.h>

typedef struct {
    b8 is;
} parse_cli_err;

typedef struct {
    u8* command;
    u8** args;
    u8** flags;
} parse_cli_ret;

typedef struct {
    parse_cli_err err;
    parse_cli_ret val;
} parse_cli_free_err_ret;


parse_cli_free_err_ret parse_cli(u8* argv[]);
void parse_cli_free(parse_cli_free_err_ret resource);