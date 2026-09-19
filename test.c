#include <stdio.h>
#include <stdbool.h>

typedef unsigned char u8;

void printb(u8* data, size_t size, bool little_endian) {
    for (int i = 0; i < size; i++) {
        u8 byte = little_endian ? data[size - i - 1] : data[i];
        for (int j = 7; j >= 0; j--) {
            printf("%d", (byte >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}


int main() {
    char str[] = "hello";
    printb(str, sizeof(str), false);
}