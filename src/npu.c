#include <stdio.h>
#include <stdlib.h>

#include "xil_cache.h"

volatile float a __attribute__((aligned(8)));
volatile float b __attribute__((aligned(8)));
volatile float c __attribute__((aligned(8)));

volatile uint32_t* flagreg = (uint32_t*)0x40000000;
volatile uintptr_t* reg_a = (uintptr_t*)0x40000004;
volatile uintptr_t* reg_b = (uintptr_t*)0x40000008;
volatile uintptr_t* reg_c = (uintptr_t*)0x4000000C;

void initialize() {
    *reg_a = (uintptr_t)&a;
    *reg_b = (uintptr_t)&b;
    *reg_c = (uintptr_t)&c;
}

void deinitialize() {

}

void do_plus(float* left, float* right, float* result, size_t count) {
    for (int i = 0; i < count; i++) {
        a = left[i];
        b = right[i];

        *flagreg = 0x00000001 | (0x00 << 8);

        while (*flagreg & 0x01)
            ;

        result[i] = c;
    }
}

void do_minus(float* left, float* right, float* result, size_t count) {
    for (int i = 0; i < count; i++) {
        a = left[i];
        b = right[i];

        *flagreg = 0x00000001 | (0x01 << 8);

        while (*flagreg & 0x01)
            ;

        result[i] = c;
    }
}

void do_multiply(float* left, float* right, float* result, size_t count) {
    for (int i = 0; i < count; i++) {
        a = left[i];
        b = right[i];

        *flagreg = 0x00000001 | (0x02 << 8);

        while (*flagreg & 0x01)
            ;

        result[i] = c;
    }
}

void do_divide(float* left, float* right, float* result, size_t count) {
    for (int i = 0; i < count; i++) {
        a = left[i];
        b = right[i];

        *flagreg = 0x00000001 | (0x03 << 8);

        while (*flagreg & 0x01)
            ;

        result[i] = c;
    }
}
