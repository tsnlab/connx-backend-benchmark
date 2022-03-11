#include <stdlib.h>

void do_plus(float* left, float* right, float* result, size_t count) {
    for (int i = 0; i < count; i++) {
        result[i] = left[i] + right[i];
    }
}

void do_minus(float* left, float* right, float* result, size_t count) {
    for (int i = 0; i < count; i++) {
        result[i] = left[i] - right[i];
    }
}

void do_multiply(float* left, float* right, float* result, size_t count) {
    for (int i = 0; i < count; i++) {
        result[i] = left[i] * right[i];
    }
}

void do_divide(float* left, float* right, float* result, size_t count) {
    for (int i = 0; i < count; i++) {
        result[i] = left[i] / right[i];
    }
}

