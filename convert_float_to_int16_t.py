#!/usr/bin/env python3

# This script reads all the csv files in Gesture/WB_gesture_data 
# (those were collected from WB watch and Mobile app with its WebUI)
# Because on WB firmware, we need raw value, 
# we will convert these float to uint16_t or maybe int16_t (as kb_run_model needs int16_t)
# And save these new CSVs with raw values in different dir


import os
import argparse
import pandas as pd

def convert_to_raw(value: float, lsb_value: float = 0.001) -> int: #0.001 as given in max32664 sensor datasheet which is used in WeBe
    """Convert acceleration in g to raw uint16_t (two's complement 16-bit)."""
    raw_value = int(round(value / lsb_value))  # Scale to integer
    raw_value = max(-32768, min(32767, raw_value))  # Clamp to int16 range

    # if raw_value < 0: # use if converting to uint16_t
    #     raw_value += (1 << 16)  

    return raw_value  # return int16_t

def process_csv(input_file: str, output_dir: str):
    """Process a single CSV file: Read, convert, and save the transformed file."""
    try:
        df = pd.read_csv(input_file)

        # Print column headers for confirmation
        print(f"Processing: {input_file}")
        print(f"Column Headers: {list(df.columns)}")

        # Validate required columns
        required_columns = {'accelx', 'accely', 'accelz', 'event'}
        if not required_columns.issubset(df.columns):
            print(f"Skipping {input_file}: Required columns missing!")
            return
        
        # Convert ax, ay, az to int16_t
        df['accelx'] = df['accelx'].apply(convert_to_raw)
        df['accely'] = df['accely'].apply(convert_to_raw)
        df['accelz'] = df['accelz'].apply(convert_to_raw)

        
        df['event'] = df['event'].apply(lambda x: 2000 if x != 0 else 0) #scaleup event to be clearly visible on the graph 

        # Save processed file
        output_file = os.path.join(output_dir, os.path.basename(input_file))
        df.to_csv(output_file, index=False)

        print(f"Saved processed file to: {output_file}")

    except Exception as e:
        print(f"Error processing {input_file}: {e}")

def main():
    """Main function to handle command-line arguments and process files."""
    parser = argparse.ArgumentParser(
        description="Convert CSV acceleration values (ax, ay, az) to uint16_t two's complement format.",
        epilog="Example Usage:\n"
               "  python script.py --single file.csv --out-dir output_dir\n"
               "  python script.py --multiple input_dir --out-dir output_dir",
        formatter_class=argparse.RawTextHelpFormatter
    )
    
    parser.add_argument("--single", type=str, help="Path to a single CSV file to process")
    parser.add_argument("--multiple", type=str, help="Directory containing multiple CSV files")
    parser.add_argument("--out-dir", type=str, required=True, help="Output directory for processed CSVs")

    args = parser.parse_args()

    # Ensure output directory exists
    os.makedirs(args.out_dir, exist_ok=True)

    if args.single:
        if not args.single.endswith(".csv") or not os.path.isfile(args.single):
            print("Invalid file path or not a CSV file!")
            return
        process_csv(args.single, args.out_dir)

    elif args.multiple:
        if not os.path.isdir(args.multiple):
            print("Invalid input directory for --multiple!")
            return
        
        print(f"Searching for CSV files in: {args.multiple}")
        csv_files = [f for f in os.listdir(args.multiple) if f.endswith(".csv")]
        
        if not csv_files:
            print("No CSV files found in the input directory!")
            return

        for filename in csv_files:
            process_csv(os.path.join(args.multiple, filename), args.out_dir)
        
        print("Batch processing completed.")

    else:
        parser.print_help()

if __name__ == "__main__":
    main()
