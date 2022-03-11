#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BATCH_SIZE 524288
#define BATCH_COUNT 200

void do_plus(float* left, float* right, float* result, size_t count);
void do_minus(float* left, float* right, float* result, size_t count);
void do_multiply(float* left, float* right, float* result, size_t count);
void do_divide(float* left, float* right, float* result, size_t count);

float rand_float32(void) {
  int32_t i = rand();
  float f = *(float*)&i;
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
    }

    struct timespec start, end, elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BATCH_COUNT; i++) {
        do_plus(left, right, result, BATCH_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);
    printf("Plus: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BATCH_COUNT; i++) {
        do_minus(left, right, result, BATCH_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);
    printf("Minus: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BATCH_COUNT; i++) {
        do_multiply(left, right, result, BATCH_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);
    printf("Multiply: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BATCH_COUNT; i++) {
        do_divide(left, right, result, BATCH_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);
    printf("Divide: %2lu.%09lu\n", elapsed.tv_sec, elapsed.tv_nsec);

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
