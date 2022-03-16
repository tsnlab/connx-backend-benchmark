#include <libxsmm.h>

static inline void do_op(const libxsmm_meltw_binary_type op_type, float* left, float* right, float* result, size_t count) {
    libxsmm_meltw_binary_param binary_param;
    libxsmm_meltw_binary_flags binary_flags;
    libxsmm_meltw_binary_shape binary_shape = libxsmm_create_meltw_binary_shape(
            count /* M */, 1/* N */,
            0 /*ldi*/,
            0 /*ldi*/,
            0 /*ldo*/,
            LIBXSMM_DATATYPE_F32 /*dtype_in*/,
            LIBXSMM_DATATYPE_F32 /*dtype_out*/,
            LIBXSMM_DATATYPE_F32 /*dtype_comp */);
    libxsmm_meltw_binary_type  binary_type = op_type;

    binary_param.in0.primary = left;
    binary_param.in1.primary = right;
    binary_param.out.primary = result;
    binary_flags = LIBXSMM_MELTW_FLAG_NONE;

    libxsmm_meltwfunction_binary binary_kernel = libxsmm_dispatch_meltw_binary_v2( binary_type, binary_shape, binary_flags );
    binary_kernel(&binary_param);
}

void do_plus(float* left, float* right, float* result, size_t count) {
    do_op(LIBXSMM_MELTW_TYPE_BINARY_ADD, left, right, result, count);
}

void do_minus(float* left, float* right, float* result, size_t count) {
    do_op(LIBXSMM_MELTW_TYPE_BINARY_SUB, left, right, result, count);
}

void do_multiply(float* left, float* right, float* result, size_t count) {
    do_op(LIBXSMM_MELTW_TYPE_BINARY_MUL, left, right, result, count);
}

void do_divide(float* left, float* right, float* result, size_t count) {
    do_op(LIBXSMM_MELTW_TYPE_BINARY_DIV, left, right, result, count);
}

