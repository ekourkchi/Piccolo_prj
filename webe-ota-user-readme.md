# nrf52_ble_dfu

A Python utility to perform BLE-based Device Firmware Update (DFU) for nRF52 devices.

---

## 📋 Prerequisites

1. **Install Conda or Miniconda**

   Make sure you have Conda or Miniconda installed on your system.  
   👉 [Installation Guide](https://docs.conda.io/projects/conda/en/latest/user-guide/install/index.html)

2. **Create and Activate a Conda Environment**

   Open a terminal and run the following:

   `conda create -n envName python=3.12`
   `conda activate envName`

## Setup & Usage

1. Navigate into the project directory - `cd nrf52_ble_dfu`
2. Install the package - `pip install -e .`
3. Run the BLE scanner script -  `python ble_scan.py`,  test if your watch is connected to your machine. While scanning, you should see a device named similar to `We-Be xxxx`.
4. Press the required buttons on your watch **to enter DFU mode** : *Power off the device. Then, press the event button firmly, followed by pressing and holding the side button for 1.5 seconds. Nothing should happen on the watch; if the LED blinks, restart the device by powering it off and repeat the process.*
5. Run the BLE scanner script again -  `python ble_scan.py`, test in the scanner output, you should see a device named `DfuTarg` indicating the device is ready for DFU. Do not proceed until you see `DfuTarg`
6. Run the DFU script - `python -m nrf52_ble_dfu path_to_zip_file`
7. When your run fisrt time, if it gives an error, run `python -m nrf52_ble_dfu path_to_zip_file` again. It takes about 4-5 minutes to flash


## 📝 Notes
1. A sample DFU .zip file is provided in the sample_update_zip/ directory for testing purposes.
