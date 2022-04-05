#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <time.h>
#elif defined(__zynq__)
#include "xil_printf.h"
#include "xparameters.h"
#include "xtime_l.h"
#endif /* __zynq__ */

#define BATCH_SIZE 4096
#ifdef __npu__
#define BATCH_COUNT 10000
#else
#define BATCH_COUNT 100000
#endif /* __npu__ */

void initialize();
void deinitialize();
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

    initialize();

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

#ifdef __linux__
    struct timespec start, end, elapsed;
#elif defined(__zynq__)
    XTime start, end;
#endif /* __zynq__ */

    // Add
#ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &start);
#elif defined(__zynq__)
    XTime_GetTime(&start);
#endif /* __zynq__ */

    for (int i = 0; i < BATCH_COUNT; i++) {
        do_plus(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
    }

#ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);
#elif defined(__zynq__)
    XTime_GetTime(&end);
#endif /* __zynq__ */

    // validation
    for (int i = 0; i < BATCH_SIZE; i++) {
        if (result[i] != left[i] + right[i]) {
            printf("ERROR: [%d]: %f + %f = %f != %f\n", i, left[i], right[i], left[i] + right[i], result[i]);
        }
    }

#ifdef __linux__
    printf("Plus: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);
#elif defined(__zynq__)
    printf("Plus: %.9f\n", 1.0 * (end - start) / (COUNTS_PER_SECOND / 1000000) / 1000000);
#endif /* __zynq__ */

    // Subtract
#ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &start);
#elif defined(__zynq__)
    XTime_GetTime(&start);
#endif /* __zynq__ */

    for (int i = 0; i < BATCH_COUNT; i++) {
        do_minus(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
    }

#ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);
#elif defined(__zynq__)
    XTime_GetTime(&end);
#endif /* __zynq__ */

    // validation
    for (int i = 0; i < BATCH_SIZE; i++) {
        if (result[i] != left[i] - right[i]) {
            printf("ERROR: [%d]: %f - %f = %f != %f\n", i, left[i], right[i], left[i] - right[i], result[i]);
        }
    }

#ifdef __linux__
    printf("Minus: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);
#elif defined(__zynq__)
    printf("Minus: %.9f\n", 1.0 * (end - start) / (COUNTS_PER_SECOND / 1000000) / 1000000);
#endif /* __zynq__ */

    // Multiply
#ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &start);
#elif defined(__zynq__)
    XTime_GetTime(&start);
#endif /* __zynq__ */

    for (int i = 0; i < BATCH_COUNT; i++) {
        do_multiply(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
    }

#ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);
#elif defined(__zynq__)
    XTime_GetTime(&end);
#endif /* __zynq__ */

    // validation
    for (int i = 0; i < BATCH_SIZE; i++) {
        if (result[i] != left[i] * right[i]) {
            printf("ERROR: [%d]: %f - %f = %f != %f\n", i, left[i], right[i], left[i] * right[i], result[i]);
        }
    }

#ifdef __linux__
    printf("Multiply: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);
#elif defined(__zynq__)
    printf("Multiply: %.9f\n", 1.0 * (end - start) / (COUNTS_PER_SECOND / 1000000) / 1000000);
#endif /* __zynq__ */

    // Divide
#ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &start);
#elif defined(__zynq__)
    XTime_GetTime(&start);
#endif /* __zynq__ */

    for (int i = 0; i < BATCH_COUNT; i++) {
        do_divide(left, right, result, BATCH_SIZE);
        dummy(left, right, result, BATCH_SIZE);
    }

#ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);
#elif defined(__zynq__)
    XTime_GetTime(&end);
#endif /* __zynq__ */

    // validation
    for (int i = 0; i < BATCH_SIZE; i++) {
        if (result[i] != left[i] / right[i]) {
            printf("ERROR: [%d]: %f / %f = %f != %f\n", i, left[i], right[i], left[i] / right[i], result[i]);
        }
    }

#ifdef __linux__
    printf("Divide: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);
#elif defined(__zynq__)
    printf("Divide: %.9f\n", 1.0 * (end - start) / (COUNTS_PER_SECOND / 1000000) / 1000000);
#endif /* __zynq__ */

    deinitialize();

    return 0;
}

#ifdef __linux__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif /* __linux__ */
void dummy(float* left, float* right, float* result, size_t count) {
}
#ifdef __linux__
#pragma GCC pop_options
#endif /* __linux__*/
