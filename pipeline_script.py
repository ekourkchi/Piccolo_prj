#!/usr/bin/env python3

import os
import sys
import zipfile
import shutil
import subprocess
import uuid
from pathlib import Path

docker_image = "seggar-with-nrfsdk"
webe_dir_on_host = Path("webe-dir")
assert webe_dir_on_host.exists() and webe_dir_on_host.is_dir(), f"{webe_dir_on_host} doesn't exist"
build_type = "Debug"
webe_dir_basename_in_docker = "webe"
webe_path_in_docker = "/home/nRF5_SDK_17.0.2_d674dde/examples/ble_peripheral"
webe_out_path_in_docker = f"/home/nRF5_SDK_17.0.2_d674dde/examples/ble_peripheral/{webe_dir_basename_in_docker}/pca10056/s140/ses/Output/{build_type}/Exe"
webe_prj_basename = "ble_app_template_pca10056_s140"
webe_seggar_project_path = f"/home/nRF5_SDK_17.0.2_d674dde/examples/ble_peripheral/{webe_dir_basename_in_docker}/pca10056/s140/ses/{webe_prj_basename}.emProject"


def extract_zip(zip_path, extract_to):
    with zipfile.ZipFile(zip_path, 'r') as zip_ref:
        zip_ref.extractall(extract_to)
    print(f"Extracted ZIP to {extract_to}")

# def build_docker_image(tag="builder_image"):
#     subprocess.run(["docker", "build", "-t", tag, "."], check=True)
#     print(f"Docker image '{tag}' built.")

def run_docker_build(extracted_dir, output_dir, docker_tag=docker_image):
    container_name = f"build_container_{uuid.uuid4().hex[:8]}"
    
    subprocess.run([
        "docker", "run", "--name", container_name,
        "-v", f"{webe_dir_on_host.resolve()}:/home/{webe_dir_basename_in_docker}",
        "-v", f"{extracted_dir}:/home/input",
        "-v", f"{output_dir}:/home/output",
        docker_tag,
        "sh", "-c", 
        f"""
         cp -r /home/{webe_dir_basename_in_docker}/ {webe_path_in_docker}/ && \
         cp -r /home/input/knowledgepack {webe_path_in_docker}/{webe_dir_basename_in_docker} && \
         /home/segger/bin/emBuild {webe_seggar_project_path} -config {build_type} && \
         cp -r {webe_out_path_in_docker}/{webe_prj_basename}.hex /home/output/
         """
    ], check=True)

    print(f"Build and copy complete. Container: {container_name}")

    # Clean up container
    subprocess.run(["docker", "rm", container_name], check=True)

def main():
    if len(sys.argv) != 2:
        print("Usage: python run_in_docker.py <your_file.zip>")
        sys.exit(1)

    zip_path = Path(sys.argv[1]).resolve()
    if not zip_path.exists() or not zip_path.suffix == ".zip":
        print("Invalid ZIP file.")
        sys.exit(1)

    work_dir = Path("temp_workspace")
    extracted_dir = work_dir / "extracted"
    output_dir = Path("output")
    
    # Clean up from previous runs
    shutil.rmtree(work_dir, ignore_errors=True)
    shutil.rmtree(output_dir, ignore_errors=True)
    work_dir.mkdir(parents=True)
    output_dir.mkdir()

    extract_zip(zip_path, extracted_dir)
    # build_docker_image()
    run_docker_build(extracted_dir.resolve(), output_dir.resolve())

    print(f"Final output is in: {output_dir.resolve()}")

if __name__ == "__main__":
    main()

