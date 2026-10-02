/*
    A simple parser for CLI arguments into semantic structures


*/
#pragma once
#include <types.h>

typedef struct {
    // Positional Arguments (Always before KW ones)
    // Keyword Arguments (in the form of flags and the values ahead them)
    /*
        Conventions:
          - Each flag starts with --, and a corresponding shorthand flag exists starting with -
          - If a flag is not defined to take a value (boolean flag), and a value is given, raise error
          - A single '-' without any letter is 'the unnamed flag', often referring to stdin as a convention, it can take no value
          - Shorthand flags can be chained together (e.g -> -fSsL)
    */
    char* program;
    
} parseCli_ret;

parseCli_ret parseCli(i32 argc, u8* argv[]);
