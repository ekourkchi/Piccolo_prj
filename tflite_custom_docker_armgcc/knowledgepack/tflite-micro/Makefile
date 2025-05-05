# TARGET_TOOLCHAIN_ROOT := tensorflow/lite/micro/tools/make/downloads/gcc_embedded/bin/
TARGET_TOOLCHAIN_ROOT := /usr/local/gcc-arm-none-eabi-10.3-2021.10/bin/
TARGET_TOOLCHAIN_PREFIX := arm-none-eabi-

# These are microcontroller-specific rules for converting the ELF output
# of the linker into a binary image that can be loaded directly.
CXX             := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)g++'
CC              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)gcc'
AS              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)as'
AR              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)ar' 
LD              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)ld'
NM              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)nm'
OBJDUMP         := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)objdump'
OBJCOPY         := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)objcopy'
SIZE            := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)size'

RM = rm -f
ARFLAGS := -csr
LDFLAGS += -Wl,--fatal-warnings -Wl,--gc-sections -lm 

# FILL_HERE

MICROLITE_CC_SRCS := tensorflow/lite/micro/cortex_m_generic/debug_log.cc tensorflow/lite/micro/fake_micro_context.cc tensorflow/lite/micro/flatbuffer_utils.cc tensorflow/lite/micro/memory_helpers.cc tensorflow/lite/micro/micro_allocation_info.cc tensorflow/lite/micro/micro_allocator.cc tensorflow/lite/micro/micro_api.cc tensorflow/lite/micro/micro_context.cc tensorflow/lite/micro/micro_interpreter.cc tensorflow/lite/micro/micro_interpreter_context.cc tensorflow/lite/micro/micro_interpreter_graph.cc tensorflow/lite/micro/micro_log.cc tensorflow/lite/micro/micro_op_resolver.cc tensorflow/lite/micro/micro_profiler.cc tensorflow/lite/micro/micro_resource_variable.cc tensorflow/lite/micro/cortex_m_generic/micro_time.cc tensorflow/lite/micro/micro_utils.cc tensorflow/lite/micro/mock_micro_graph.cc tensorflow/lite/micro/recording_micro_allocator.cc tensorflow/lite/micro/system_setup.cc tensorflow/lite/micro/test_helper_custom_ops.cc tensorflow/lite/micro/test_helpers.cc tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.cc tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.cc tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.cc tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.cc tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc tensorflow/lite/micro/memory_planner/linear_memory_planner.cc tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.cc tensorflow/lite/micro/tflite_bridge/flatbuffer_conversions_bridge.cc tensorflow/lite/micro/tflite_bridge/micro_error_reporter.cc tensorflow/lite/schema/schema_utils.cc tensorflow/lite/kernels/internal/tensor_ctypes.cc tensorflow/lite/kernels/internal/common.cc tensorflow/lite/kernels/internal/portable_tensor_utils.cc tensorflow/lite/kernels/internal/tensor_utils.cc tensorflow/lite/kernels/internal/reference/portable_tensor_utils.cc tensorflow/lite/kernels/internal/reference/comparisons.cc tensorflow/lite/kernels/internal/quantization_util.cc tensorflow/lite/kernels/kernel_util.cc tensorflow/lite/core/api/flatbuffer_conversions.cc tensorflow/lite/core/api/tensor_utils.cc tensorflow/lite/core/api/error_reporter.cc tensorflow/lite/core/c/common.cc
MICROLITE_CC_KERNEL_SRCS := signal/micro/kernels/delay.cc signal/micro/kernels/energy.cc signal/micro/kernels/fft_auto_scale.cc signal/micro/kernels/filter_bank.cc signal/micro/kernels/filter_bank_log.cc signal/micro/kernels/filter_bank_square_root.cc signal/micro/kernels/filter_bank_spectral_subtraction.cc signal/micro/kernels/framer.cc signal/micro/kernels/irfft.cc signal/micro/kernels/rfft.cc signal/micro/kernels/stacker.cc signal/micro/kernels/overlap_add.cc signal/micro/kernels/pcan.cc signal/micro/kernels/window.cc signal/src/circular_buffer.cc signal/src/energy.cc signal/src/fft_auto_scale.cc signal/src/filter_bank.cc signal/src/filter_bank_log.cc signal/src/filter_bank_square_root.cc signal/src/filter_bank_spectral_subtraction.cc signal/src/irfft_float.cc signal/src/irfft_int16.cc signal/src/irfft_int32.cc signal/src/log.cc signal/src/max_abs.cc signal/src/msb_32.cc signal/src/msb_64.cc signal/src/overlap_add.cc signal/src/pcan_argc_fixed.cc signal/src/rfft_float.cc signal/src/rfft_int16.cc signal/src/rfft_int32.cc signal/src/square_root_32.cc signal/src/square_root_64.cc signal/src/window.cc tensorflow/lite/micro/kernels/activations.cc tensorflow/lite/micro/kernels/activations_common.cc tensorflow/lite/micro/kernels/cmsis_nn/add.cc tensorflow/lite/micro/kernels/add_common.cc tensorflow/lite/micro/kernels/add_n.cc tensorflow/lite/micro/kernels/arg_min_max.cc tensorflow/lite/micro/kernels/assign_variable.cc tensorflow/lite/micro/kernels/batch_matmul.cc tensorflow/lite/micro/kernels/batch_to_space_nd.cc tensorflow/lite/micro/kernels/broadcast_args.cc tensorflow/lite/micro/kernels/broadcast_to.cc tensorflow/lite/micro/kernels/call_once.cc tensorflow/lite/micro/kernels/cast.cc tensorflow/lite/micro/kernels/ceil.cc tensorflow/lite/micro/kernels/circular_buffer.cc tensorflow/lite/micro/kernels/circular_buffer_common.cc tensorflow/lite/micro/kernels/comparisons.cc tensorflow/lite/micro/kernels/concatenation.cc tensorflow/lite/micro/kernels/cmsis_nn/conv.cc tensorflow/lite/micro/kernels/conv_common.cc tensorflow/lite/micro/kernels/cumsum.cc tensorflow/lite/micro/kernels/depth_to_space.cc tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cc tensorflow/lite/micro/kernels/depthwise_conv_common.cc tensorflow/lite/micro/kernels/dequantize.cc tensorflow/lite/micro/kernels/dequantize_common.cc tensorflow/lite/micro/kernels/detection_postprocess.cc tensorflow/lite/micro/kernels/div.cc tensorflow/lite/micro/kernels/elementwise.cc tensorflow/lite/micro/kernels/elu.cc tensorflow/lite/micro/kernels/embedding_lookup.cc tensorflow/lite/micro/kernels/ethosu.cc tensorflow/lite/micro/kernels/exp.cc tensorflow/lite/micro/kernels/expand_dims.cc tensorflow/lite/micro/kernels/fill.cc tensorflow/lite/micro/kernels/floor.cc tensorflow/lite/micro/kernels/floor_div.cc tensorflow/lite/micro/kernels/floor_mod.cc tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cc tensorflow/lite/micro/kernels/fully_connected_common.cc tensorflow/lite/micro/kernels/gather.cc tensorflow/lite/micro/kernels/gather_nd.cc tensorflow/lite/micro/kernels/hard_swish.cc tensorflow/lite/micro/kernels/hard_swish_common.cc tensorflow/lite/micro/kernels/if.cc tensorflow/lite/micro/kernels/kernel_runner.cc tensorflow/lite/micro/kernels/kernel_util.cc tensorflow/lite/micro/kernels/l2norm.cc tensorflow/lite/micro/kernels/l2_pool_2d.cc tensorflow/lite/micro/kernels/leaky_relu.cc tensorflow/lite/micro/kernels/leaky_relu_common.cc tensorflow/lite/micro/kernels/logical.cc tensorflow/lite/micro/kernels/logical_common.cc tensorflow/lite/micro/kernels/logistic.cc tensorflow/lite/micro/kernels/logistic_common.cc tensorflow/lite/micro/kernels/log_softmax.cc tensorflow/lite/micro/kernels/lstm_eval.cc tensorflow/lite/micro/kernels/lstm_eval_common.cc tensorflow/lite/micro/kernels/maximum_minimum.cc tensorflow/lite/micro/kernels/micro_tensor_utils.cc tensorflow/lite/micro/kernels/mirror_pad.cc tensorflow/lite/micro/kernels/cmsis_nn/mul.cc tensorflow/lite/micro/kernels/mul_common.cc tensorflow/lite/micro/kernels/neg.cc tensorflow/lite/micro/kernels/pack.cc tensorflow/lite/micro/kernels/pad.cc tensorflow/lite/micro/kernels/cmsis_nn/pooling.cc tensorflow/lite/micro/kernels/pooling_common.cc tensorflow/lite/micro/kernels/prelu.cc tensorflow/lite/micro/kernels/prelu_common.cc tensorflow/lite/micro/kernels/quantize.cc tensorflow/lite/micro/kernels/quantize_common.cc tensorflow/lite/micro/kernels/read_variable.cc tensorflow/lite/micro/kernels/reduce.cc tensorflow/lite/micro/kernels/reduce_common.cc tensorflow/lite/micro/kernels/reshape.cc tensorflow/lite/micro/kernels/reshape_common.cc tensorflow/lite/micro/kernels/resize_bilinear.cc tensorflow/lite/micro/kernels/resize_nearest_neighbor.cc tensorflow/lite/micro/kernels/round.cc tensorflow/lite/micro/kernels/select.cc tensorflow/lite/micro/kernels/shape.cc tensorflow/lite/micro/kernels/slice.cc tensorflow/lite/micro/kernels/cmsis_nn/softmax.cc tensorflow/lite/micro/kernels/softmax_common.cc tensorflow/lite/micro/kernels/space_to_batch_nd.cc tensorflow/lite/micro/kernels/space_to_depth.cc tensorflow/lite/micro/kernels/split.cc tensorflow/lite/micro/kernels/split_v.cc tensorflow/lite/micro/kernels/squared_difference.cc tensorflow/lite/micro/kernels/squeeze.cc tensorflow/lite/micro/kernels/strided_slice.cc tensorflow/lite/micro/kernels/strided_slice_common.cc tensorflow/lite/micro/kernels/sub.cc tensorflow/lite/micro/kernels/sub_common.cc tensorflow/lite/micro/kernels/cmsis_nn/svdf.cc tensorflow/lite/micro/kernels/svdf_common.cc tensorflow/lite/micro/kernels/tanh.cc tensorflow/lite/micro/kernels/transpose.cc tensorflow/lite/micro/kernels/transpose_conv.cc tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.cc tensorflow/lite/micro/kernels/unpack.cc tensorflow/lite/micro/kernels/var_handle.cc tensorflow/lite/micro/kernels/while.cc tensorflow/lite/micro/kernels/zeros_like.cc signal/src/kiss_fft_wrappers/kiss_fft_float.cc signal/src/kiss_fft_wrappers/kiss_fft_int16.cc signal/src/kiss_fft_wrappers/kiss_fft_int32.cc
THIRD_PARTY_CC_SRCS := 
THIRD_PARTY_KERNEL_CC_SRCS := third_party/cmsis_nn/Source/ActivationFunctions/arm_nn_activation_s16.c third_party/cmsis_nn/Source/ActivationFunctions/arm_relu6_s8.c third_party/cmsis_nn/Source/ActivationFunctions/arm_relu_q15.c third_party/cmsis_nn/Source/ActivationFunctions/arm_relu_q7.c third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_add_s16.c third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_add_s8.c third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_mul_s16.c third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_mul_s16_s8.c third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_mul_s8.c third_party/cmsis_nn/Source/ConcatenationFunctions/arm_concatenation_s8_w.c third_party/cmsis_nn/Source/ConcatenationFunctions/arm_concatenation_s8_x.c third_party/cmsis_nn/Source/ConcatenationFunctions/arm_concatenation_s8_y.c third_party/cmsis_nn/Source/ConcatenationFunctions/arm_concatenation_s8_z.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_1_x_n_s8.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_1x1_s8.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_1x1_s8_fast.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_fast_s16.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_get_buffer_sizes_s16.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_get_buffer_sizes_s8.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_s16.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_s8.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_wrapper_s16.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_wrapper_s8.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_3x3_s8.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_fast_s16.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_get_buffer_sizes_s16.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_get_buffer_sizes_s8.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_s16.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_s8.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_s8_opt.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_wrapper_s16.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_wrapper_s8.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_nn_depthwise_conv_s8_core.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.c third_party/cmsis_nn/Source/ConvolutionFunctions/arm_nn_mat_mult_s8.c third_party/cmsis_nn/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s16.c third_party/cmsis_nn/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s8.c third_party/cmsis_nn/Source/FullyConnectedFunctions/arm_fully_connected_s16.c third_party/cmsis_nn/Source/FullyConnectedFunctions/arm_fully_connected_s8.c third_party/cmsis_nn/Source/FullyConnectedFunctions/arm_vector_sum_s8.c third_party/cmsis_nn/Source/LSTMFunctions/arm_lstm_unidirectional_s8_s16.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s16.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s8_s16.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s8_s16.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_update_cell_state_s16.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_update_output_s8_s16.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_kernel_s16.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s8.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_nntables.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.c third_party/cmsis_nn/Source/NNSupportFunctions/arm_s8_to_s16_unordered_with_offset.c third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s16.c third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s8.c third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s16.c third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s8.c third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s16.c third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s8.c third_party/cmsis_nn/Source/ReshapeFunctions/arm_reshape_s8.c third_party/cmsis_nn/Source/SVDFunctions/arm_svdf_get_buffer_sizes_s8.c third_party/cmsis_nn/Source/SVDFunctions/arm_svdf_s8.c third_party/cmsis_nn/Source/SVDFunctions/arm_svdf_state_s16_s8.c third_party/cmsis_nn/Source/SoftmaxFunctions/arm_nn_softmax_common_s8.c third_party/cmsis_nn/Source/SoftmaxFunctions/arm_softmax_s16.c third_party/cmsis_nn/Source/SoftmaxFunctions/arm_softmax_s8.c third_party/cmsis_nn/Source/SoftmaxFunctions/arm_softmax_s8_s16.c third_party/cmsis_nn/Source/SoftmaxFunctions/arm_softmax_u8.c

CORE_OPTIMIZATION_LEVEL := -Os
THIRD_PARTY_KERNEL_OPTIMIZATION_LEVEL := -O2
KERNEL_OPTIMIZATION_LEVEL := -O2

OBJ_DIR := obj/
CORE_OBJDIR := $(OBJ_DIR)core/
KERNEL_OBJDIR := $(OBJ_DIR)kernels/
THIRD_PARTY_KERNEL_OBJDIR := $(OBJ_DIR)third_party_kernels/
THIRD_PARTY_OBJDIR := $(OBJ_DIR)third_party/

CXXFLAGS := -std=c++11 -fno-rtti -fno-exceptions -fno-use-cxa-atexit -fno-threadsafe-statics -Wnon-virtual-dtor -Werror -fno-unwind-tables -ffunction-sections -fdata-sections -fmessage-length=0 -DTF_LITE_STATIC_MEMORY -DTF_LITE_DISABLE_X86_NEON -Wsign-compare -Wdouble-promotion -Wunused-variable -Wunused-function -Wswitch -Wvla -Wall -Wextra -Wmissing-field-initializers -Wstrict-aliasing -Wno-unused-parameter -DCMSIS_NN -DNDEBUG -DTF_LITE_STRIP_ERROR_STRINGS -mcpu=cortex-m4 -mfpu=auto -DTF_LITE_MCU_DEBUG_LOG -mthumb -mfloat-abi=hard -funsigned-char -mlittle-endian -Wno-type-limits -Wno-unused-private-field -fomit-frame-pointer -MD -DCPU_M4=1 -DARM_MATH_DSP -DTARGET_CPU=cortex-m4 -DCMSIS_DEVICE_ARM_CORTEX_M_XX_HEADER_FILE="\"ARMCM4_FP.h\"" -DARMCM4
CCFLAGS := -Wimplicit-function-declaration -std=c11 -Werror -fno-unwind-tables -ffunction-sections -fdata-sections -fmessage-length=0 -DTF_LITE_STATIC_MEMORY -DTF_LITE_DISABLE_X86_NEON -Wsign-compare -Wdouble-promotion -Wunused-variable -Wunused-function -Wswitch -Wvla -Wall -Wextra -Wmissing-field-initializers -Wstrict-aliasing -Wno-unused-parameter -DCMSIS_NN -DNDEBUG -DTF_LITE_STRIP_ERROR_STRINGS -mcpu=cortex-m4 -mfpu=auto -DTF_LITE_MCU_DEBUG_LOG -mthumb -mfloat-abi=hard -funsigned-char -mlittle-endian -Wno-type-limits -Wno-unused-private-field -fomit-frame-pointer -MD -DCPU_M4=1 -DARM_MATH_DSP -DTARGET_CPU=cortex-m4 -DCMSIS_DEVICE_ARM_CORTEX_M_XX_HEADER_FILE="\"ARMCM4_FP.h\"" -DARMCM4
INCLUDES := -I. -DPROJECT_GENERATION -I./ -Ithird_party/cmsis -Ithird_party/cmsis/CMSIS/Core/Include -Ithird_party/cmsis/Device/ARM/ARMCM4/Include -Ithird_party/cmsis_nn -Ithird_party/cmsis_nn/Include -Ithird_party/cmsis_nn/Include/ -Ithird_party/flatbuffers/include -Ithird_party/gemmlowp -Ithird_party/kissfft -Ithird_party/ruy -I./ -DPROJECT_GENERATION 

MICROLITE_LIB_OBJS := $(addprefix $(CORE_OBJDIR), \
$(patsubst %.S,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(MICROLITE_CC_SRCS)))))

MICROLITE_THIRD_PARTY_OBJS := $(addprefix $(THIRD_PARTY_OBJDIR), \
$(patsubst %.S,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(THIRD_PARTY_CC_SRCS)))))

MICROLITE_THIRD_PARTY_KERNEL_OBJS := $(addprefix $(THIRD_PARTY_KERNEL_OBJDIR), \
$(patsubst %.S,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(THIRD_PARTY_KERNEL_CC_SRCS)))))

MICROLITE_KERNEL_OBJS := $(addprefix $(KERNEL_OBJDIR), \
$(patsubst %.S,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(MICROLITE_CC_KERNEL_SRCS)))))

$(CORE_OBJDIR)%.o: %.cc 
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CORE_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(CORE_OBJDIR)%.o: %.c 
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(CORE_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(CORE_OBJDIR)%.o: %.S 
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS)  $(LDFLAGS) $(CORE_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(THIRD_PARTY_OBJDIR)%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CORE_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(THIRD_PARTY_OBJDIR)%.o: %.c 
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(CORE_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(THIRD_PARTY_OBJDIR)%.o: %.S 
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(CORE_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(THIRD_PARTY_KERNEL_OBJDIR)%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(THIRD_PARTY_KERNEL_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(THIRD_PARTY_KERNEL_OBJDIR)%.o: %.c 
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(THIRD_PARTY_KERNEL_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(THIRD_PARTY_KERNEL_OBJDIR)%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(THIRD_PARTY_KERNEL_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(KERNEL_OBJDIR)%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(KERNEL_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(KERNEL_OBJDIR)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(KERNEL_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@

$(KERNEL_OBJDIR)%.o: %.S 
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(KERNEL_OPTIMIZATION_LEVEL) $(INCLUDES) -c $< -o $@


# library to be generated
MICROLITE_LIB := libtensorflow-microlite.a
MICROLITE_SO := libtensorflow-microlite.so

# Gathers together all the objects we've compiled into a single '.a' archive.
$(MICROLITE_LIB): $(MICROLITE_LIB_OBJS) $(MICROLITE_KERNEL_OBJS) $(MICROLITE_THIRD_PARTY_OBJS) $(MICROLITE_THIRD_PARTY_KERNEL_OBJS) $(MICROLITE_CUSTOM_OP_OBJS)
	@mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $(MICROLITE_LIB) $(MICROLITE_LIB_OBJS) \
	$(MICROLITE_KERNEL_OBJS) $(MICROLITE_THIRD_PARTY_OBJS) $(MICROLITE_THIRD_PARTY_KERNEL_OBJS) $(MICROLITE_CUSTOM_OP_OBJS)


# Creates a tensorflow-litemicro.a which excludes any example code.
$(MICROLITE_SO): $(MICROLITE_LIB_OBJS) $(MICROLITE_KERNEL_OBJS) $(MICROLITE_THIRD_PARTY_OBJS) $(MICROLITE_THIRD_PARTY_KERNEL_OBJS) $(MICROLITE_CUSTOM_OP_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) -shared  -o $(MICROLITE_SO) $(MICROLITE_LIB_OBJS) \
	$(MICROLITE_KERNEL_OBJS) $(MICROLITE_THIRD_PARTY_OBJS) $(MICROLITE_THIRD_PARTY_KERNEL_OBJS) $(MICROLITE_CUSTOM_OP_OBJS)

lib: $(MICROLITE_LIB)

so: $(MICROLITE_SO)

project : $(MICROLITE_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-$(RM) -r $(OBJ_DIR)
	-$(RM) ${MICROLITE_LIB}
