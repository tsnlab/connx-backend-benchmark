#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BATCH_SIZE 4096
#define BATCH_COUNT 100000

void do_plus(float* left, float* right, float* result, size_t count);
void do_minus(float* left, float* right, float* result, size_t count);
void do_multiply(float* left, float* right, float* result, size_t count);
void do_divide(float* left, float* right, float* result, size_t count);
void dummy(float* left, float* right, float* result, size_t count);

float rand_float32(void) {
    float f = 0.0;

    while (f == 0.0) {
        f = (float)rand() / (float)RAND_MAX  * 10 - 5; // -5.0 ~ 5.0
    }

    return f;
}

void timespec_diff(struct timespec* start, struct timespec* end,
                   struct timespec* result) {
    result->tv_sec = end->tv_sec - start->tv_sec;
    result->tv_nsec = end->tv_nsec - start->tv_nsec;
    if (result->tv_nsec < 0) {
        result->tv_sec -= 1;
        result->tv_nsec += 1000000000;
    }
}

int main(int argc, char *argv[]) {
    float left[BATCH_SIZE], right[BATCH_SIZE];
    float result[BATCH_SIZE];

    if (argc > 1) {
        srand(atoi(argv[1]));
    } else {
        srand(0);
    }

    for (int i = 0; i < BATCH_SIZE; i++) {
        left[i] = rand_float32();
        right[i] = rand_float32();
        result[i] = 0.0;
    }

    // Warming up
    for (int i = 0; i < 100; i++) {
        do_plus(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
        do_minus(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
        do_multiply(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
        do_divide(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
    }

    struct timespec start, end, elapsed;

    // Add
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BATCH_COUNT; i++) {
        do_plus(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);

    // validation
    for (int i = 0; i < BATCH_SIZE; i++) {
        if (result[i] != left[i] + right[i]) {
            printf("ERROR: [%d]: %f + %f = %f != %f\n", i, left[i], right[i], left[i] + right[i], result[i]);
        }
    }

    printf("Plus: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);

    // Subtract
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BATCH_COUNT; i++) {
        do_minus(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);

    // validation
    for (int i = 0; i < BATCH_SIZE; i++) {
        if (result[i] != left[i] - right[i]) {
            printf("ERROR: [%d]: %f - %f = %f != %f\n", i, left[i], right[i], left[i] - right[i], result[i]);
        }
    }

    printf("Minus: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);

    // Multiply
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BATCH_COUNT; i++) {
        do_multiply(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);

    // validation
    for (int i = 0; i < BATCH_SIZE; i++) {
        if (result[i] != left[i] * right[i]) {
            printf("ERROR: [%d]: %f - %f = %f != %f\n", i, left[i], right[i], left[i] * right[i], result[i]);
        }
    }

    printf("Multiply: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);

    // Divide
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BATCH_COUNT; i++) {
        do_divide(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);

    // validation
    for (int i = 0; i < BATCH_SIZE; i++) {
        if (result[i] != left[i] / right[i]) {
            printf("ERROR: [%d]: %f / %f = %f != %f\n", i, left[i], right[i], left[i] / right[i], result[i]);
        }
    }

    printf("Divide: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);

    return 0;
}

#pragma GCC push_options
#pragma GCC optimize ("O0")
void dummy(float* left, float* right, float* result, size_t count) {
}
#pragma GCC pop_options
