# The `package` folder and tools

The `package` folder includes scripts tailored for advanced users who require detailed interaction with the Silicon Labs SDK. These scripts enable customization, advanced integration, and management of SDK components, providing tools for building, packaging, and verifying core functionality.

## Folder Overview
- `bootstrap.py` / `bootstrap.sh`: Initializes the development environment.
- `generate_gsdk.py`: Generates the Silicon Labs GSDK / SiSDK files for core integration.
- `get_checksum_filesize.py`: Utility to compute file sizes and SHA-256 checksums for files in a directory, useful for verifying file integrity.
- `gsdk_license`: Licensing file for the Gecko SDK.
- `host_local_install.py`: Hosts a local server to distribute a custom Arduino core for installation in the Arduino IDE.
- `package_arduinosilabs_index.json`: A JSON file defining metadata for Arduino core installation or updates via the Arduino IDE.
- `package_core.sh`: Packs the Arduino core into a ZIP/ZST archive, excluding unnecessary files, and computes its size and checksum.
- `repackage_as_zst.sh`: Converts existing archive files into Zstandard (ZST) format for optimized compression.
- `uncrustify.cfg`: Configuration file for the Uncrustify tool, used to enforce coding style and formatting standards.
- `slcp` folder: Contains project files related to Silicon Labs Configurator Project (SLCP), used for managing and configuring software components of the Silabs board supported by the core.

## The `generate_gsdk.py` script

A script that generates and builds platform support GSDK files/archives for Arduino using `slc`. It relies on the SLCP files contained in the `slcp` folder for managing and configuring the necessary software components.

### Install pre-requirements

- Install `python3`.
- Download and install `Simplicity Studio` from [here](https://www.silabs.com/developer-tools/simplicity-studio).
- Set environment variables for Simplicity Studio. Add the following lines to your shell configuration file, adjusting the path based on your system.

  For macOS, look like this:
  ```bash
  export PATH="/Applications/Simplicity Studio.app/Contents/Eclipse/developer/adapter_packs/zap/:$PATH"
  export ZAP_INSTALL_PATH="/Applications/Simplicity Studio.app/Contents/Eclipse/developer/adapter_packs/zap/"
  export STUDIO_ADAPTER_PACK_PATH=$ZAP_INSTALL_PATH
  source ~/.bashrc  # or ~/.zshrc, ~/.profile
  ```
- Download Silicon Labs Configurator (`slc`) Command-Line Interface (CLI) from [here](https://www.silabs.com/developer-tools/simplicity-studio/configurator-command-line-interface). 
- Extract `slc` ZIP and add its path to your system’s global environment variables.

  For Linux/macOS, look like this:
  ```bash
  export PATH="/path/to/slc_cli:$PATH"
  source ~/.bashrc  # or ~/.zshrc, ~/.profile
  ```
- Install the GCC ARM toolchain (`gcc-arm-none-eabi`) version `12.2.rel1` from [here](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads/12-2-rel1), or install the `Silicon Labs` core via the Arduino IDE's Board Manager, which will install all the required tools, including the GCC ARM toolchain.

- Add `gcc-arm-none-eabi` to your system’s global environment variables.
  
  For Linux/macOS, look like this:
  ```bash
  export PATH="/path/to/gcc-arm-none-eabi/bin:$PATH"
  source ~/.bashrc  # or ~/.zshrc, ~/.profile
  ```

### Configure SDK

1. Download the Silicon Labs Gecko SDK (GSDK) Version 4.4.0 from [here](https://github.com/SiliconLabs/gecko_sdk/releases/download/v4.4.0/gecko-sdk.zip).
2. Extract the content of the SDK ZIP and run the following `slc` commands to configure and trust the SDK:
   ```bash
   slc configuration --sdk "/path/to/gecko-sdk/"
   slc signature trust --sdk "/path/to/gecko-sdk/"
   ```
4. Create the folder `extension/matter_extension` inside the GSDK folder (`/path/to/gecko-sdk/`).
5. Download the Silicon Labs Matter Extension Version 2.2.0 from [here](https://github.com/SiliconLabs/matter_extension/archive/refs/tags/v2.2.0.zip).
6. Extract the content of the Matter Extension ZIP into the path `/path/to/gecko-sdk/extension/matter_extension`.
7. Run the following `slc` command to configure and trust the Matter extension:
   ```bash
   slc signature trust --extension-path="/path/to/gecko-sdk/extension/matter_extension"
   ```

### Configure Script  

Before running the script, make sure to set the correct paths and versions in the `generate_gsdk.cfg` file:  

```python
# Absolute path to your Silicon Labs Gecko SDK (GSDK) directory
gsdk_dir /path/to/gecko-sdk/

# Folder to put the temporary generated files into
slc_output_dir gen/

# Version of the GSDK under 'gsdk_dir'
gsdk_version 4.4.0

# Version of the GSDK Matter Extension under 'gsdk_dir'
matter_extension_version 2.2.0
```

### Run the script
To run the script, use the following command:
```
python generate_gsdk.py <board_variant>
```

Replace `<board_variant>` with the specific board and variant you want to generate, such as `nano_matter_noradio_precomp` to generate the core for the Arduino Nano Matter board with the "None / No radio" protocol stack variant.

Run `python generate_gsdk.py` without arguments to see the full list of board variants:

```
% python3 generate_gsdk.py                                   
Please provide a platform configuration name in the first argument!
Available platform configurations:
all
noradio_precomp_all
ble_arduino_precomp_all
ble_silabs_precomp_all
matter_precomp_all
thingplusmatter_noradio
thingplusmatter_noradio_precomp
thingplusmatter_ble_arduino
thingplusmatter_ble_arduino_precomp
thingplusmatter_ble_silabs
thingplusmatter_ble_silabs_precomp
thingplusmatter_matter
thingplusmatter_matter_precomp
...
```

### Troubleshooting

**Q: I get the error `/bin/sh: /bin/arm-none-eabi-gcc: No such file or directory`** 

**A:** First, check if arm-none-eabi-gcc is in your system PATH. If it’s not found, ensure the toolchain is installed. If it is installed but not recognized, add it to your PATH. If the issue persists, update the build command in the script to explicitly set the correct ARM GCC directory for your operating system.

Open the script and locate the following block of code:
```python
build_process = subprocess.Popen(
["make", "-f", makefile, "-j", "20"], cwd=slc_output_dir
)
```
Set `ARM_GCC_DIR_xxx` to the correct path, for example:
- _Windows_
 ```python
 build_process = subprocess.Popen(
   ["make", "-f", makefile, "-j", "20", "ARM_GCC_DIR_WIN=C:/Program Files/ARM/GCC/"], cwd=slc_output_dir
 )
 ```
- _Linux_
 ```python
 build_process = subprocess.Popen(
   ["make", "-f", makefile, "-j", "20", "ARM_GCC_DIR_LINUX=/usr/local/bin/"], cwd=slc_output_dir
 )
 ```
- _MacOS_
 ```python
 build_process = subprocess.Popen(
   ["make", "-f", makefile, "-j", "20", "ARM_GCC_DIR_OSX=/opt/homebrew/"], cwd=slc_output_dir
 )
 ```

---
