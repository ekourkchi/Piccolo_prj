#!/usr/bin/env python3

"""
CSV Processing Script with JSON Configuration

This script processes CSV files based on a JSON configuration file.
It keeps only the columns marked as 1 in the config and applies convert_to_raw() (for int16_t conversion) 
function to those columns.

Usage:
    python process_csv_with_config.py <config_file> <input_folder> [output_folder]

Example:
    python process_csv_with_config.py column_config.json Sample_CSVs processed_output
"""

import os
import sys
import json
import argparse
import pandas as pd
from pathlib import Path

def convert_to_int16(value: float, lsb_value: float = 0.001) -> int:
    """Convert acceleration in g to raw int16_t (two's complement 16-bit)."""
    if pd.isna(value) or value == "":
        return 0
    
    try:
        raw_value = int(round(float(value) / lsb_value))  # Scale to integer
        raw_value = max(-32768, min(32767, raw_value))  # Clamp to int16 range
        return raw_value
    except (ValueError, TypeError):
        return 0

def load_config(config_file: str) -> dict:
    """Load and validate JSON configuration file."""
    try:
        with open(config_file, 'r') as f:
            config = json.load(f)
        
        # Validate config structure
        if not isinstance(config, dict):
            raise ValueError("Config must be a JSON object")
        
        # Check if at least one column is selected
        selected_columns = [col for col, value in config.items() if value == 1]
        if not selected_columns:
            print("Warning: No columns are selected (marked as 1) in the config file!")
        
        return config
    except FileNotFoundError:
        print(f"Error: Config file '{config_file}' not found!")
        sys.exit(1)
    except json.JSONDecodeError as e:
        print(f"Error: Invalid JSON in config file: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error loading config file: {e}")
        sys.exit(1)

def validate_paths(config_file: str, input_folder: str) -> bool:
    """Validate that all provided paths exist."""
    errors = []
    
    if not os.path.isfile(config_file):
        errors.append(f"Config file '{config_file}' does not exist")
    
    if not os.path.isdir(input_folder):
        errors.append(f"Input folder '{input_folder}' does not exist")
    
    if errors:
        print("Validation errors:")
        for error in errors:
            print(f"  - {error}")
        return False
    
    return True

def process_csv_file(csv_file: str, config: dict, output_dir: str) -> bool:
    """Process a single CSV file based on the configuration."""
    try:
        print(f"Processing: {os.path.basename(csv_file)}")
        
        # Read CSV file - skip first row (metadata) and use 2nd row as headers
        df = pd.read_csv(csv_file, skiprows=1, header=0) #in files downloaded from healthetile, first row has name and id, 2nd row has actual headers
        # df = pd.read_csv(csv_file)
        # print(f"  After reading CSV (first 5 rows):")
        # print(df.head())
        # print()
        
        
        # Get selected columns (marked as 1 in config)
        selected_columns = [col for col, value in config.items() if value == 1]
        
        if not selected_columns:
            print(f"  Warning: No columns selected for processing")
            return False
        
        # Check which selected columns exist in the CSV
        available_columns = [col for col in selected_columns if col in df.columns]
        missing_columns = [col for col in selected_columns if col not in df.columns]
        
        if missing_columns:
            print(f"  Warning: Missing columns: {missing_columns}")
        
        if not available_columns:
            print(f"  Error: None of the selected columns are available in this CSV")
            return False
        
        # Keep only the selected columns that exist
        df_filtered = df[available_columns].copy()
        
        # Apply convert_to_int16 function to all selected columns
        for column in available_columns:
            df_filtered[column] = df_filtered[column].apply(convert_to_int16)
        
        
        # Save processed file
        output_filename = os.path.basename(csv_file)
        output_path = os.path.join(output_dir, output_filename)
        df_filtered.to_csv(output_path, index=False)

        # df_processed = pd.read_csv(output_path)
        # print(f"  After reading processed CSV (first 5 rows):")
        # print(df_processed.head())
        # print()
        
        print(f"  Saved: {output_filename} ({len(available_columns)} columns processed)")
        return True
        
    except Exception as e:
        print(f"  Error processing {csv_file}: {e}")
        return False

def main():
    parser = argparse.ArgumentParser(
        description="Process CSV files based on JSON configuration",
        epilog="Example:\n"
               "  python process_csv_with_config.py config.json input_folder output_folder\n"
               "  python process_csv_with_config.py config.json input_folder",
        formatter_class=argparse.RawTextHelpFormatter
    )
    
    parser.add_argument("config_file", help="Path to JSON configuration file")
    parser.add_argument("input_folder", help="Folder containing CSV files to process")
    parser.add_argument("output_folder", nargs="?", default="processed_output", 
                       help="Output folder for processed files (default: processed_output)")
    
    args = parser.parse_args()
    
    # Validate paths
    if not validate_paths(args.config_file, args.input_folder):
        sys.exit(1)
    
    # Load configuration
    print(f"Loading configuration from: {args.config_file}")
    config = load_config(args.config_file)
    
    # Create output directory - remove if it exists from before
    try:
        # Check if input and output directories are the same
        if os.path.abspath(args.input_folder) == os.path.abspath(args.output_folder):
            print("Error: Input and output directories cannot be the same!")
            print(f"Input:  {args.input_folder}")
            print(f"Output: {args.output_folder}")
            print("Please specify a different output directory.")
            sys.exit(1)
        
        if os.path.exists(args.output_folder):
            import shutil
            shutil.rmtree(args.output_folder)
            print(f"Removed existing output directory: {args.output_folder}")
        os.makedirs(args.output_folder)
        print(f"Created output directory: {args.output_folder}")
    except Exception as e:
        print(f"Error managing output directory: {e}")
        sys.exit(1)
    
    # Find CSV files
    csv_files = []
    for file in os.listdir(args.input_folder):
        if file.lower().endswith('.csv'):
            csv_files.append(os.path.join(args.input_folder, file))
    
    if not csv_files:
        print(f"No CSV files found in '{args.input_folder}'")
        sys.exit(1)
    
    print(f"Found {len(csv_files)} CSV files to process")
    print(f"Selected columns: {[col for col, val in config.items() if val == 1]}")
    print("-" * 50)
    
    # Process each CSV file
    successful = 0
    failed = 0
    
    for csv_file in csv_files:
        if process_csv_file(csv_file, config, args.output_folder):
            successful += 1
        else:
            failed += 1
    
    print("-" * 50)
    print(f"Processing complete!")
    print(f"Successfully processed: {successful} files")
    print(f"Failed: {failed} files")
    print(f"Output saved to: {args.output_folder}")

if __name__ == "__main__":
    main() 