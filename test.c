#include <stdio.h>
#include <vit/types.h>


int main(void) {
    /*
        The point of 'unsigned' in C:
            - Some integer operations like division, bit shifts, comparisons, and type casting (width changing) are done with different CPU instructions based on whether it is signed or unsigned
            - So generally, when representing raw arbitrary byte data, always use unsigned ints for normal behavior. Signed ones have exclusive behavior with these operations
    */
}
