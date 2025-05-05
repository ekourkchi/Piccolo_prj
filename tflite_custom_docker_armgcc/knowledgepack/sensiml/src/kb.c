/*
Copyright 2017-2024 SensiML Corporation

This file is part of SensiML™ Piccolo AI™.

SensiML Piccolo AI is free software: you can redistribute it and/or
modify it under the terms of the GNU Affero General Public License
as published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

SensiML Piccolo AI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public
License along with SensiML Piccolo AI. If not, see <https://www.gnu.org/licenses/>.
*/



#include <limits.h>

/* kb_classifier includes */
#include "kb_pipeline.h"
#include "kb_common.h"
#include "kb_classifier.h"
#include "tf_micro.h"
#include "tf_micro_trained_models.h"
#include "kb.h"
#include "kb_defines.h"

#define STARTER_EDITION_CHECK 1
#define MAX_RINGBUFFERS_USED 3

#define NUMBER_OF_PARENT_MODELS 1
#define NUMBER_OF_MODELS 1
#define NUMBER_OF_SEGMENTERS 1
#define NUMBER_OF_FEATURE_BANKS_0 1
#define NUMBER_OF_FEATURES_PER_BANK_0 47
#define TOTAL_FEATURES_0 47
#define OUTPUT_TENSORS_0 3
#define NUMBER_OF_RINGBUFFS_0 3
#define _RAWDATABUF_LEN_0 384
#define NUMBER_OF_SBUFFS_0 0
#define _RAWDATASBUF_LEN_0 0
static kb_model_t kb_models[NUMBER_OF_MODELS];
static seg_params segParams[NUMBER_OF_PARENT_MODELS];
static data_buffers_t data_buffers[NUMBER_OF_PARENT_MODELS];
static data_buffers_t sdata_buffers[NUMBER_OF_PARENT_MODELS];
static feature_bank_t feature_banks[NUMBER_OF_MODELS];
static feature_vector_t feature_vectors[NUMBER_OF_MODELS];
static model_results_t model_results[NUMBER_OF_MODELS];
static float_data_t output_tensors[NUMBER_OF_MODELS];
static int32_t rbuffers_len[NUMBER_OF_PARENT_MODELS] = {3};
static int32_t sbuffers_len[NUMBER_OF_PARENT_MODELS] = {0};
static const int32_t max_classifications[NUMBER_OF_MODELS] = {100000};
static uint8_t model_uuid_0[16] = {0x80,0x5c,0xd9,0x1f,0x7b,0x51,0x48,0x65,0x96,0x17,0xea,0xfa,0x6c,0xeb,0xbd,0x88};
static float pFeatures_0[TOTAL_FEATURES_0];
static uint8_t feature_vector_arr_0[FEATURE_VECTOR_SIZE_0];
static float output_tensor_0[OUTPUT_TENSORS_0];
static int16_t input_data_0[NUMBER_OF_RINGBUFFS_0];
int16_t RAW_DATA_BUFFER_0[_RAWDATABUF_LEN_0];
ring_buffer_t rbuffers_0[NUMBER_OF_RINGBUFFS_0];
int16_t RAW_SDATA_BUFFER_0[_RAWDATASBUF_LEN_0];
ring_buffer_t sbuffers_0[NUMBER_OF_SBUFFS_0];
static int16_t *input_data[NUMBER_OF_PARENT_MODELS] = {input_data_0};
int16_t *RAW_DATA_BUFFER[NUMBER_OF_PARENT_MODELS] = {RAW_DATA_BUFFER_0};
ring_buffer_t *rbuffers[NUMBER_OF_PARENT_MODELS] = {rbuffers_0};
int16_t *RAW_SDATA_BUFFER[NUMBER_OF_PARENT_MODELS] = {RAW_SDATA_BUFFER_0};
ring_buffer_t *sbuffers[NUMBER_OF_PARENT_MODELS] = {sbuffers_0};


#define SORTED_DATA_LEN 128
int16_t sortedData[SORTED_DATA_LEN];
#ifndef SORTED_DATA_LEN
#define SORTED_DATA_LEN 8192
int16_t sortedData[SORTED_DATA_LEN];
#endif

static bool reset_num_classes = true;

// need to do some checking on this
#if SML_DEBUG
#define PRINTBUFFLEN 256 + 4 * MAX_VECTOR_SIZE
static char printfbuff[PRINTBUFFLEN];
static char *pbuf = printfbuff;
#endif
static int16_t segment_sample_data[MAX_RINGBUFFERS_USED];

#if STARTER_EDITION_CHECK

static bool kb_check_starter_still_has_classifications(int32_t model_index)
{
    if (kb_models[model_index].total_classifications > max_classifications[model_index])
    {
        return false;
    }
    return true;
}
#endif

uint8_t get_feature_vector_type(int32_t model_index)
{
    return kb_models[model_index].pfeature_vector->typeID;
}

uint16_t kb_get_feature_vector_size(int32_t model_index)
{
    return kb_models[model_index].pfeature_vector->size;
}

feature_vector_t *get_feature_vector_pointer(int32_t model_index)
{
    return kb_models[model_index].pfeature_vector;
}

void get_feature_vector_data_pointer(int32_t model_index, void **fv_arr)
{
    if (get_feature_vector_type(model_index) == 1)
    {
        *fv_arr = (uint8_t *)kb_models[model_index].pfeature_vector->data;
    }
    else
    {
        *fv_arr = (float *)kb_models[model_index].pfeature_vector->data;
    }
}

void copy_feature_vector(int32_t model_index, void *fv_arr)
{
    uint16_t size = kb_get_feature_vector_size(model_index);
    if (get_feature_vector_type(model_index) == 1)
    {
        uint8_t *out_arr = (uint8_t *)fv_arr;
        uint8_t *in_arr = (uint8_t *)kb_models[model_index].pfeature_vector->data;
        for (int32_t i = 0; i < size; i++)
        {
            out_arr[i] = in_arr[i];
        }
    }
    else
    {
        float *out_arr = (float *)fv_arr;
        float *in_arr = (float *)kb_models[model_index].pfeature_vector->data;
        for (int32_t i = 0; i < size; i++)
        {
            out_arr[i] = in_arr[i];
        }
    }
}

void kb_set_feature_vector(int32_t model_index, void *fv_arr)
{
    uint16_t size = kb_get_feature_vector_size(model_index);
    if (get_feature_vector_type(model_index) == 1)
    {
        uint8_t *in_arr = (uint8_t *)fv_arr;
        uint8_t *out_arr = (uint8_t *)kb_models[model_index].pfeature_vector->data;
        for (int32_t i = 0; i < size; i++)
        {
            out_arr[i] = in_arr[i];
        }
    }
    else
    {
        float *in_arr = (float *)fv_arr;
        float *out_arr = (float *)kb_models[model_index].pfeature_vector->data;
        for (int32_t i = 0; i < size; i++)
        {
            out_arr[i] = in_arr[i];
        }
    }
}

void ring_buffer_init()
{
    int32_t cbuflen, ibuf, imodel, sbuflen;
    static int32_t init_framelen = 1;

    if (init_framelen)
    {
        init_framelen = 0;
        for (imodel = 0; imodel < NUMBER_OF_PARENT_MODELS; imodel++)
        {
            // Initialize the ring buffers (this will have to be more carefully done)
            switch (imodel)
            {
                case(0):
		cbuflen = _RAWDATABUF_LEN_0 / NUMBER_OF_RINGBUFFS_0;
sbuflen = 0;
break;
                // FILL_SRING_BUFFER_INIT_SIZES
            }
            for (ibuf = 0; ibuf < rbuffers_len[imodel]; ibuf++)
            {
                setup_rb(&rbuffers[imodel][ibuf], (rb_data_t *)&RAW_DATA_BUFFER[imodel][ibuf * cbuflen], cbuflen);
            }

            for (ibuf = 0; ibuf < sbuffers_len[imodel]; ibuf++) // Make this a fill
            {
                setup_rb(&sbuffers[imodel][ibuf], (rb_data_t *)&RAW_SDATA_BUFFER[imodel][ibuf * sbuflen], sbuflen);
            }
        }
    }
    return;
}

void kb_model_init()
{
    segParams[0].window_size = 70;
segParams[0].delta = 20;

    feature_vectors[0].typeID = 1;
feature_vectors[0].size = FEATURE_VECTOR_SIZE_0;
feature_vectors[0].data = feature_vector_arr_0;
output_tensors[0].size = OUTPUT_TENSORS_0;
output_tensors[0].data = output_tensor_0;
model_results[0].model_type = 1;
model_results[0].result = 0;
model_results[0].output_tensor = &output_tensors[0];
feature_banks[0].num_banks = NUMBER_OF_FEATURE_BANKS_0;
feature_banks[0].bank_size = NUMBER_OF_FEATURES_PER_BANK_0;
feature_banks[0].pFeatures = pFeatures_0;
feature_banks[0].filled_flag = false;
data_buffers[0].size = NUMBER_OF_RINGBUFFS_0;
data_buffers[0].data = rbuffers[0];
sdata_buffers[0].size = NUMBER_OF_SBUFFS_0;
sdata_buffers[0].data = sbuffers[0];
kb_models[0].model_uuid = model_uuid_0;
kb_models[0].parent = 0;
kb_models[0].sg_length = 0;
kb_models[0].sg_index = 0;
kb_models[0].last_read_idx = 0;
kb_models[0].feature_bank_index = 0;
kb_models[0].streaming_filter_length = 1;
kb_models[0].classifier_id = 0;
kb_models[0].classifier_type = 5;
kb_models[0].m_profile.enabled = 0;
kb_models[0].input_data = input_data_0;
kb_models[0].data_streaming = data_streaming_0;
kb_models[0].data_segmentation = data_segmentation_0;
kb_models[0].feature_gen = feature_gen_0;
kb_models[0].feature_transform = feature_transform_0;
kb_models[0].recognize_vector = recognize_vector_0;
kb_models[0].psdata_buffer = &sdata_buffers[0];
kb_models[0].pdata_buffer = &data_buffers[0];
kb_models[0].psegParams = &segParams[0];
kb_models[0].pfeature_bank = &feature_banks[0];
kb_models[0].pfeature_vector = &feature_vectors[0];
kb_models[0].pmodel_results = &model_results[0];
if(reset_num_classes){

kb_models[0].total_classifications = 0;
reset_num_classes = false;
}

    ring_buffer_init();

    	tf_micro_init(tf_micro_classifier_rows, 0);

}

int32_t kb_flush_model_buffer(int32_t model_index)
{
    int32_t parent = kb_models[model_index].parent;
    ringb *rb;

    for (int32_t i = 0; i < kb_models[parent].pdata_buffer->size; i++)
    {
        rb = &(kb_models[parent].pdata_buffer->data[i]);
        rb_reset(rb);
    }

    for (int32_t i = 0; i < kb_models[parent].psdata_buffer->size; i++)
    {
        rb = &(kb_models[parent].psdata_buffer->data[i]);
        rb_reset(rb);
    }

    kb_models[model_index].sg_length = 0;
    kb_models[model_index].sg_index = 0;
    kb_models[model_index].last_read_idx = 0;
    kb_models[model_index].feature_bank_index = 0;
    kb_models[model_index].pfeature_bank->filled_flag = false;

    

    return 1;
}

int32_t kb_flush_model(int32_t model_index)
{
    int32_t ret = 1;
    switch (kb_models[model_index].classifier_type)
    {
    	case(5):
		ret = 0;
		break;

    default:
        ret = 0;
    }
    return ret;
}

void kb_data_streaming_reset(int32_t model_index)
{
    switch(model_index)
{
case(0):
sg_windowing_init(&kb_models[model_index], kb_models[model_index].psegParams);
break;
}
    rb_unlock(kb_models[model_index].pdata_buffer->data);
}

int32_t kb_reset_model(int32_t model_index)
{
    kb_data_streaming_reset(model_index);

    return 1;
}

void kb_reset_feature_banks(int32_t model_index)
{
    kb_models[model_index].feature_bank_index = 0;
    kb_models[model_index].pfeature_bank->filled_flag = false;
}

bool kb_feature_bank_ready(int32_t model_index)
{
    return kb_models[model_index].pfeature_bank->filled_flag;
}

void kb_feature_generation_reset(int32_t model_index)
{
    int32_t start = kb_models[model_index].pfeature_bank->bank_size * kb_models[model_index].feature_bank_index;
    for (int32_t i = 0; i < kb_models[model_index].pfeature_bank->bank_size; i++)
    {
        kb_models[model_index].pfeature_bank->pFeatures[start + i] = 0.0f;
    }
    sorted_copy(kb_models[model_index].pdata_buffer->data, 0, 0, 1);
}

void kb_feature_generation_increment(int32_t model_index)
{
    kb_models[model_index].feature_bank_index += 1;

    if (kb_models[model_index].feature_bank_index == kb_models[model_index].pfeature_bank->num_banks - 1)
    {
        kb_models[model_index].pfeature_bank->filled_flag = true;
    }

    if (kb_models[model_index].feature_bank_index >= kb_models[model_index].pfeature_bank->num_banks)
    {
        kb_models[model_index].feature_bank_index = 0;
    }
}

uint16_t kb_recognize(int32_t model_index)
{
    int32_t ret;
#if STARTER_EDITION_CHECK
    if (kb_check_starter_still_has_classifications(model_index) == false)
    {
        return USHRT_MAX - 1;
    }
    kb_models[model_index].total_classifications++;
#endif
    ret = kb_models[model_index].recognize_vector(&kb_models[model_index]);
    return (uint16_t)ret;
}

uint16_t kb_feature_transform(int32_t model_index)
{
    int32_t ret;

    ret = kb_models[model_index].feature_transform(&kb_models[model_index]);

    return (uint16_t)ret;
}

int32_t kb_feature_generation(int32_t model_index)
{
    int32_t ret = -1;
    int32_t nfeats = 0;

    ret = kb_models[model_index].feature_gen(&kb_models[model_index], &nfeats);
    switch (ret)
    {
    case -1:
        break;
    case 1:
        break;
    default:
        break;
    }
    return ret;
}

int32_t kb_data_streaming(int16_t *pSample, int32_t nsensors, int32_t model_index)
{
    int32_t ret = 0;

    ret = kb_models[model_index].data_streaming(&kb_models[model_index], pSample);

    return ret;
}

int32_t kb_segmentation(int32_t model_index)
{
    int32_t ret = 0;

    ret = kb_models[model_index].data_segmentation(&kb_models, model_index);

    if (ret == 1)
    {
        rb_lock(kb_models[model_index].pdata_buffer->data);
    }

    return ret;
}

void kb_generate_children_features(int32_t model_index)
{
    if (model_index != kb_models[model_index].parent)
    {
        return;
    }

    for (int32_t index = 0; index < NUMBER_OF_MODELS; index++)
    {
        if (kb_models[index].parent == model_index && index != model_index)
        {
            if (kb_segmentation(index) == 1)
            {
                kb_feature_generation_reset(index);
                kb_feature_generation(index);
                kb_feature_generation_increment(index);
            }
        }
    }
}

int32_t kb_generate_classification(int32_t model_index)
{
    int32_t ret = -1;
    if (kb_feature_transform(model_index) == 1)
    {
        ret = kb_recognize(model_index);
        if (ret == USHRT_MAX)
        {
            ret = 0;
        }
    }

    return ret;
}

int32_t kb_run_model(int16_t *pSample, int32_t nsensors, int32_t model_index)
{
    int32_t ret = -1;
    if (kb_data_streaming(pSample, nsensors, model_index))
    {
        if (kb_segmentation(model_index) == 1)
        {
            kb_feature_generation_reset(model_index);
            if (kb_feature_generation(model_index) == 1)
            {
                ret = kb_generate_classification(model_index);
                kb_feature_generation_increment(model_index);
                return ret;
            }
            else
            {
                kb_reset_feature_banks(model_index);
                kb_reset_model(model_index);
                return -2;
            }
        }
    }
    return -1;
}

int32_t kb_run_model_with_cascade_features(int16_t *pSample, int32_t nsensors, int32_t model_index)
{
    int32_t ret = -1;
    if (kb_data_streaming(pSample, nsensors, model_index))
    {
        if (kb_segmentation(model_index) == 1)
        {
            kb_feature_generation_reset(model_index);
            if (kb_feature_generation(model_index) == 1)
            {
                if (kb_feature_bank_ready(model_index))
                {
                    ret = kb_generate_classification(model_index);
                    kb_generate_children_features(model_index);
                    kb_feature_generation_increment(model_index);
                }
                else
                {
                    ret = -2;
                    kb_generate_children_features(model_index);
                    kb_feature_generation_increment(model_index);
                    kb_reset_model(model_index);
                }
                return ret;
            }
            else
            {

                kb_generate_children_features(model_index);
                kb_feature_generation_increment(model_index);
                kb_reset_model(model_index);
                return -2;
            }
        }
    }
    return -1;
}

int32_t kb_run_model_with_cascade_reset(int16_t *pSample, int32_t nsensors, int32_t model_index)
{
    int32_t ret = -1;

    if (kb_data_streaming(pSample, nsensors, model_index))
    {
        if (kb_segmentation(model_index) == 1)
        {
            kb_feature_generation_reset(model_index);
            if (kb_feature_generation(model_index) == 1)
            {

                if (kb_feature_bank_ready(model_index))
                {
                    ret = kb_generate_classification(model_index);
                    kb_generate_children_features(model_index);
                    kb_reset_feature_banks(model_index);
                    return ret;
                }
                else
                {
                    kb_feature_generation_increment(model_index);
                    kb_generate_children_features(model_index);
                    kb_data_streaming_reset(model_index);
                    return -2;
                }
            }
            else
            {
                kb_generate_children_features(model_index);
                kb_feature_generation_increment(model_index);
                kb_reset_model(model_index);
                return -2;
            }
        }
    }
    return -1;
}

void kb_add_segment(uint16_t *pBuffer, int32_t len, int32_t nbuffs, int32_t model_index)
{
    int32_t ibuf;
    for (ibuf = 0; ibuf < nbuffs; ibuf++)
    {
        setup_rb_with_data(&kb_models[model_index].pdata_buffer->data[ibuf], (rb_data_t *)&pBuffer[ibuf * len], next_pow_2(len), len);
        kb_models[model_index].sg_length = 0;
        kb_models[model_index].sg_index = 0;
        kb_models[model_index].last_read_idx = 0;
    }
}

int32_t kb_run_segment(int32_t model_index)
{
    int32_t ret;
    if (kb_segmentation(model_index) == 1)
    {
        kb_feature_generation_reset(model_index);
        ret = kb_feature_generation(model_index);
        if (ret == 1)
        {
            ret = kb_generate_classification(model_index);
            kb_feature_generation_increment(model_index);
            return ret;
        }
    }
    return -1;
}

int32_t kb_run_segment_with_cascade_features(int32_t model_index)
{
    int32_t ret = -1;
    if (kb_segmentation(model_index) == 1)
    {
        kb_feature_generation_reset(model_index);
        if (kb_feature_generation(model_index) == 1)
        {
            ret = kb_generate_classification(model_index);
            kb_feature_generation_increment(model_index);
            return ret;
        }
        else
        {
            kb_feature_generation_increment(model_index);
            kb_data_streaming_reset(model_index);
            kb_generate_children_features(model_index);
            return -2;
        }
    }

    return -1;
}

int32_t kb_run_segment_with_cascade_reset(int32_t model_index)
{
    int32_t ret = -1;
    if (kb_segmentation(model_index) == 1)
    {
        kb_feature_generation_reset(model_index);
        if (kb_feature_generation(model_index) == 1)
        {
            if (kb_models[model_index].feature_bank_index == kb_models[model_index].pfeature_bank->num_banks - 1)
            {

                ret = kb_generate_classification(model_index);
                kb_generate_children_features(model_index);
                kb_reset_feature_banks(model_index);
                return ret;
            }
            else
            {
                kb_feature_generation_increment(model_index);
                kb_data_streaming_reset(model_index);
                kb_generate_children_features(model_index);
                return -2;
            }
        }
    }
    return -1;
}

int32_t kb_add_last_pattern_to_model(int32_t model_index, uint16_t category, uint16_t influence)
{
    // This needs to be added in a way so that if a classifier doesn't support it, it just returns false. ie no training of random forest on the device
    int32_t ret = 1;

    switch (model_index)
    {
    	case(0):
		ret = 0;
		break;

    default:
        ret = 0;
        break;
    }
    return ret;
}

int32_t kb_add_custom_pattern_to_model(int32_t model_index, uint8_t *feature_vector, uint16_t category, uint16_t influence)
{
    int32_t ret = 1;
    switch (model_index)
    {
    	case(0):
		ret=0;
		break;

    default:
        ret = 0;
        break;
    }

    return ret;
}

int32_t kb_score_model(int32_t model_index, uint16_t category)
{
    int32_t ret = 1;
    switch (model_index)
    {
    	case(0):
		ret = 0;
		break;

    default:
        ret = 0;
        break;
    }
    return ret;
}

int32_t kb_retrain_model(int32_t model_index)
{
    int32_t ret = 1;
    switch (model_index)
    {
    	case(0):
		ret = 0;
		break;

    default:
        ret = 0;
        break;
    }

    return ret;
}

int32_t kb_print_model_score(int32_t model_index)
{
    int32_t ret = 1;
    switch (kb_models[model_index].classifier_type)
    {
    	case(0):
		ret = 0;
		break;

    default:
        ret = 0;
        break;
    }
    return ret;
}

int32_t kb_get_model_header(int32_t model_index, void *model_header)
{
    int32_t ret = 1;
    pme_model_header_t *pme_model_header = (pme_model_header_t *)model_header;
    switch (kb_models[model_index].classifier_type)
    {
    	case(5):
		ret = 0;
		break;

    default:
        ret = 0;
        break;
    }
    return ret;
}

int32_t kb_get_model_pattern(int32_t model_index, int32_t pattern_index, void *pattern)
{
    int32_t ret = 1;
    pme_pattern_t *pme_pattern = (pme_pattern_t *)pattern;
    switch (kb_models[model_index].classifier_type)
    {
    	case(5):
		ret = 0;
		break;

    default:
        ret = 0;
        break;
    }
    return ret;
}

const uint8_t *kb_get_model_uuid_ptr(int32_t model_index)
{
    if (model_index < NUMBER_OF_MODELS)
    {
        return (const uint8_t *)kb_models[model_index].model_uuid;
    }
    else
    {
        return NULL;
    }
}
int32_t kb_get_feature_vector_raw_size(int32_t model_index)
{
    return kb_models[model_index].pfeature_bank->bank_size * kb_models[model_index].pfeature_bank->num_banks;
}

int32_t kb_set_feature_vector_raw(int32_t model_index, float *feature_vector)
{
    int32_t i;

    for (i = 0; i < kb_get_feature_vector_raw_size(model_index); i++)
    {
        kb_models[model_index].pfeature_bank->pFeatures[i] = feature_vector[i];
    };

    return i;
}

float *get_feature_vector_raw_pointer(int32_t model_index)
{
    return kb_models[model_index].pfeature_bank->pFeatures;
}

int32_t sml_get_feature_bank_number(int32_t model_index)
{
    return kb_models[model_index].pfeature_bank->num_banks;
}

int32_t kb_copy_feature_vector_raw(int32_t model_index, float *fv_arr)
{
    int32_t i;

    for (i = 0; i < kb_get_feature_vector_raw_size(model_index); i++)
    {
        fv_arr[i] = kb_models[model_index].pfeature_bank->pFeatures[i];
    };

    return i;
}

int32_t kb_get_segment_length(int32_t model_index)
{
    return (kb_models[model_index].sg_length + ((kb_models[model_index].pfeature_bank->num_banks - 1) * segParams[model_index].delta)) * kb_models[model_index].streaming_filter_length;
}

int32_t kb_get_model_sg_length(int32_t model_index)
{
    return kb_models[model_index].sg_length;
}

int32_t kb_get_num_sensor_buffers(int32_t model_index)
{
    return kb_models[model_index].pdata_buffer->size;
}

void kb_get_segment_data(int32_t model_index, int32_t number_samples, int32_t index, int16_t *p_sample_data)
{
    int32_t t = 0;
    for (int32_t col = 0; col < kb_models[model_index].pdata_buffer->size; col++)
    {
        for (int32_t i = 0; i < number_samples; i++)
        {
            p_sample_data[t] = get_axis_data(kb_models[model_index].pdata_buffer->data + col, index + i);
            t++;
        };
    };
}

int32_t kb_recognize_feature_vector(int32_t model_index)
{
    return (int32_t)kb_recognize(model_index);
}

model_results_t *kb_get_model_result_info(int32_t model_index)
{
    return kb_models[model_index].pmodel_results;
}

// returns the starting index of the very last segment
// before cascading
int32_t kb_get_sg_start_index(int32_t model_index)
{
    return kb_models[model_index].sg_index;
}

// returns the beginning of the last segment used for
// generating inference, after applying cascading
int32_t kb_get_segment_start(int32_t model_index)
{
    return (kb_models[model_index].sg_index - ((kb_models[model_index].pfeature_bank->num_banks - 1) * segParams[model_index].delta)) * kb_models[model_index].streaming_filter_length;
}

bool kb_is_profiling_enabled(int32_t model_index)
{
    return kb_models[model_index].m_profile.enabled;
}

void kb_get_feature_gen_times(int32_t model_index, float *time_arr)
{
    if (!kb_models[model_index].m_profile.enabled)
    {
        return;
    }
    for (int32_t i = 0; i < kb_models[model_index].pfeature_vector->size; i++)
    {
        time_arr[i] = kb_models[model_index].m_profile.feature_gen_times[i];
    }
}

void kb_get_feature_gen_cycles(int32_t model_index, uint32_t *cycle_arr)
{
    if (!kb_models[model_index].m_profile.enabled)
    {
        return;
    }
    for (int32_t i = 0; i < kb_models[model_index].pfeature_vector->size; i++)
    {
        cycle_arr[i] = kb_models[model_index].m_profile.feature_gen_cycles[i];
    }
}

float kb_get_classifier_time(int32_t model_index)
{
    if (!kb_models[model_index].m_profile.enabled)
    {
        return -1.0f;
    }
    return kb_models[model_index].m_profile.classifier_time;
}

uint32_t kb_get_classifier_cycles(int32_t model_index)
{
    if (!kb_models[model_index].m_profile.enabled)
    {
        return 0;
    }
    return kb_models[model_index].m_profile.classifier_cycles;
}
