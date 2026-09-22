#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <vit/types.h>


static 
void printb(u8* const data, usize size, b8 little_endian) {
    for (usize i = 0; i < size; i++) {
        u8 byte = little_endian ? data[i] : data[size - i - 1];
        for (isize j = 7; j >= 0; j--) {
            printf("%d", (byte >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}


int main(void) {
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
    /*
        The point of 'unsigned' in C:
            - Some integer operations like division, bit shifts, comparisons, and type casting (width changing) are done with different CPU instructions based on whether it is signed or unsigned
            - So generally, when representing raw arbitrary byte data, always use unsigned ints for normal behavior. Signed ones have exclusive behavior with these operations
    */
    //! Type Casting in C
            
}
