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



#ifndef KB_PIPELINE_H
#define KB_PIPELINE_H

#ifdef WIN32
#include <stdio.h>
#include <math.h>
#include <time.h>
#else
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#include <stdint.h>
#include <stddef.h>

#include "kb_defines.h"
#include "kb_common.h"
#include "kb_typedefs.h"
#include "kbalgorithms.h"
#include "kbutils.h"

#ifdef __cplusplus
extern "C"
{
#endif

    #define ACCELX_S_TF_FOLD_0 0
#define ACCELY_S_TF_FOLD_0 1
#define ACCELZ_S_TF_FOLD_0 2

    #define ACCELX_D_TF_FOLD_0 0
#define ACCELY_D_TF_FOLD_0 1
#define ACCELZ_D_TF_FOLD_0 2

    
int32_t data_streaming_0(void* model, int16_t *pSample);

int32_t data_segmentation_0(void* model, int32_t model_index);

int32_t feature_gen_0(void * model, int32_t *nfeats);

int32_t feature_transform_0(void* model);

int32_t recognize_vector_0(void* model);


#ifdef __cplusplus
}
#endif

#endif // KB_PIPELINE_H
