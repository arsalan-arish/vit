#include <stdio.h>
#include <types.h>

void printb(const u8* data, usize size, b8 little_endian) {
    for (usize i = 0; i < size; i++) {
        u8 byte = little_endian ? data[i] : data[size - i - 1];
        for (isize j = 7; j >= 0; j--) {
            printf("%d", (byte >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}