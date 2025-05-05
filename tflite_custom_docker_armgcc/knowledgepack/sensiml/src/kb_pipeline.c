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




#include "kb_pipeline.h"

#include "tf_micro.h"

#include "sml_profile_utils.h"
static uint32_t profile_cycle_count = 0;
static float profile_total_time = 0.0f;
static float profile_avg_iter_time = 0.0f;

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"

// Operations
#define FLT_MAX (FLOAT)0x7f7fffff
#define FLT_MIN -(FLT_MAX)
// FILL_MAX_NUMBER_COLUMNS
// FILL_MAX_NUMBER_PARAMS
#ifndef NUMBER_OF_COLUMNS
#define NUMBER_OF_COLUMNS 10
#endif

#ifndef MAX_NUMBER_PARAMS
#define MAX_NUMBER_PARAMS 10
#endif

int16_t columns[NUMBER_OF_COLUMNS];
static float params[MAX_NUMBER_PARAMS];

static float_data_t input_params = {
    .data = params,
    .size = MAX_NUMBER_PARAMS};

static int16_data_t input_columns = {
    .data = columns,
    .size = NUMBER_OF_COLUMNS};


int32_t data_streaming_0(void* model, int16_t *pSample)
{
	int32_t FrameIdx = 0;
	kb_model_t * kb_model = (kb_model_t*)model;

	input_columns.data[0] = ACCELX_S_TF_FOLD_0;
	input_columns.data[1] = ACCELY_S_TF_FOLD_0;
	input_columns.data[2] = ACCELZ_S_TF_FOLD_0;
	input_columns.size = 3;
	FrameIdx += tr_sensor_sensors(pSample, &input_columns, &kb_model->input_data[FrameIdx]);	
saveSensorData(kb_model->pdata_buffer->data, kb_model->input_data, kb_model->pdata_buffer->size);
	return 1;
}



int32_t data_segmentation_0(void* model, int32_t model_index)
{
	int32_t FrameIdx = 0;
	kb_model_t * kb_model = (kb_model_t*)model+model_index;
	int32_t new_samples = rb_num_items(kb_model->pdata_buffer->data, kb_model->last_read_idx);
	int32_t i;
	for (i=0; i<new_samples; i++){
			kb_model->last_read_idx += 1;
		input_columns.size = 0;

	if(sg_windowing(kb_model, &input_columns, kb_model->psegParams)){
			return 1;
		}
	}
	// Colect more samples
	return -1;
}



int32_t feature_gen_0(void * model, int32_t *nfeats)
{
	int32_t column = 0;
	int32_t num_params = 1;
	kb_model_t * kb_model = (kb_model_t*)model;
	int32_t nframes = kb_model->sg_length;
	int32_t CompIdx = (kb_model->feature_bank_index) * (kb_model->pfeature_bank->bank_size);
	
input_params.size = 0;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_pct025(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_pct025(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_pct025(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_pct075(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_pct075(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_pct075(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_abs_mean(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_abs_mean(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_abs_sum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_iqr(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_iqr(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_iqr(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_kurtosis(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_kurtosis(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_kurtosis(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_linear_regression(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_linear_regression(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_linear_regression(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_maximum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_maximum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_maximum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_mean(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_median(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_median(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_minimum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_minimum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_minimum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 100;	
input_params.size = 1;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_negative_zero_crossings(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 100;	
input_params.size = 1;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_negative_zero_crossings(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 100;	
input_params.size = 1;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_negative_zero_crossings(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 100;	
input_params.size = 1;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_positive_zero_crossings(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 100;	
input_params.size = 1;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_positive_zero_crossings(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELX_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_skewness(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_skewness(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_skewness(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_sum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELY_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_variance(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = ACCELZ_D_TF_FOLD_0;	
input_columns.size=1;	
*nfeats = fg_stats_variance(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
return 1;
}



int32_t feature_transform_0(void * model)
{
	kb_model_t * kb_model = (kb_model_t*)model;
	
	static struct minmax aminmax[43] = {
	
		{0, -1115.0f, 940.0f},	
		{1, -822.5f, 182.5f},	
		{2, -1095.0f, 980.0f},	
		{3, -390.0f, 1277.5f},	
		{4, -75.0f, 1037.5f},	
		{5, 190.0f, 1577.5f},	
		{6, 62.0f, 792.2857055664062f},	
		{7, 526.7142944335938f, 1137.5714111328125f},	
		{8, 8190.0f, 68330.0f},	
		{9, 30.0f, 1132.5f},	
		{10, 30.0f, 1165.0f},	
		{11, 35.0f, 2040.0f},	
		{12, -1.3650370836257935f, 10.408971786499023f},	
		{13, -1.4735939502716064f, 4.074319362640381f},	
		{14, -1.5534054040908813f, 2.8475732803344727f},	
		{15, -15.003408432006836f, 19.707111358642578f},	
		{16, -529.2998046875f, 1092.28564453125f},	
		{18, 0.6311800479888916f, 4.097434997558594f},	
		{19, -12.294286727905273f, 25.516048431396484f},	
		{20, -1140.732177734375f, 813.15087890625f},	
		{22, 0.5432425737380981f, 5.796986103057861f},	
		{23, -21.940860748291016f, 16.67290687561035f},	
		{24, -591.7464599609375f, 1186.257568359375f},	
		{27, 170.0f, 2510.0f},	
		{28, 40.0f, 2100.0f},	
		{29, 730.0f, 3480.0f},	
		{30, -255.57142639160156f, 997.0f},	
		{31, -395.0f, 255.0f},	
		{32, -285.0f, 995.0f},	
		{33, -1560.0f, 610.0f},	
		{34, -2200.0f, -10.0f},	
		{35, -2050.0f, 520.0f},	
		{36, 2.0f, 19.0f},	
		{37, 1.0f, 13.0f},	
		{38, 2.0f, 10.0f},	
		{39, 2.0f, 14.0f},	
		{40, 2.0f, 11.0f},	
		{41, -2.64194917678833f, 1.5928038358688354f},	
		{42, -1.4954160451889038f, 1.834968090057373f},	
		{43, -1.446130633354187f, 1.2024331092834473f},	
		{44, -19140.0f, 30070.0f},	
		{45, 9222.85546875f, 950479.75f},	
		{46, 34842.296875f, 1545875.125f},	
	};
	
	int32_t start = (kb_model->feature_bank_index+1) * (kb_model->pfeature_bank->bank_size);
	
	int32_t total_features = (kb_model->pfeature_bank->bank_size) * (kb_model->pfeature_bank->num_banks);
	
	min_max_scale(kb_model->pfeature_bank->pFeatures, kb_model->pfeature_vector, kb_model->pfeature_vector->size, start, total_features, 0, 255, aminmax);
	return 1;

}



int32_t recognize_vector_0(void * model)
{
	int32_t ret;
	kb_model_t * kb_model = (kb_model_t*)model;
	ret = tf_micro_simple_submit(kb_model->classifier_id, kb_model->pfeature_vector, kb_model->pmodel_results);
	return ret;

}

