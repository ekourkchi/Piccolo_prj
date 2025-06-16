#!/usr/bin/env python3

import os
import sys
import zipfile
import shutil
import subprocess
import uuid
from pathlib import Path

docker_image_tag = "asmita08/segger-nrfsdk-ota:latest"

#ota
webe_ota_dir_on_host = Path("webe-ota")
assert webe_ota_dir_on_host.exists() and webe_ota_dir_on_host.is_dir(), f"{webe_ota_dir_on_host} doesn't exist"
webe_ota_dir_basename_in_docker = "webe-ota"


def run_docker_build(webe_fw_hex_filename,work_dir, output_dir, docker_tag=docker_image_tag):
    container_name = f"build_container_{uuid.uuid4().hex[:8]}"
    
    subprocess.run([
        "docker", "run", "--name", container_name,
         "-v", f"{webe_ota_dir_on_host.resolve()}:/home/{webe_ota_dir_basename_in_docker}",
        "-v", f"{output_dir.resolve()}:/home/output",
        "-v", f"{work_dir.resolve()}:/home/input",
        docker_tag,
        "sh", "-c", 
        f"""
         cd /home/{webe_ota_dir_basename_in_docker} && \
         make package APP_IMG=/home/input/{webe_fw_hex_filename} && \
         cp -r /home/{webe_ota_dir_basename_in_docker}/hex/we-be-firmware-update-pkg.zip /home/output/
         """
    ], check=True)

    print(f"Build and copy complete. Container: {container_name}")

    # Clean up container
    subprocess.run(["docker", "rm", container_name], check=True)

def main():
    if len(sys.argv) != 2:
        print("Usage: python run_in_docker.py <webe_firmware.hex>")
        sys.exit(1)

    webe_fw_hex_file = Path(sys.argv[1]).resolve()
    if not webe_fw_hex_file.exists() or not webe_fw_hex_file.is_file() or webe_fw_hex_file.suffix.lower() != '.hex':
        print("Invalid hex file.")
        sys.exit(1)

    webe_fw_hex_file_name = webe_fw_hex_file.name

    output_dir = Path("output")
    work_dir = Path("temp-workspace")
    
    # Clean up from previous runs
    shutil.rmtree(work_dir, ignore_errors=True)
    shutil.rmtree(output_dir, ignore_errors=True)
    work_dir.mkdir(parents=True)
    output_dir.mkdir()

    shutil.copy(webe_fw_hex_file, work_dir)

    # build_docker_image()
    run_docker_build(webe_fw_hex_file_name, work_dir.resolve(), output_dir.resolve())

    print(f"Final output is in: {output_dir.resolve()}")

if __name__ == "__main__":
    main()

