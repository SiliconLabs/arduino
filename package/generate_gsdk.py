import os
import shutil
import subprocess
import sys

"""
Little helper script to generate and optionally build platform support GSDK files/archives for Arduino
Not pretty, but saves a lot of time :)
"""

# Configurations

all_platform_config = {
    "name": "all"
}

thingplusmatter_ble_platform_config = {
    "name": "thingplusmatter_ble",
    "board_opn": "brd2704a",
    "prebuild": False,
    "matter": False,
    "slcp_file": "slcp/thingplusmatter_ble/thingplusmatter_ble.slcp",
    "additional_files": ["slcp/thingplusmatter_ble/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter_ble/sl_iostream_eusart_thingplus1_config.h"]
}

thingplusmatter_ble_prebuilt_platform_config = {
    "name": "thingplusmatter_ble_precomp",
    "board_opn": "brd2704a",
    "prebuild": True,
    "matter": False,
    "slcp_file": "slcp/thingplusmatter_ble/thingplusmatter_ble.slcp",
    "additional_files": ["slcp/thingplusmatter_ble/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter_ble/sl_iostream_eusart_thingplus1_config.h"]
}

thingplusmatter_matter_platform_config = {
    "name": "thingplusmatter_matter",
    "board_opn": "brd2704a",
    "prebuild": False,
    "matter": True,
    "slcp_file": "slcp/thingplusmatter_matter/thingplusmatter_matter.slcp",
    "additional_files": ["slcp/thingplusmatter_ble/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter_ble/sl_iostream_eusart_thingplus1_config.h"],
    "matter_zap_file": "slcp/common/arduino_matter.zap"
}

thingplusmatter_matter_prebuilt_platform_config = {
    "name": "thingplusmatter_matter_precomp",
    "board_opn": "brd2704a",
    "prebuild": True,
    "matter": True,
    "slcp_file": "slcp/thingplusmatter_matter/thingplusmatter_matter.slcp",
    "additional_files": ["slcp/thingplusmatter_ble/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter_ble/sl_iostream_eusart_thingplus1_config.h"],
    "matter_zap_file": "slcp/common/arduino_matter.zap"
}

xg27devkit_platform_config = {
    "name": "xg27devkit",
    "board_opn": "brd2602a",
    "prebuild": False,
    "matter": False,
    "slcp_file": "slcp/xg27devkit/xg27devkit.slcp",
    "additional_files": ["slcp/xg27devkit/sl_iostream_usart_xg27devkit1_config.h"]
}

xg27devkit_prebuilt_platform_config = {
    "name": "xg27devkit_precomp",
    "board_opn": "brd2602a",
    "prebuild": True,
    "matter": False,
    "slcp_file": "slcp/xg27devkit/xg27devkit.slcp",
    "additional_files": ["slcp/xg27devkit/sl_iostream_usart_xg27devkit1_config.h"]
}

xg24explorerkit_platform_config = {
    "name": "xg24explorerkit",
    "board_opn": "brd2703a",
    "prebuild": False,
    "matter": False,
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h"]
}

xg24explorerkit_prebuilt_platform_config = {
    "name": "xg24explorerkit_precomp",
    "board_opn": "brd2703a",
    "prebuild": True,
    "matter": False,
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h"]
}

xg24explorerkit_matter_platform_config = {
    "name": "xg24explorerkit_matter",
    "board_opn": "brd2703a",
    "prebuild": False,
    "matter": True,
    "slcp_file": "slcp/xg24explorerkit_matter/xg24explorerkit_matter.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h"],
    "matter_zap_file": "slcp/common/arduino_matter.zap"
}

xg24explorerkit_matter_prebuilt_platform_config = {
    "name": "xg24explorerkit_matter_precomp",
    "board_opn": "brd2703a",
    "prebuild": True,
    "matter": True,
    "slcp_file": "slcp/xg24explorerkit_matter/xg24explorerkit_matter.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h"],
    "matter_zap_file": "slcp/common/arduino_matter.zap"
}

wio_mg24_platform_config = {
    "name": "wio_mg24_ble",
    "board_opn": "brd2907a",
    "prebuild": False,
    "matter": False,
    "slcp_file": "slcp/wio_mg24/wio_mg24.slcp",
    "additional_files": ["slcp/wio_mg24/sl_spidrv_eusart_wio_mg24_config.h",
                         "slcp/wio_mg24/sl_iostream_eusart_wio_mg24_config.h",
                         ["slcp/wio_mg24/brd2907a.slcc", "hardware/board/component/"],
                         ["slcp/wio_mg24/brd2907a_config.slcc", "hardware/board/config/component/"]]
}

wio_mg24_prebuilt_platform_config = {
    "name": "wio_mg24_ble_precomp",
    "board_opn": "brd2907a",
    "prebuild": True,
    "matter": False,
    "slcp_file": "slcp/wio_mg24/wio_mg24.slcp",
    "additional_files": ["slcp/wio_mg24/sl_spidrv_eusart_wio_mg24_config.h",
                         "slcp/wio_mg24/sl_iostream_eusart_wio_mg24_config.h",
                         ["slcp/wio_mg24/brd2907a.slcc", "hardware/board/component/"],
                         ["slcp/wio_mg24/brd2907a_config.slcc", "hardware/board/config/component/"]]
}

bgm220explorerkit_platform_config = {
    "name": "bgm220explorerkit",
    "board_opn": "brd4314a",
    "prebuild": False,
    "matter": False,
    "slcp_file": "slcp/bgm220explorerkit/bgm220explorerkit.slcp",
    "additional_files": []
}

bgm220explorerkit_prebuilt_platform_config = {
    "name": "bgm220explorerkit_precomp",
    "board_opn": "brd4314a",
    "prebuild": True,
    "matter": False,
    "slcp_file": "slcp/bgm220explorerkit/bgm220explorerkit.slcp",
    "additional_files": []
}

nanomatter_ble_platform_config = {
    "name": "nanomatter_ble",
    "board_opn": "brd9050a",
    "prebuild": False,
    "matter": False,
    "slcp_file": "slcp/nanomatter_ble/nanomatter_ble.slcp",
    "additional_files": ["slcp/nanomatter_ble/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nanomatter_ble/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nanomatter_ble/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nanomatter_ble/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nanomatter_ble/sl_i2cspm_nanomatter_config.h",
                         ["slcp/nanomatter_ble/brd9050a.slcc", "hardware/board/component/"],
                         ["slcp/nanomatter_ble/brd9050a_config.slcc", "hardware/board/config/component/"]]
}

nanomatter_ble_prebuilt_platform_config = {
    "name": "nanomatter_ble_precomp",
    "board_opn": "brd9050a",
    "prebuild": True,
    "matter": False,
    "slcp_file": "slcp/nanomatter_ble/nanomatter_ble.slcp",
    "additional_files": ["slcp/nanomatter_ble/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nanomatter_ble/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nanomatter_ble/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nanomatter_ble/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nanomatter_ble/sl_i2cspm_nanomatter_config.h",
                         ["slcp/nanomatter_ble/brd9050a.slcc", "hardware/board/component/"],
                         ["slcp/nanomatter_ble/brd9050a_config.slcc", "hardware/board/config/component/"]]
}

nanomatter_matter_platform_config = {
    "name": "nanomatter_matter",
    "board_opn": "brd9050a",
    "prebuild": False,
    "matter": True,
    "slcp_file": "slcp/nanomatter_matter/nanomatter_matter.slcp",
    "additional_files": ["slcp/nanomatter_ble/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nanomatter_ble/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nanomatter_ble/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nanomatter_ble/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nanomatter_ble/sl_i2cspm_nanomatter_config.h",
                         ["slcp/nanomatter_ble/brd9050a.slcc", "hardware/board/component/"],
                         ["slcp/nanomatter_ble/brd9050a_config.slcc", "hardware/board/config/component/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap"
}

nanomatter_matter_prebuilt_platform_config = {
    "name": "nanomatter_matter_precomp",
    "board_opn": "brd9050a",
    "prebuild": True,
    "matter": True,
    "slcp_file": "slcp/nanomatter_matter/nanomatter_matter.slcp",
    "additional_files": ["slcp/nanomatter_ble/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nanomatter_ble/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nanomatter_ble/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nanomatter_ble/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nanomatter_ble/sl_i2cspm_nanomatter_config.h",
                         ["slcp/nanomatter_ble/brd9050a.slcc", "hardware/board/component/"],
                         ["slcp/nanomatter_ble/brd9050a_config.slcc", "hardware/board/config/component/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap"
}

platform_configurations = [all_platform_config,
                           thingplusmatter_ble_platform_config,
                           thingplusmatter_ble_prebuilt_platform_config,
                           thingplusmatter_matter_platform_config,
                           thingplusmatter_matter_prebuilt_platform_config,
                           xg27devkit_platform_config,
                           xg27devkit_prebuilt_platform_config,
                           xg24explorerkit_platform_config,
                           xg24explorerkit_prebuilt_platform_config,
                           xg24explorerkit_matter_platform_config,
                           xg24explorerkit_matter_prebuilt_platform_config,
                           wio_mg24_platform_config,
                           wio_mg24_prebuilt_platform_config,
                           bgm220explorerkit_platform_config,
                           bgm220explorerkit_prebuilt_platform_config,
                           nanomatter_ble_platform_config,
                           nanomatter_ble_prebuilt_platform_config,
                           nanomatter_matter_platform_config,
                           nanomatter_matter_prebuilt_platform_config]

gsdk_dir = "/Users/tajozsi/siliconlabs/gsdk_4.3.2_matter_2.1.1/"
slc_output_dir = "gen/"
gsdk_version="4.3.2"
matter_extension_version = "2.1.1"

gsdk_soc_empty_folder = "app/bluetooth/example/bt_soc_empty/"
matter_lighting_app_folder = "extension/matter_extension/slc/sample-app/lighting-app/efr32/"
matter_zap_folder = "extension/matter_extension/examples/lighting-app/silabs/efr32/data_model/"
gsdk_license_file = "gsdk_license"


def main():
    current_platform_config = get_platform_config_from_arguments()
    if current_platform_config["name"] == "all":
        # Get all the platform configurations and remove the first one (which is the "all")
        all_platform_configs = platform_configurations[1:]
        for config in all_platform_configs:
            generate_gsdk(config)
    else:
        generate_gsdk(current_platform_config)


def generate_gsdk(current_platform_config):
    """
    The main entry point of the GSDK generator
    Generates GSDK with the provided config
    """
    config_name = current_platform_config["name"]
    prebuild = current_platform_config["prebuild"]
    additional_files = current_platform_config["additional_files"]
    is_matter = current_platform_config["matter"]
    project_slcp_file = current_platform_config["slcp_file"]
    board_opn = current_platform_config["board_opn"]
    if is_matter:
        matter_project_zap_file = current_platform_config["matter_zap_file"]

    output_dir = "gen_gsdk_" + config_name + "/"
    slcp_file_name = os.path.basename(project_slcp_file)

    print()
    print("Arduino GSDK generator/prebuilder")
    print("-" * 10)
    print(f"Name: {config_name}")
    print(f"Board: {board_opn}")
    print(f"Project file: {slcp_file_name}")
    print(f"Prebuild: {prebuild}")
    print(f"Matter: {is_matter}")
    if is_matter:
        print(f"Matter ZAP file: {matter_project_zap_file}")
    print(f"GSDK dir: {gsdk_dir}")
    print(f"Output dir: {output_dir}")
    print("-" * 10)

    # If we're generating a GSDK with Matter then the example is at a different path
    if is_matter:
        slcp_folder = matter_lighting_app_folder
        print("Copying ZAP file...")
        shutil.copy(matter_project_zap_file, gsdk_dir + matter_zap_folder + "lighting-app.zap")
    else:
        slcp_folder = gsdk_soc_empty_folder

    copy_slcp_file(slcp_folder, project_slcp_file, additional_files)
    generate_project_with_slc(slcp_folder, slcp_file_name, board_opn)
    if prebuild:
        patch_makefiles()
    if is_matter:
        patch_matter_max_dynamic_endpoint_count()
        patch_matter_color_control_endpoint_count()
        patch_matter_device_descriptors()
    build_project()
    apply_license_to_unlicensed_files(slc_output_dir + "autogen")
    apply_license_to_unlicensed_files(slc_output_dir + "config")
    if prebuild:
        copy_output_files_prebuild(output_dir, is_matter)
    else:
        copy_output_files(output_dir, is_matter)
    cleanup(slcp_folder, slcp_file_name, additional_files)

    print(f"Output location: {output_dir}")
    print("Finished!")


def copy_slcp_file(gsdk_soc_empty_folder, project_slcp_file, additional_files):
    """
    Copy the slcp file to the soc_empty example folder
    """
    print("Copying slcp file...")
    shutil.copy(project_slcp_file, gsdk_dir + gsdk_soc_empty_folder)

    print("Copying additional files...")
    for file in additional_files:
        # If the current element is a list - extract the filename and target directory and copy there
        if type(file) == list:
            shutil.copy(file[0], gsdk_dir + file[1])
        else:
            shutil.copy(file, gsdk_dir + gsdk_soc_empty_folder)


def generate_project_with_slc(gsdk_soc_empty_folder, slcp_file_name, board_opn):
    """
    Generate the project with slc
    """
    print("Starting SLC generation...")

    # Remove the target directory if it exists
    if os.path.exists(slc_output_dir) and os.path.isdir(slc_output_dir):
        shutil.rmtree(slc_output_dir)

    # Call slc to generate the project
    slc_process = subprocess.Popen(
        ["slc", "generate", "-o", "makefile", "-tlcn", "gcc", "-cp",
         "-s", gsdk_dir, "-p", gsdk_dir+gsdk_soc_empty_folder+slcp_file_name, "-d", slc_output_dir, "--with", board_opn]
    )
    slc_process.communicate(timeout=180)
    print("SLC generation finished")


def find_type_of_file_in_dir(file_extension, dir):
    """
    Finds the first file with the given extension in the provided directory
    """
    ret_file = None
    for file_path in os.listdir(dir):
        # check if current file_path is a file
        if os.path.isfile(os.path.join(dir, file_path)):
            if file_path.endswith(file_extension):
                # Return with the first matching file
                ret_file = file_path
                break
    return ret_file


def patch_makefiles():
    """
    Patch the makefiles to output an archive of the GSDK instead of a binary
    """
    # Find the first .Makefile in the directory
    print("Patching the makefiles...")
    makefile = find_type_of_file_in_dir(".Makefile", slc_output_dir)

    # Patch in the archive building
    with open(slc_output_dir + makefile, "r") as in_file:
        buf = in_file.readlines()

    linecnt = 0
    with open(slc_output_dir + makefile, "w") as out_file:
        for line in buf:
            # Add archive file generation to the build process
            if "@echo $(OBJS) > $(OUTPUT_DIR)/linker_objs" in line:
                line = line + "\t$(ECHO)$(AR) rcs gsdk.a $(OBJS)\n"
            # Comment out the linking phase and the binary conversion so that the build won't fail
            # (cosmetic only, we don't need it to link, we have the archive before that)
            if "$(ECHO)$(LD) $(LD_FLAGS) @$(OUTPUT_DIR)/linker_objs $(LIBS) -o $(OUTPUT_DIR)/$(PROJECTNAME).out" in line:
                line = "#" + line
                buf[linecnt + 1] = "#" + buf[linecnt + 1]
                buf[linecnt + 2] = "#" + buf[linecnt + 2]
                buf[linecnt + 3] = "#" + buf[linecnt + 3]
            out_file.write(line)
            linecnt += 1

    # Find the first .mak in the directory
    mak_file = find_type_of_file_in_dir(".mak", slc_output_dir)

    # Patch out the build of 'main.c' and 'app.c' - we don't need their symbols in the archive
    with open(slc_output_dir + mak_file, "r") as in_file:
        buf = in_file.readlines()

    linecnt = 0
    with open(slc_output_dir + mak_file, "w") as out_file:
        for line in buf:
            # If we find the build recipe for these files, comment them out and the next 5 lines as well
            if (("$(OUTPUT_DIR)/project/main.o: main.c" in line) or
                ("$(OUTPUT_DIR)/project/app.o: app.c" in line) or
                ("$(OUTPUT_DIR)/project/src/main.o: src/main.cpp" in line) or
                ("$(OUTPUT_DIR)/project/src/AppTask.o: src/AppTask.cpp" in line) or
                ("$(OUTPUT_DIR)/project/src/LightingManager.o: src/LightingManager.cpp" in line) or
                ("$(OUTPUT_DIR)/project/src/ZclCallbacks.o: src/ZclCallbacks.cpp" in line)
               ):
                line = "#" + line
                buf[linecnt + 1] = "#" + buf[linecnt + 1]
                buf[linecnt + 2] = "#" + buf[linecnt + 2]
                buf[linecnt + 3] = "#" + buf[linecnt + 3]
                buf[linecnt + 4] = "#" + buf[linecnt + 4]
                buf[linecnt + 5] = "#" + buf[linecnt + 5]

            out_file.write(line)
            linecnt += 1

    print("Patching finished")


def patch_matter_max_dynamic_endpoint_count():
    """
    Patches a Matter config file to allow dynamic endpoints to be created
    """
    chip_device_config_file_path = "matter_" + matter_extension_version + "/src/include/platform/CHIPDeviceConfig.h"
    dynamic_endpoints_num = 16

    with open(slc_output_dir + chip_device_config_file_path, "r") as in_file:
        buf = in_file.readlines()

    with open(slc_output_dir + chip_device_config_file_path, "w") as out_file:
        for line in buf:
            if "#define CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT" in line:
                 line = "#define CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT " + str(dynamic_endpoints_num) + "\n"
            out_file.write(line)
    print("Patched max number of dynamic Matter endpoints")


def patch_matter_color_control_endpoint_count():
    """
    Patches Matter to have more light endpoints (same as the max dynamic clusters)
    Needed to workaround an issue in color-control-server where the endpoint count is treated as an index
    """
    zap_gen_config_file_path = "autogen/zap-generated/gen_config.h"
    dynamic_endpoints_num = 16

    with open(slc_output_dir + zap_gen_config_file_path, "r") as in_file:
        buf = in_file.readlines()

    with open(slc_output_dir + zap_gen_config_file_path, "w") as out_file:
        for line in buf:
            if "#define EMBER_AF_COLOR_CONTROL_CLUSTER_SERVER_ENDPOINT_COUNT" in line:
                 line = "#define EMBER_AF_COLOR_CONTROL_CLUSTER_SERVER_ENDPOINT_COUNT (" + str(dynamic_endpoints_num) + ")\n"
            out_file.write(line)
    print("Patched number of Matter Color control endpoints")


def patch_matter_device_descriptors():
    """
    Patches device descriptors (like name and manufacturer) in the Matter config file
    """
    chip_device_config_file_path = "matter_" + matter_extension_version + "/src/include/platform/CHIPDeviceConfig.h"
    dynamic_endpoints_num = 16

    with open(slc_output_dir + chip_device_config_file_path, "r") as in_file:
        buf = in_file.readlines()

    with open(slc_output_dir + chip_device_config_file_path, "w") as out_file:
        for line in buf:
            if "#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME" in line:
                 line = "#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME \"Bozont Labs\"\n"
            if "#define CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME" in line:
                 line = "#define CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME \"Matter device\"\n"
            if "#define CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING" in line:
                 line = "#define CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING \"v1.0\"\n"
            if "#define CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER" in line:
                 line = "#define CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER \"42069\"\n"
            out_file.write(line)
    print("Patched Matter device descriptors")


def build_project():
    """
    Build the generated project
    """
    print("Building the project...")
    makefile = find_type_of_file_in_dir(".Makefile", slc_output_dir)
    build_process = subprocess.Popen(
        ["make", "-f", makefile, "-j", "20"], cwd=slc_output_dir
    )
    build_process.communicate(timeout=180)
    print("Build finished")


def apply_license_to_unlicensed_files(folder):
    """
    Applies the MIT license to any unlicensed source file found in the provided folder
    """
    mit_license = """/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2023 Silicon Laboratories Inc. www.silabs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

"""
    license_start_1 = "/***************************************************************************//**"
    license_start_2 = "/* --------------------------------------------------------------------------"
    license_start_3 = "/*"

    for (root, dirs, file) in os.walk(folder):
        # For each file
        for f in file:
            # Get the full path of the current file
            filepath = os.path.join(root, f)

            # Check if it's a source file
            is_source_file = False
            filename_lower = f.lower()
            for ext in ['.c', '.h', '.cpp', '.hpp']:
                if filename_lower.endswith(ext):
                    is_source_file = True
            if not is_source_file:
                continue

            # Open the file
            with open(filepath, 'r') as fp:
                try:
                    lines = fp.readlines()
                except UnicodeDecodeError:
                    continue
                # See if it has a license
                if (license_start_1 in lines[0]
                   or license_start_2 in lines[0]
                   or license_start_3 in lines[0]):
                    continue

                # Apply the MIT license if no license found
                print(f"Applying license to: {filepath}")
                lines.insert(0, mit_license)

            # Write the file back with the license applied
            with open(filepath, "w") as out_file:
                for line in lines:
                    out_file.write(line)


def copy_output_files(output_dir, is_matter):
    """
    Get the built stuff in one place
    """
    # remove the output dir if it exists
    if os.path.exists(output_dir) and os.path.isdir(output_dir):
        shutil.rmtree(output_dir)
    os.mkdir(output_dir)

    shutil.copytree(slc_output_dir + "autogen", output_dir + "autogen")
    shutil.copytree(slc_output_dir + "config", output_dir + "config")
    shutil.copytree(slc_output_dir + "gecko_sdk_" + gsdk_version, output_dir + "gecko_sdk_" + gsdk_version)
    # Matter has some additional folders
    if is_matter:
        shutil.copytree(slc_output_dir + "matter_" + matter_extension_version, output_dir + "matter_" + matter_extension_version)
        shutil.copytree(slc_output_dir + "include", output_dir + "include")

    print("Applying license to GSDK folder...")
    shutil.copy(gsdk_license_file, output_dir + "gecko_sdk_" + gsdk_version + "/LICENSE")


def copy_output_files_prebuild(output_dir, is_matter):
    """
    Get the built stuff in one place
    """
    print("Copying built files...")

    # Check if the archive has been created successfully
    prebuilt_core_archive_file = slc_output_dir + "gsdk.a"
    if not os.path.exists(prebuilt_core_archive_file):
        print("'gsdk.a' has not been created - build might've failed")
        exit(-1)

    # remove the output dir if it exists
    if os.path.exists(output_dir) and os.path.isdir(output_dir):
        shutil.rmtree(output_dir)
    os.mkdir(output_dir)

    # Get all the archive files from the project
    for (root, dirs, file) in os.walk(slc_output_dir):
        for f in file:
            filepath = os.path.join(root, f)
            if f.endswith(".a"):
                shutil.copy(filepath, output_dir)
                print(f"Copying '{f}'")

    # Get all the header files from the project
    if not is_matter:
        header_output_dir = output_dir + "include/"
        os.mkdir(header_output_dir)
        os.mkdir(header_output_dir + "psa")
        os.mkdir(header_output_dir + "mbedtls")
        count = 0
        for (root, dirs, file) in os.walk(slc_output_dir):
            for f in file:
                filepath = os.path.join(root, f)
                if f.endswith('.h'):
                    # Retain containing directory for 'psa' and 'mbedtls'
                    if "include/psa" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "psa/" + str(f))
                    elif "include/mbedtls" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "mbedtls/" + str(f))
                    else:
                        shutil.copyfile(filepath, header_output_dir + str(f))
                    print(f"Copying '{f}'")
                    count += 1

        print(f"Copied {count} header files")

    # Copy the headers from Matter/src/ with preserving the structure
    if is_matter:
        # Copy the whole output directory
        shutil.copytree(slc_output_dir + "autogen", output_dir + "autogen")
        shutil.copytree(slc_output_dir + "config", output_dir + "config")
        shutil.copytree(slc_output_dir + "gecko_sdk_" + gsdk_version, output_dir + "gecko_sdk_" + gsdk_version)
        shutil.copytree(slc_output_dir + "matter_" + matter_extension_version, output_dir + "matter_" + matter_extension_version)
        shutil.copytree(slc_output_dir + "include", output_dir + "include")
        # Remove 'c' 'cpp' 'a' and 'S' files
        for (root, dirs, file) in os.walk(output_dir):
            for f in file:
                filepath = os.path.join(root, f)
                if f.endswith('.c'):
                    os.remove(filepath)
                if f.endswith('.cpp'):
                    os.remove(filepath)
                #if f.endswith('.a'):
                    #os.remove(filepath)
                if f.endswith('.S'):
                    os.remove(filepath)

    # Get the linkerfile
    shutil.copy(slc_output_dir + "autogen/linkerfile.ld", output_dir)

    print("Applying license to GSDK folder...")
    shutil.copy(gsdk_license_file, output_dir + "/LICENSE")


def cleanup(gsdk_soc_empty_folder, slcp_file_name, additional_files):
    """
    Clean up
    """
    # Remove the slcp file
    print("Cleaning up...")
    os.remove(gsdk_dir + gsdk_soc_empty_folder + slcp_file_name)
    for file in additional_files:
        # If the current element is a list - extract the filename and target directory and remove from there
        if type(file) == list:
            filename = os.path.basename(file[0])
            target_dir = file[1]
            os.remove(gsdk_dir + target_dir + filename)
        else:
            filename = os.path.basename(file)
            os.remove(gsdk_dir + gsdk_soc_empty_folder + filename)
    # Remove the generated project
    shutil.rmtree(slc_output_dir)


def get_platform_config_from_arguments():
    """
    Determines the platform configuration from the command line arguments
    """
    def print_available_configs():
        print("Available platform configurations:")
        for config in platform_configurations:
            print(config["name"])

    try:
        input_config_name = sys.argv[1]
    except Exception:
        print("Please provide a platform configuration name in the first argument!")
        print_available_configs()
        exit(-1)

    found = False
    for config in platform_configurations:
        if config["name"] == input_config_name:
            return config
    if not found:
        print(f"Platform configuration with the name '{input_config_name}' not found!")
        print_available_configs()
        exit(-1)

if __name__ == "__main__":
    main()
