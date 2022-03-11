#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void do_plus(float* left, float* right, float* result, size_t count);
void do_minus(float* left, float* right, float* result, size_t count);
void do_multiply(float* left, float* right, float* result, size_t count);
void do_divide(float* left, float* right, float* result, size_t count);

float rand_float32(void) {
  int32_t i = rand();
  float f = *(float*)&i;
  return f;
}

int main(int argc, char *argv[]) {
    float left[4096], right[4096];
    float result[4096];

    if (argc > 1) {
        srand(atoi(argv[1]));
    } else {
        srand(0);
    }

    for (int i = 0; i < 4096; i++) {
        left[i] = rand_float32();
        right[i] = rand_float32();
    }

    struct timespec start, end;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    do_plus(left, right, result, 4096);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Plus: %1.14f\n", elapsed);

    clock_gettime(CLOCK_MONOTONIC, &start);
    do_plus(left, right, result, 4096);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Minus: %1.14f\n", elapsed);

    clock_gettime(CLOCK_MONOTONIC, &start);
    do_multiply(left, right, result, 4096);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Multiply: %1.14f\n", elapsed);

    clock_gettime(CLOCK_MONOTONIC, &start);
    do_divide(left, right, result, 4096);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Divide: %1.14f\n", elapsed);

    return 0;
}

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
