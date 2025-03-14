#!/usr/bin/env python3
import os
import argparse
import pandas as pd

def process_csv_files(directory):
    # List all CSV files in the directory
    csv_files = [f for f in os.listdir(directory) if f.endswith('.csv')]
    
    for file in csv_files:
        file_path = os.path.join(directory, file)
        
        df = pd.read_csv(file_path)
        
        # Modify 'event' column if it exists
        if 'event' in df.columns:
            df['event'] = df['event'].apply(lambda x: 2000 if x != 0 else 0)
        
        # Save back to CSV
        df.to_csv(file_path, index=False)
        print(f"Processed: {file}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process CSV files and modify the 'event' column.")
    parser.add_argument("--in-dir", required=True, help="Path to the input directory containing CSV files")
    args = parser.parse_args()
    
    process_csv_files(args.in_dir)