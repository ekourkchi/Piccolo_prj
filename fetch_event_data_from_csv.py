#!/usr/bin/env python3
import pandas as pd
import argparse
import os

# Read CSV file
def process_csv_to_header(csv_filename, header_filename, start, end):
    df = pd.read_csv(csv_filename)
    
    # Validate required columns
    required_columns = {'accelx', 'accely', 'accelz', 'event', 'sequence'}
    if not required_columns.issubset(df.columns):
        print("Required columns missing! : 'accelx', 'accely', 'accelz', 'event', 'sequence'")
        return
    # # Filter rows where event is non-zero
    # filtered_df = df[df['event'] != 0][['accelx', 'accely', 'accelz']]
    # Filter rows based on the 'sequence' column 
    filtered_df = df[(df['sequence'] >= start) & (df['sequence'] <= end) & (df['event'] != 0)]
    # Convert to list of tuples
    # accel_data = filtered_df.values.tolist()
    accel_data = filtered_df[['accelx', 'accely', 'accelz']].values.tolist()
    
    # Write to header file
    with open(header_filename, 'w') as f:
        f.write('#ifndef TESTDATA_H\n')
        f.write('#define TESTDATA_H\n\n')
        f.write(f'#define TD_NUMROWS {len(accel_data)}\n\n')
        f.write(f'#define TD_NUMCOLS {3}\n\n')
        f.write('const short testdata[TD_NUMROWS][TD_NUMCOLS] = {\n')
        
        for i, (x, y, z) in enumerate(accel_data):
            f.write(f'    {{{x}, {y}, {z}}}{"," if i < len(accel_data) - 1 else ""}\n')
        
        f.write('};\n\n')
        f.write('#endif // TESTDATA_H\n')

# Command line argument parsing
parser = argparse.ArgumentParser(description='Process a CSV file and generate a C header file with acceleration data.')
parser.add_argument('--csv-file', required=True, help='Path to the input CSV file.')
parser.add_argument('--out-file', required=True, help='Path to the output header file.')
parser.add_argument('--start', required=True, type=int,help='start sequence index number from csv')
parser.add_argument('--end', required=True, type=int,help='end sequence index number from csv')
args = parser.parse_args()

# Validate file extensions
if not args.csv_file.lower().endswith('.csv'):
    raise ValueError("Input file must have a .csv extension")
if not args.out_file.lower().endswith('.h'):
    raise ValueError("Output file must have a .h extension")

# Process the CSV file
process_csv_to_header(args.csv_file, args.out_file, args.start, args.end)
print(f"Header file '{args.out_file}' generated successfully.")
