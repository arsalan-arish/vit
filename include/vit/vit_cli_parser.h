/*
    Validates the general parsed cli structure against vit-specifics
*/
#pragma once
#include <types.h>
#include <vit/subsystems/cli_parser.h>
#include <vit/commands/index.h>

typedef struct {
    b8 is;
    enum {
        VIT_COMMAND_NOT_FOUND = 1, 
    } code;
    u8 msg[200]; // Max message length is 200 bytes
} parseVitCli_err;

typedef struct {
    VIT_COMMAND command;
    u8** args;
    u8** flags;
} parseVitCli_ret;

typedef struct {
    parseVitCli_err err;
    parseVitCli_ret val;
} parseVitCli_free_err_ret;


parseVitCli_free_err_ret parseVitCli(int argc, char* argv[]);
void parseVitCli_free(parseVitCli_free_err_ret res);