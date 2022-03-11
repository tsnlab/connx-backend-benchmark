#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include <xnnpack.h>

void do_plus(float* left, float* right, float* result, size_t count) {
    size_t shape[] = { count };

    enum xnn_status status = xnn_initialize(NULL);

    if (status != xnn_status_success) {
        printf("failed to initialize XNNPACK\n");
        return;
    }

    xnn_operator_t add_op = NULL;
    status = xnn_create_add_nd_f32(-FLT_MAX, FLT_MAX, 0, &add_op);

    if (status != xnn_status_success) {
        printf("failed to create add operator\n");
        return;
    }

    status = xnn_setup_add_nd_f32(add_op, 1, shape, 1, shape, left, right, result, NULL);

    if (status != xnn_status_success) {
        printf("failed to setup add operator\n");
        return;
    }

    status = xnn_run_operator(add_op, NULL);

    if (status != xnn_status_success) {
        printf("failed to run add operator\n");
        return;
    }

    status = xnn_delete_operator(add_op);
    if (status != xnn_status_success) {
        printf("failed to delete add operator\n");
        return;
    }
}

void do_minus(float* left, float* right, float* result, size_t count) {
    size_t shape[] = { count };

    enum xnn_status status = xnn_initialize(NULL);

    if (status != xnn_status_success) {
        printf("failed to initialize XNNPACK\n");
        return;
    }

    xnn_operator_t sub_op = NULL;
    status = xnn_create_subtract_nd_f32(-FLT_MAX, FLT_MAX, 0, &sub_op);

    if (status != xnn_status_success) {
        printf("failed to create subtract operator\n");
        return;
    }

    status = xnn_setup_subtract_nd_f32(sub_op, 1, shape, 1, shape, left, right, result, NULL);

    if (status != xnn_status_success) {
        printf("failed to setup subtract operator\n");
        return;
    }

    status = xnn_run_operator(sub_op, NULL);

    if (status != xnn_status_success) {
        printf("failed to run subtract operator\n");
        return;
    }

    status = xnn_delete_operator(sub_op);
    if (status != xnn_status_success) {
        printf("failed to delete subtract operator\n");
        return;
    }
}

void do_multiply(float* left, float* right, float* result, size_t count) {
    size_t shape[] = { count };

    enum xnn_status status = xnn_initialize(NULL);

    if (status != xnn_status_success) {
        printf("failed to initialize XNNPACK\n");
        return;
    }

    xnn_operator_t mul_op = NULL;
    status = xnn_create_multiply_nd_f32(-FLT_MAX, FLT_MAX, 0, &mul_op);

    if (status != xnn_status_success) {
        printf("failed to create multiply operator\n");
        return;
    }

    status = xnn_setup_multiply_nd_f32(mul_op, 1, shape, 1, shape, left, right, result, NULL);

    if (status != xnn_status_success) {
        printf("failed to setup multiply operator\n");
        return;
    }

    status = xnn_run_operator(mul_op, NULL);

    if (status != xnn_status_success) {
        printf("failed to run multiply operator\n");
        return;
    }

    status = xnn_delete_operator(mul_op);
    if (status != xnn_status_success) {
        printf("failed to delete multiply operator\n");
        return;
    }
}

void do_divide(float* left, float* right, float* result, size_t count) {
    size_t shape[] = { count };

    enum xnn_status status = xnn_initialize(NULL);

    if (status != xnn_status_success) {
        printf("failed to initialize XNNPACK\n");
        return;
    }

    xnn_operator_t div_op = NULL;
    status = xnn_create_divide_nd_f32(-FLT_MAX, FLT_MAX, 0, &div_op);

    if (status != xnn_status_success) {
        printf("failed to create divide operator\n");
        return;
    }

    status = xnn_setup_divide_nd_f32(div_op, 1, shape, 1, shape, left, right, result, NULL);

    if (status != xnn_status_success) {
        printf("failed to setup divide operator\n");
        return;
    }

    status = xnn_run_operator(div_op, NULL);

    if (status != xnn_status_success) {
        printf("failed to run divide operator\n");
        return;
    }

    status = xnn_delete_operator(div_op);
    if (status != xnn_status_success) {
        printf("failed to delete divide operator\n");
        return;
    }
}

