#include "THClTensorMath.h"
#include "THClGeneral.h"
//#include "THClBlas.h"
#include "THClTensorCopy.h"
//#include "THClTensorRandom.h"
#include "THClApply.h"
//#include "THClReduce.cuh"

#ifndef DIVUP
#define DIVUP(x, y) (((x) + (y) - 1) / (y))
#endif

#define IMPLEMENT_CL_TENSOR_BASIC_FUNC(NAME, CFUNC)                   \
  void THClTensor_##NAME(THClState* state, THClTensor* self_, THClTensor* src) { \
    THAssert(THClTensor_checkGPU(state, 2, self_, src));                \
    if (self_ == src) {                                                 \
      if (!THClTensor_pointwiseApply1(state, self_, Tensor##NAME##Op())) { \
        THArgCheck(false, 2, CLNN_DIM_WARNING); \
      }                                                                 \
    } else {                                                            \
      THClTensor_resizeAs(state, self_, src);                         \
                                                                        \
      if (!THClTensor_pointwiseApply2(state, self_, src, Tensor##NAME##Op())) { \
        THArgCheck(false, 2, CLNN_DIM_WARNING); \
      }                                                                 \
    }                                                                   \
                                                                        \
  }

//IMPLEMENT_CL_TENSOR_BASIC_FUNC(log, log)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(log1p, log1p)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(exp, exp)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(cos, cos)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(acos, acos)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(cosh, cosh)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(sin, sin)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(asin, asin)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(sinh, sinh)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(tan, tan)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(atan, atan)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(tanh, tanh)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(sqrt, sqrt)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(ceil, ceil)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(floor, floor)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(abs, fabs)
//IMPLEMENT_CL_TENSOR_BASIC_FUNC(round, roundf)

#undef IMPLEMENT_CL_TENSOR_BASIC_FUNC

struct TensorAddOp {
    std::string operator2() {
        return "*out += *in1";
    }
    std::string operator3() {
        return "*out = *in1 + *in2";
    }
//  __device__ __forceinline__ void operator()(float* out, float* in) {
//    *out += *in;
//  }

//  __device__ __forceinline__ void operator()(float* out, float* in1, float* in2) {
//    *out = *in1 + *in2;
//  }
};

struct TensorCAddOp {
  TensorCAddOp(float v) : val(v) {}
    std::string operator2() {
        return "*out += " + toString(val) + " * *in1";
    }
    std::string operator3() {
        return "*out += *in1 + " + toString(val) + " * *in2";
    }

//  __device__ __forceinline__ void operator()(float* out, float* in) {
//    *out += val * *in;
//  }

//  __device__ __forceinline__ void operator()(float* out, float* in1, float* in2) {
//    *out = *in1 + val * *in2;
//  }

  float val;
};

void THClTensor_cadd(THClState *state, THClTensor *self_, THClTensor* src1, float value, THClTensor *src2)
{
  THAssert(THClTensor_checkGPU(state, 3, self_, src1, src2));
  THArgCheck(THClTensor_nElement(state, src1) ==
             THClTensor_nElement(state, src2), 3, "sizes do not match");

  if (self_ == src1) {
    if (value == 1.0f) {
      // self += src2
      if (!THClTensor_pointwiseApply2(state, self_, src2, TensorAddOp())) {
        THArgCheck(false, 2, CLNN_DIM_WARNING);
      }
    } else {
      // self += value * src2
      if (!THClTensor_pointwiseApply2(state, self_, src2, TensorCAddOp(value))) {
        THArgCheck(false, 2, CLNN_DIM_WARNING);
      }
    }
  } else {
    THClTensor_resizeAs(state, self_, src1);

    if (value == 1.0f) {
      // self = src1 + src2
      if (!THClTensor_pointwiseApply3(state, self_, src1, src2, TensorAddOp())) {
        THArgCheck(false, 2, CLNN_DIM_WARNING);
      }
    } else {
      // self = src1 + value * src2
      if (!THClTensor_pointwiseApply3(state, self_, src1, src2, TensorCAddOp(value))) {
        THArgCheck(false, 2, CLNN_DIM_WARNING);
      }
    }
  }

//  THClCheck(cudaGetLastError());
}

struct TensorMulOp {
    std::string operator2() {
        return "*out *= *in1";
    }
    std::string operator3() {
        return "*out = *in1 * *in2";
    }
//  __device__ __forceinline__ void operator()(float* out, float* in) {
//    *out *= *in;
//  }

//  __device__ __forceinline__ void operator()(float* out, float* in1, float* in2) {
//    *out = *in1 * *in2;
//  }
};

void THClTensor_cmul(THClState *state, THClTensor *self_, THClTensor *src1, THClTensor *src2)
{
  THAssert(THClTensor_checkGPU(state, 3, self_, src1, src2));
  THArgCheck(THClTensor_nElement(state, src1) ==
             THClTensor_nElement(state, src2), 3, "sizes do not match");

  if (self_ == src1) {
    // self *= src2
    if (!THClTensor_pointwiseApply2(state, self_, src2, TensorMulOp())) {
      THArgCheck(false, 2, CLNN_DIM_WARNING);
    }
  } else {
    THClTensor_resizeAs(state, self_, src1);

    // self = src1 * src2
    if (!THClTensor_pointwiseApply3(state, self_, src1, src2, TensorMulOp())) {
      THArgCheck(false, 2, CLNN_DIM_WARNING);
    }
  }

//  THClCheck(cudaGetLastError());
}
