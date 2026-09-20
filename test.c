#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef unsigned char u8;

void printb(u8* data, size_t size, bool little_endian) {
    for (int i = 0; i < size; i++) {
        u8 byte = little_endian ? data[i] : data[size - i - 1];
        for (int j = 7; j >= 0; j--) {
            printf("%d", (byte >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}


int main() {
    /*  There are 2 fundamental numeric data types in C:
    *       int & float, and their variations of different sizes
    *
    *   Others are types for specific behavior:
    *       pointers for manipulating with addresses
            _Bool for restricting a 1-byte int to only 2 values
            A built-in feature that maps a single-quoted charater to its ascii int value
            A built-in feature that converts string literals to \0-terminated character arrays (stored in .rodata)
            Arrays for contiguous storage of data
            Structs to group data
            Unions & Enums for specific behavior with data types
    */
    
}
