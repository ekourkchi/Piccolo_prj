# Compiling the simple test data app

This application requires testdata.h to be populated in order to run. If you haven't specified any, it won't work.

To compile this application with GCC, simply run:

`g++ main.c a.out -L../libsensiml -lsensiml -lm -I../libsensiml` -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu--mcpu=cortex-m4

# Linking to your own application

1. Copy `libsensiml` folder to your desired location
2. Add an include path to your Makefile/IDE project

## Running in your application

1. Add `#include "kb.h"` to the file you wish to use the library in.
2. Before running any other code for the model, call `kb_model_init();`
3. To add sensor data a sample at a time, call `kb_run_model(<samples>, 0, <model_number>);`


