import os
import shutil
import subprocess
import sys

"""
Little helper script to generate and optionally build platform support GSDK files/archives for Arduino with SLC
"""

# To customize/configure generation, please edit the 'generate_gsdk.cfg' file to your preferences
config_file_path = "generate_gsdk.cfg"
gsdk_dir = None
slc_output_dir = None
gsdk_version = None
matter_extension_version = None
aiml_extension_version = None

gsdk_cpp_empty_folder = "app/common/example/empty/"
gsdk_bt_soc_empty_folder = "app/bluetooth/example/bt_soc_empty/"
matter_lighting_app_folder = "extension/matter_extension/slc/sample-app/lighting-app/efr32/"
matter_zap_folder = "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/data_model/"
gsdk_license_file = "gsdk_license"
variants_folder = "../variants/"


def main():
    print()
    print("Arduino GSDK generator/prebuilder")
    print("-" * 33)

    read_config_file(config_file_path)

    current_platform_config = get_platform_config_from_arguments()
    if current_platform_config["name"] == "all":
        # Get all the platform configurations and remove the first three (which are the "all" configs)
        all_platform_configs = platform_configurations[5:]
        for config in all_platform_configs:
            generate_gsdk(config)
    elif current_platform_config["name"] == "noradio_precomp_all":
        # Generate all the noradio platform configurations
        noradio_all_platform_configs = platform_configurations[5:]
        for config in noradio_all_platform_configs:
            if config["protocol_stack"] == 'noradio' and config["prebuild"]:
                generate_gsdk(config)
    elif current_platform_config["name"] == "ble_arduino_precomp_all":
        # Generate all the ble (controller) platform configurations
        ble_all_platform_configs = platform_configurations[5:]
        for config in ble_all_platform_configs:
            if config["protocol_stack"] == 'ble_arduino' and config["prebuild"]:
                generate_gsdk(config)
    elif current_platform_config["name"] == "ble_silabs_precomp_all":
        # Generate all the ble (silabs host) platform configurations
        ble_silabs_all_platform_configs = platform_configurations[5:]
        for config in ble_silabs_all_platform_configs:
            if config["protocol_stack"] == 'ble_silabs' and config["prebuild"]:
                generate_gsdk(config)
    elif current_platform_config["name"] == "matter_precomp_all":
        # Generate all the matter platform configurations
        matter_all_platform_configs = platform_configurations[5:]
        for config in matter_all_platform_configs:
            if config["protocol_stack"] == 'matter' and config["prebuild"]:
                generate_gsdk(config)
    else:
        generate_gsdk(current_platform_config)


def generate_gsdk(current_platform_config):
    """
    The main entry point of the GSDK generator
    Generates GSDK with the provided config
    """
    config_name = current_platform_config["name"]
    arduino_variant_name = current_platform_config["arduino_variant_name"]
    prebuild = current_platform_config["prebuild"]
    additional_files = current_platform_config["additional_files"]
    protocol_stack = current_platform_config["protocol_stack"]
    project_slcp_file = current_platform_config["slcp_file"]
    board_opn = current_platform_config["board_opn"]
    ai_capable = current_platform_config["ai_capable"]
    if protocol_stack == 'matter':
        matter_project_zap_file = current_platform_config["matter_zap_file"]

    output_dir = "gen_gsdk_" + config_name + "/"
    slcp_file_name = os.path.basename(project_slcp_file)
    variant_target_folder = variants_folder + arduino_variant_name + "/" + protocol_stack + "/"

    print()
    print("-" * 50)
    print(f"Name: {config_name}")
    print(f"Arduino variant: {arduino_variant_name}")
    print(f"Board: {board_opn}")
    print(f"AI capable: {ai_capable}") # Indicates if the board supports hardware acceleration for AI/ML tasks
    if ai_capable:
        print(f"AI/ML extension version: {aiml_extension_version}")
    print(f"Project file: {slcp_file_name}")
    print(f"Prebuild: {prebuild}")
    print(f"Protocol stack: {protocol_stack}")
    if protocol_stack == 'matter':
        print(f"Matter ZAP file: {matter_project_zap_file}")
    print(f"GSDK dir: {gsdk_dir}")
    print(f"GSDK version: {gsdk_version}")
    if protocol_stack == 'matter':
        print(f"Matter extension version: {matter_extension_version}")
    print(f"Output dir: {output_dir}")
    if prebuild:
        print(f"Variant target folder: {variant_target_folder}")
    print("-" * 50)

    is_noradio = (protocol_stack == 'noradio')
    is_ble_arduino = (protocol_stack == 'ble_arduino')
    is_ble_silabs = (protocol_stack == 'ble_silabs')
    is_matter = (protocol_stack == 'matter')

    # If we're generating a GSDK with Matter then the example is at a different path
    if is_matter:
        slcp_folder = matter_lighting_app_folder
        print("Copying ZAP file...")
        shutil.copy(matter_project_zap_file, gsdk_dir + matter_zap_folder + "lighting-app.zap")
    elif is_ble_arduino:
        slcp_folder = gsdk_cpp_empty_folder
    elif is_ble_silabs:
        slcp_folder = gsdk_bt_soc_empty_folder
    elif is_noradio:
        slcp_folder = gsdk_cpp_empty_folder

    copy_slcp_file(slcp_folder, project_slcp_file, additional_files)
    generate_project_with_slc(slcp_folder, slcp_file_name, board_opn)
    patch_peripheral_inits()
    if prebuild:
        patch_makefiles()
    if is_matter:
        patch_matter_max_dynamic_endpoint_count()
        patch_matter_device_descriptors(current_platform_config["matter_vendor_name"], current_platform_config["matter_vendor_id"])
        patch_matter_device_name()
    build_project()
    apply_license_to_unlicensed_files(slc_output_dir + "autogen")
    apply_license_to_unlicensed_files(slc_output_dir + "config")
    if prebuild:
        copy_output_files_prebuild(output_dir, is_matter, is_ble_arduino, is_noradio, ai_capable)
        copy_generated_sdk_to_variants(output_dir, variant_target_folder)
    else:
        copy_output_files(output_dir, is_matter)

    keep_gen_folder = get_keep_gen_folder_parameter_from_arguments()
    cleanup(slcp_folder, slcp_file_name, additional_files, keep_gen_folder)

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
        ["slc", "generate", "-o", "makefile", "-tlcn", "gcc", "-cp", "--generator-timeout", "600",
         "-s", gsdk_dir, "-p", gsdk_dir+gsdk_soc_empty_folder+slcp_file_name, "-d", slc_output_dir, "--with", board_opn]
    )
    slc_process.communicate(timeout=600)
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


def patch_peripheral_inits():
    """
    Removes the peripheral initialization calls from the generated project.
    The GSDK automatically initializes all included peripherals - but Arduino has a different philosophy.
    By patching these out the peripherals won't be initialized after boot - only when the user requests them.
    """
    event_handler_file_path = "/autogen/sl_event_handler.c"

    with open(slc_output_dir + event_handler_file_path, "r") as in_file:
        buf = in_file.readlines()

    with open(slc_output_dir + event_handler_file_path, "w") as out_file:
        for line in buf:
            if "sl_debug_swo_init();" in line:
                 line = "  //" + line
            if "sl_cos_send_config();" in line:
                line = "  //" + line
            if "sl_i2cspm_init_instances();" in line:
                 line = "  //" + line
            if "sl_spidrv_init_instances();" in line:
                 line = "  //" + line
            if "sl_iostream_init_instances();" in line:
                line = "  //" + line
            if "sl_iostream_uart_init_instances();" in line:
                 line = "  //" + line
            if "sl_iostream_usart_init_instances();" in line:
                 line = "  //" + line
            if "sl_iostream_eusart_init_instances();" in line:
                 line = "  //" + line
            if "sl_rail_util_pti_init();" in line:
                 line = "  //" + line
            if "sl_pwm_init_instances();" in line:
                line = "  //" + line
            if "sl_simple_led_init_instances();" in line:
                line = "  //" + line
            if "sl_tflite_micro_init();" in line:
                line = "  //" + line
            out_file.write(line)
    print("Patched peripheral initialization calls")


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
                line = line + "else ifeq ($(UNAME_S),Linux)\n"
                line = line + "\t@echo $(OBJS) > $(OUTPUT_DIR)/linker_objs\n"
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
                ("$(OUTPUT_DIR)/project/src/ZclCallbacks.o: src/ZclCallbacks.cpp" in line) or
                ("$(OUTPUT_DIR)/project/src/ZclCallbacks.o: src/ZclCallbacks.cpp" in line) or
                ("$(OUTPUT_DIR)/project/aiml_2.0.0/src/sl_tflite_micro_init.o: aiml_2.0.0/src/sl_tflite_micro_init.cc" in line)
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
    chip_device_config_file_path = "matter_" + matter_extension_version + "/third_party/matter_sdk/src/include/platform/CHIPDeviceConfig.h"
    dynamic_endpoints_num = 16

    with open(slc_output_dir + chip_device_config_file_path, "r") as in_file:
        buf = in_file.readlines()

    with open(slc_output_dir + chip_device_config_file_path, "w") as out_file:
        for line in buf:
            if "#define CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT" in line:
                 line = "#define CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT " + str(dynamic_endpoints_num) + "\n"
            out_file.write(line)
    print("Patched max number of dynamic Matter endpoints")


def patch_matter_device_descriptors(vendor_name, vendor_id):
    """
    Patches device descriptors (like name and manufacturer) in the Matter config file
    """
    chip_device_config_file_path = "matter_" + matter_extension_version + "/third_party/matter_sdk/src/include/platform/CHIPDeviceConfig.h"

    with open(slc_output_dir + chip_device_config_file_path, "r") as in_file:
        buf = in_file.readlines()

    with open(slc_output_dir + chip_device_config_file_path, "w") as out_file:
        for line in buf:
            #if "#define CHIP_DEVICE_CONFIG_ENABLE_TEST_SETUP_PARAMS 1" in line:
            #    line = "#define CHIP_DEVICE_CONFIG_ENABLE_TEST_SETUP_PARAMS 0\n"
            if "#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME" in line:
                 line = "#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME \"" + vendor_name + "\"\n"
            if "#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID" in line:
                line = "#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID " + vendor_id + "\n"
            if "#define CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME" in line:
                 line = "#define CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME \"Matter device\"\n"
            if "#define CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING" in line:
                 line = "#define CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING \"v1.0\"\n"
            if "#define CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER" in line:
                 line = "#define CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER \"42069\"\n"
            out_file.write(line)

    platform_config_header_path = "matter_" + matter_extension_version + "/third_party/matter_sdk/src/platform/silabs/CHIPDevicePlatformConfig.h"
    with open(slc_output_dir + platform_config_header_path, "r") as in_file:
        buf = in_file.readlines()

    with open(slc_output_dir + platform_config_header_path, "w") as out_file:
        for line in buf:
            #if "#define CHIP_DEVICE_CONFIG_ENABLE_TEST_SETUP_PARAMS 1" in line:
            #    line = "#define CHIP_DEVICE_CONFIG_ENABLE_TEST_SETUP_PARAMS 0\n"
            if "#define CHIP_DEVICE_CONFIG_TEST_PRODUCT_NAME" in line:
                 line = "#define CHIP_DEVICE_CONFIG_TEST_PRODUCT_NAME \"Matter device\"\n"
            if "#define CHIP_DEVICE_CONFIG_TEST_VENDOR_NAME" in line:
                line = "#define CHIP_DEVICE_CONFIG_TEST_VENDOR_NAME \"" + vendor_name + "\"\n"
            out_file.write(line)

    print("Patched Matter device descriptors")


def patch_matter_device_name():
    """
    Patches the Matter device name to "Arduino Matter Device"
    """
    app_config_file_path = "include/AppConfig.h"

    with open(slc_output_dir + app_config_file_path, "r") as in_file:
        buf = in_file.readlines()

    with open(slc_output_dir + app_config_file_path, "w") as out_file:
        for line in buf:
            if "#define BLE_DEV_NAME" in line:
                 line = "#define BLE_DEV_NAME \"Arduino Matter Device\"\n"
            out_file.write(line)

    print("Patched Matter device name")


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
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
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
    shutil.copytree(slc_output_dir + "simplicity_sdk_" + gsdk_version, output_dir + "simplicity_sdk_" + gsdk_version)
    # Matter has some additional folders
    if is_matter:
        shutil.copytree(slc_output_dir + "matter_" + matter_extension_version, output_dir + "matter_" + matter_extension_version)
        shutil.copytree(slc_output_dir + "include", output_dir + "include")

    print("Applying license to GSDK folder...")
    shutil.copy(gsdk_license_file, output_dir + "simplicity_sdk_" + gsdk_version + "/LICENSE")


def copy_output_files_prebuild(output_dir, is_matter, is_ble_arduino, is_noradio, ai_capable):
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
        os.mkdir(header_output_dir + "flatbuffers")
        os.mkdir(header_output_dir + "fixedpoint")
        os.mkdir(header_output_dir + "internal")
        count = 0
        for (root, dirs, file) in os.walk(slc_output_dir):
            for f in file:
                filepath = os.path.join(root, f)
                if f.endswith('.h'):
                    # Skip the 'tflite-micro' headers
                    if "tflite-micro/tensorflow" in filepath:
                        print(f"Skipping '{f}' as it is in 'tflite-micro/tensorflow'")
                        continue
                    # Retain containing directory for 'psa' and 'mbedtls'
                    if "include/psa" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "psa/" + str(f))
                    elif "include/mbedtls" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "mbedtls/" + str(f))
                    elif "include/flatbuffers" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "flatbuffers/" + str(f))
                    elif "gemmlowp/fixedpoint" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "fixedpoint/" + str(f))
                    elif "gemmlowp/internal" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "internal/" + str(f))
                    else:
                        shutil.copyfile(filepath, header_output_dir + str(f))
                    print(f"Copying '{filepath}'")
                    count += 1

        if ai_capable:
            # Copy the headers from tflite-micro with preserving the structure
            shutil.copytree(slc_output_dir + "aiml_" + aiml_extension_version + "/third_party/tflite-micro/tensorflow", header_output_dir + "tensorflow")

        # Remove 'cc' files
        for (root, dirs, file) in os.walk(header_output_dir):
            for f in file:
                filepath = os.path.join(root, f)
                if f.endswith('.cc'):
                    os.remove(filepath)

        print(f"Copied {count} header files")

    # Copy the headers from a generated Matter variant while preserving the structure of the Matter SDK
    if is_matter:
        # Copy the whole output directory except for the SDK
        shutil.copytree(slc_output_dir + "autogen", output_dir + "autogen")
        shutil.copytree(slc_output_dir + "config", output_dir + "config")
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

        # Copy the headers from the Simplicity SDK folder
        header_output_dir = output_dir + "include/"
        # Create the folders for the structures we have to keep
        os.mkdir(header_output_dir + "openthread")
        os.mkdir(header_output_dir + "openthread/platform")
        os.mkdir(header_output_dir + "psa")
        os.mkdir(header_output_dir + "mbedtls")
        sisdk_folder_path = slc_output_dir + "simplicity_sdk_" + gsdk_version
        for (root, dirs, file) in os.walk(sisdk_folder_path):
            for f in file:
                filepath = os.path.join(root, f)
                if f.endswith('.h'):
                    # Retain containing directory structure for the following
                    if "include/openthread/platform" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "openthread/platform/" + str(f))
                    elif "include/openthread" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "openthread/" + str(f))
                    elif "include/psa" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "psa/" + str(f))
                    elif "include/mbedtls" in filepath:
                        shutil.copyfile(filepath, header_output_dir + "mbedtls/" + str(f))
                    else:
                        shutil.copyfile(filepath, header_output_dir + str(f))
                    print(f"Copying '{filepath}'")

        if ai_capable:
            # Create AI/ML specific include directories
            os.mkdir(header_output_dir + "flatbuffers")
            os.mkdir(header_output_dir + "fixedpoint")
            os.mkdir(header_output_dir + "internal")
            # Copy headers from the AI/ML SDK without preserving structure
            aimlsdk_folder_path = slc_output_dir + "aiml_" + aiml_extension_version
            for (root, dirs, file) in os.walk(aimlsdk_folder_path):
                for f in file:
                    filepath = os.path.join(root, f)
                    if f.endswith('.h'):
                        # Skip the 'tflite-micro' headers
                        if "tflite-micro/tensorflow" in filepath:
                            print(f"Skipping '{f}' as it is in 'tflite-micro/tensorflow'")
                            continue
                        if "include/flatbuffers" in filepath:
                            shutil.copyfile(filepath, header_output_dir + "flatbuffers/" + str(f))
                        elif "gemmlowp/fixedpoint" in filepath:
                            shutil.copyfile(filepath, header_output_dir + "fixedpoint/" + str(f))
                        elif "gemmlowp/internal" in filepath:
                            shutil.copyfile(filepath, header_output_dir + "internal/" + str(f))
                        else:
                            shutil.copyfile(filepath, header_output_dir + str(f))
                        print(f"Copying '{filepath}'")
            # Copy the headers from tflite-micro with preserving the structure
            shutil.copytree(slc_output_dir + "aiml_" + aiml_extension_version + "/third_party/tflite-micro/tensorflow", header_output_dir + "tensorflow")
            # Remove 'cc' files
            for (root, dirs, file) in os.walk(header_output_dir):
                for f in file:
                    filepath = os.path.join(root, f)
                    if f.endswith('.cc'):
                        os.remove(filepath)

    # Copy BLE HCI header files if the variant is 'BLE (Arduino)'
    if is_ble_arduino:
        ble_hci_header_dir = gsdk_dir + "/protocol/bluetooth/bgstack/ll/inc/"
        ble_hci_headers = ["sl_btctrl_hci_handler.h", "sl_btctrl_hci.h", "sl_hci_common_transport.h"]
        for header in ble_hci_headers:
            src_path = os.path.join(ble_hci_header_dir, header)
            if os.path.exists(src_path):
                shutil.copy(src_path, output_dir + "/include/")
                print(f"Copying '{header}'")
        print(f"Copied BLE HCI header files")

    # Get the linkerfile
    shutil.copy(slc_output_dir + "autogen/linkerfile.ld", output_dir)

    print("Applying license to GSDK folder...")
    shutil.copy(gsdk_license_file, output_dir + "/LICENSE")


def copy_generated_sdk_to_variants(output_dir, variant_target_folder):
    """
    Copy the generated SDK to the variants folder
    """
    print("Copying generated SDK to variants folder...")

    print(f"Target folder: {variant_target_folder}")
    if os.path.exists(variant_target_folder):
        shutil.rmtree(variant_target_folder)

    shutil.copytree(output_dir, variant_target_folder + os.path.basename(output_dir))
    print("Finished copying to variants folder")

def cleanup(gsdk_soc_empty_folder, slcp_file_name, additional_files, keep_gen_folder=False):
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
    # Remove the generated project if requested
    if keep_gen_folder:
        print("Keeping the 'gen' folder")
    else:
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


def get_keep_gen_folder_parameter_from_arguments():
    """
    Determines if the generated folder should be kept from the command line arguments
    """
    try:
        input_keep_gen_folder = sys.argv[2]
    except Exception:
        return False

    if input_keep_gen_folder == "--keep" or input_keep_gen_folder == "-k":
        return True
    else:
        return False

def read_config_file(config_file_path):
    with open(config_file_path, "r") as f:
        global gsdk_dir
        global slc_output_dir
        global gsdk_version
        global matter_extension_version
        global aiml_extension_version
        lines = f.readlines()
        for line in lines:
            if line.startswith("#") or len(line) < 3:
                continue
            config = line.split(" ")[0].strip()
            value = line.split(" ")[1].strip()
            if config == "gsdk_dir":
                gsdk_dir = value
            if config == "slc_output_dir":
                slc_output_dir = value
            if config == "gsdk_version":
                gsdk_version = value
            if config == "matter_extension_version":
                matter_extension_version = value
            if config == "aiml_extension_version":
                aiml_extension_version = value
        if gsdk_dir is None or slc_output_dir is None or gsdk_version is None or matter_extension_version is None:
            print(f"Incomplete or incorrect config file ({config_file_path}), exiting...")
            exit(-1)


# Configurations
all_platform_config = {
    "name": "all"
}

noradio_precomp_all_platform_config = {
    "name": "noradio_precomp_all",
}

ble_arduino_precomp_all_platform_config = {
    "name": "ble_arduino_precomp_all",
}

ble_silabs_precomp_all_platform_config = {
    "name": "ble_silabs_precomp_all",
}

matter_precomp_all_platform_config = {
    "name": "matter_precomp_all",
}

thingplusmatter_noradio_platform_config = {
    "name": "thingplusmatter_noradio",
    "arduino_variant_name": "thingplusmatter",
    "board_opn": "brd2704a",
    "ai_capable": False, # TODO: True
    "prebuild": False,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/thingplusmatter/thingplusmatter_noradio.slcp",
    "additional_files": ["slcp/thingplusmatter/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter/sl_iostream_eusart_thingplus1_config.h"]
}

thingplusmatter_noradio_prebuilt_platform_config = {
    "name": "thingplusmatter_noradio_precomp",
    "arduino_variant_name": "thingplusmatter",
    "board_opn": "brd2704a",
    "ai_capable": False, # TODO: True
    "prebuild": True,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/thingplusmatter/thingplusmatter_noradio.slcp",
    "additional_files": ["slcp/thingplusmatter/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter/sl_iostream_eusart_thingplus1_config.h"]
}

thingplusmatter_ble_arduino_platform_config = {
    "name": "thingplusmatter_ble_arduino",
    "arduino_variant_name": "thingplusmatter",
    "board_opn": "brd2704a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/thingplusmatter/thingplusmatter_ble_arduino.slcp",
    "additional_files": ["slcp/thingplusmatter/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter/sl_iostream_eusart_thingplus1_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

thingplusmatter_ble_arduino_prebuilt_platform_config = {
    "name": "thingplusmatter_ble_arduino_precomp",
    "arduino_variant_name": "thingplusmatter",
    "board_opn": "brd2704a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/thingplusmatter/thingplusmatter_ble_arduino.slcp",
    "additional_files": ["slcp/thingplusmatter/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter/sl_iostream_eusart_thingplus1_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

thingplusmatter_ble_silabs_platform_config = {
    "name": "thingplusmatter_ble_silabs",
    "arduino_variant_name": "thingplusmatter",
    "board_opn": "brd2704a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/thingplusmatter/thingplusmatter_ble_silabs.slcp",
    "additional_files": ["slcp/thingplusmatter/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter/sl_iostream_eusart_thingplus1_config.h"]
}

thingplusmatter_ble_silabs_prebuilt_platform_config = {
    "name": "thingplusmatter_ble_silabs_precomp",
    "arduino_variant_name": "thingplusmatter",
    "board_opn": "brd2704a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/thingplusmatter/thingplusmatter_ble_silabs.slcp",
    "additional_files": ["slcp/thingplusmatter/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter/sl_iostream_eusart_thingplus1_config.h"]
}

thingplusmatter_matter_platform_config = {
    "name": "thingplusmatter_matter",
    "arduino_variant_name": "thingplusmatter",
    "board_opn": "brd2704a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/thingplusmatter/thingplusmatter_matter.slcp",
    "additional_files": ["slcp/thingplusmatter/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter/sl_iostream_eusart_thingplus1_config.h",
                         ["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Silicon Labs",
    "matter_vendor_id": "0x1049"
}

thingplusmatter_matter_prebuilt_platform_config = {
    "name": "thingplusmatter_matter_precomp",
    "arduino_variant_name": "thingplusmatter",
    "board_opn": "brd2704a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/thingplusmatter/thingplusmatter_matter.slcp",
    "additional_files": ["slcp/thingplusmatter/sl_spidrv_eusart_thingplus1_config.h",
                         "slcp/thingplusmatter/sl_iostream_eusart_thingplus1_config.h",
                         ["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Silicon Labs",
    "matter_vendor_id": "0x1049"
}

xg27devkit_noradio_platform_config = {
    "name": "xg27devkit_noradio",
    "arduino_variant_name": "xg27devkit",
    "board_opn": "brd2602a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/xg27devkit/xg27devkit_noradio.slcp",
    "additional_files": ["slcp/xg27devkit/sl_iostream_usart_xg27devkit1_config.h"]
}

xg27devkit_noradio_prebuilt_platform_config = {
    "name": "xg27devkit_noradio_precomp",
    "arduino_variant_name": "xg27devkit",
    "board_opn": "brd2602a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/xg27devkit/xg27devkit_noradio.slcp",
    "additional_files": ["slcp/xg27devkit/sl_iostream_usart_xg27devkit1_config.h"]
}

xg27devkit_ble_arduino_platform_config = {
    "name": "xg27devkit_ble_arduino",
    "arduino_variant_name": "xg27devkit",
    "board_opn": "brd2602a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/xg27devkit/xg27devkit_ble_arduino.slcp",
    "additional_files": ["slcp/xg27devkit/sl_iostream_usart_xg27devkit1_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

xg27devkit_ble_arduino_prebuilt_platform_config = {
    "name": "xg27devkit_ble_arduino_precomp",
    "arduino_variant_name": "xg27devkit",
    "board_opn": "brd2602a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/xg27devkit/xg27devkit_ble_arduino.slcp",
    "additional_files": ["slcp/xg27devkit/sl_iostream_usart_xg27devkit1_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

xg27devkit_ble_silabs_platform_config = {
    "name": "xg27devkit_ble_silabs",
    "arduino_variant_name": "xg27devkit",
    "board_opn": "brd2602a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/xg27devkit/xg27devkit_ble_silabs.slcp",
    "additional_files": ["slcp/xg27devkit/sl_iostream_usart_xg27devkit1_config.h"]
}

xg27devkit_ble_silabs_prebuilt_platform_config = {
    "name": "xg27devkit_ble_silabs_precomp",
    "arduino_variant_name": "xg27devkit",
    "board_opn": "brd2602a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/xg27devkit/xg27devkit_ble_silabs.slcp",
    "additional_files": ["slcp/xg27devkit/sl_iostream_usart_xg27devkit1_config.h"]
}

xg24explorerkit_noradio_platform_config = {
    "name": "xg24explorerkit_noradio",
    "arduino_variant_name": "xg24explorerkit",
    "board_opn": "brd2703a",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit_noradio.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h"]
}

xg24explorerkit_noradio_prebuilt_platform_config = {
    "name": "xg24explorerkit_noradio_precomp",
    "arduino_variant_name": "xg24explorerkit",
    "board_opn": "brd2703a",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit_noradio.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h"]
}

xg24explorerkit_ble_arduino_platform_config = {
    "name": "xg24explorerkit_ble_arduino",
    "arduino_variant_name": "xg24explorerkit",
    "board_opn": "brd2703a",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit_ble_arduino.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

xg24explorerkit_ble_arduino_prebuilt_platform_config = {
    "name": "xg24explorerkit_ble_arduino_precomp",
    "arduino_variant_name": "xg24explorerkit",
    "board_opn": "brd2703a",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit_ble_arduino.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

xg24explorerkit_ble_silabs_platform_config = {
    "name": "xg24explorerkit_ble_silabs",
    "arduino_variant_name": "xg24explorerkit",
    "board_opn": "brd2703a",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit_ble_silabs.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h"]
}

xg24explorerkit_ble_silabs_prebuilt_platform_config = {
    "name": "xg24explorerkit_ble_silabs_precomp",
    "arduino_variant_name": "xg24explorerkit",
    "board_opn": "brd2703a",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit_ble_silabs.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h"]
}

xg24explorerkit_matter_platform_config = {
    "name": "xg24explorerkit_matter",
    "arduino_variant_name": "xg24explorerkit",
    "board_opn": "brd2703a",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit_matter.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h",
                         ["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Silicon Labs",
    "matter_vendor_id": "0x1049"
}

xg24explorerkit_matter_prebuilt_platform_config = {
    "name": "xg24explorerkit_matter_precomp",
    "arduino_variant_name": "xg24explorerkit",
    "board_opn": "brd2703a",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/xg24explorerkit/xg24explorerkit_matter.slcp",
    "additional_files": ["slcp/xg24explorerkit/sl_spidrv_eusart_xg24explorerkit1_config.h",
                         "slcp/xg24explorerkit/sl_iostream_eusart_xg24explorerkit1_config.h",
                         ["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Silicon Labs",
    "matter_vendor_id": "0x1049"
}

xg24devkit_noradio_platform_config = {
    "name": "xg24devkit_noradio",
    "arduino_variant_name": "xg24devkit",
    "board_opn": "brd2601b",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/xg24devkit/xg24devkit_noradio.slcp",
    "additional_files": []
}

xg24devkit_noradio_prebuilt_platform_config = {
    "name": "xg24devkit_noradio_precomp",
    "arduino_variant_name": "xg24devkit",
    "board_opn": "brd2601b",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/xg24devkit/xg24devkit_noradio.slcp",
    "additional_files": []
}

xg24devkit_ble_arduino_platform_config = {
    "name": "xg24devkit_ble_arduino",
    "arduino_variant_name": "xg24devkit",
    "board_opn": "brd2601b",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/xg24devkit/xg24devkit_ble_arduino.slcp",
    "additional_files": [["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

xg24devkit_ble_arduino_prebuilt_platform_config = {
    "name": "xg24devkit_ble_arduino_precomp",
    "arduino_variant_name": "xg24devkit",
    "board_opn": "brd2601b",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/xg24devkit/xg24devkit_ble_arduino.slcp",
    "additional_files": [["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

xg24devkit_ble_silabs_platform_config = {
    "name": "xg24devkit_ble_silabs",
    "arduino_variant_name": "xg24devkit",
    "board_opn": "brd2601b",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/xg24devkit/xg24devkit_ble_silabs.slcp",
    "additional_files": []
}

xg24devkit_ble_silabs_prebuilt_platform_config = {
    "name": "xg24devkit_ble_silabs_precomp",
    "arduino_variant_name": "xg24devkit",
    "board_opn": "brd2601b",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/xg24devkit/xg24devkit_ble_silabs.slcp",
    "additional_files": []
}

xg24devkit_matter_platform_config = {
    "name": "xg24devkit_matter",
    "arduino_variant_name": "xg24devkit",
    "board_opn": "brd2601b",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/xg24devkit/xg24devkit_matter.slcp",
    "additional_files": [["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Silicon Labs",
    "matter_vendor_id": "0x1049"
}

xg24devkit_matter_prebuilt_platform_config = {
    "name": "xg24devkit_matter_precomp",
    "arduino_variant_name": "xg24devkit",
    "board_opn": "brd2601b",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/xg24devkit/xg24devkit_matter.slcp",
    "additional_files": [["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Silicon Labs",
    "matter_vendor_id": "0x1049"
}

wio_mg24_noradio_platform_config = {
    "name": "wio_mg24_noradio",
    "arduino_variant_name": "wio_mg24",
    "board_opn": "brd2907a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/wio_mg24/wio_mg24_noradio.slcp",
    "additional_files": ["slcp/wio_mg24/sl_spidrv_eusart_wio_mg24_config.h",
                         "slcp/wio_mg24/sl_iostream_eusart_wio_mg24_config.h",
                         ["slcp/wio_mg24/brd2907a.slcc", "hardware/board/component/"],
                         ["slcp/wio_mg24/brd2907a_config.slcc", "hardware/board/config/component/"]]
}

wio_mg24_noradio_prebuilt_platform_config = {
    "name": "wio_mg24_noradio_precomp",
    "arduino_variant_name": "wio_mg24",
    "board_opn": "brd2907a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/wio_mg24/wio_mg24_noradio.slcp",
    "additional_files": ["slcp/wio_mg24/sl_spidrv_eusart_wio_mg24_config.h",
                         "slcp/wio_mg24/sl_iostream_eusart_wio_mg24_config.h",
                         ["slcp/wio_mg24/brd2907a.slcc", "hardware/board/component/"],
                         ["slcp/wio_mg24/brd2907a_config.slcc", "hardware/board/config/component/"]]
}

wio_mg24_ble_arduino_platform_config = {
    "name": "wio_mg24_ble_arduino",
    "arduino_variant_name": "wio_mg24",
    "board_opn": "brd2907a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/wio_mg24/wio_mg24_ble_arduino.slcp",
    "additional_files": ["slcp/wio_mg24/sl_spidrv_eusart_wio_mg24_config.h",
                         "slcp/wio_mg24/sl_iostream_eusart_wio_mg24_config.h",
                         ["slcp/wio_mg24/brd2907a.slcc", "hardware/board/component/"],
                         ["slcp/wio_mg24/brd2907a_config.slcc", "hardware/board/config/component/"],
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

wio_mg24_ble_arduino_prebuilt_platform_config = {
    "name": "wio_mg24_ble_arduino_precomp",
    "arduino_variant_name": "wio_mg24",
    "board_opn": "brd2907a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/wio_mg24/wio_mg24_ble_arduino.slcp",
    "additional_files": ["slcp/wio_mg24/sl_spidrv_eusart_wio_mg24_config.h",
                         "slcp/wio_mg24/sl_iostream_eusart_wio_mg24_config.h",
                         ["slcp/wio_mg24/brd2907a.slcc", "hardware/board/component/"],
                         ["slcp/wio_mg24/brd2907a_config.slcc", "hardware/board/config/component/"],
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

wio_mg24_ble_silabs_platform_config = {
    "name": "wio_mg24_ble_silabs",
    "arduino_variant_name": "wio_mg24",
    "board_opn": "brd2907a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/wio_mg24/wio_mg24_ble_silabs.slcp",
    "additional_files": ["slcp/wio_mg24/sl_spidrv_eusart_wio_mg24_config.h",
                         "slcp/wio_mg24/sl_iostream_eusart_wio_mg24_config.h",
                         ["slcp/wio_mg24/brd2907a.slcc", "hardware/board/component/"],
                         ["slcp/wio_mg24/brd2907a_config.slcc", "hardware/board/config/component/"]]
}

wio_mg24_ble_silabs_prebuilt_platform_config = {
    "name": "wio_mg24_ble_silabs_precomp",
    "arduino_variant_name": "wio_mg24",
    "board_opn": "brd2907a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/wio_mg24/wio_mg24_ble_silabs.slcp",
    "additional_files": ["slcp/wio_mg24/sl_spidrv_eusart_wio_mg24_config.h",
                         "slcp/wio_mg24/sl_iostream_eusart_wio_mg24_config.h",
                         ["slcp/wio_mg24/brd2907a.slcc", "hardware/board/component/"],
                         ["slcp/wio_mg24/brd2907a_config.slcc", "hardware/board/config/component/"]]
}

bgm220explorerkit_noradio_platform_config = {
    "name": "bgm220explorerkit_noradio",
    "arduino_variant_name": "bgm220explorerkit",
    "board_opn": "brd4314a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/bgm220explorerkit/bgm220explorerkit_noradio.slcp",
    "additional_files": []
}

bgm220explorerkit_noradio_prebuilt_platform_config = {
    "name": "bgm220explorerkit_noradio_precomp",
    "arduino_variant_name": "bgm220explorerkit",
    "board_opn": "brd4314a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/bgm220explorerkit/bgm220explorerkit_noradio.slcp",
    "additional_files": []
}

bgm220explorerkit_ble_arduino_platform_config = {
    "name": "bgm220explorerkit_ble_arduino",
    "arduino_variant_name": "bgm220explorerkit",
    "board_opn": "brd4314a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/bgm220explorerkit/bgm220explorerkit_ble_arduino.slcp",
    "additional_files": [["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

bgm220explorerkit_ble_arduino_prebuilt_platform_config = {
    "name": "bgm220explorerkit_ble_arduino_precomp",
    "arduino_variant_name": "bgm220explorerkit",
    "board_opn": "brd4314a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/bgm220explorerkit/bgm220explorerkit_ble_arduino.slcp",
    "additional_files": [["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

bgm220explorerkit_ble_silabs_platform_config = {
    "name": "bgm220explorerkit_ble_silabs",
    "arduino_variant_name": "bgm220explorerkit",
    "board_opn": "brd4314a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/bgm220explorerkit/bgm220explorerkit_ble_silabs.slcp",
    "additional_files": []
}

bgm220explorerkit_ble_silabs_prebuilt_platform_config = {
    "name": "bgm220explorerkit_ble_silabs_precomp",
    "arduino_variant_name": "bgm220explorerkit",
    "board_opn": "brd4314a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/bgm220explorerkit/bgm220explorerkit_ble_silabs.slcp",
    "additional_files": []
}

nanomatter_noradio_platform_config = {
    "name": "nano_matter_noradio",
    "arduino_variant_name": "nano_matter",
    "board_opn": "brd2707a",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/nano_matter/nano_matter_noradio.slcp",
    "additional_files": ["slcp/nano_matter/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nano_matter/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nano_matter/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter1_config.h",
                         ["slcp/nano_matter/brd2707a.slcc", "hardware/board/component/"],
                         ["slcp/nano_matter/brd2707a_config.slcc", "hardware/board/config/component/"]]
}

nanomatter_noradio_prebuilt_platform_config = {
    "name": "nano_matter_noradio_precomp",
    "arduino_variant_name": "nano_matter",
    "board_opn": "brd2707a",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/nano_matter/nano_matter_noradio.slcp",
    "additional_files": ["slcp/nano_matter/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nano_matter/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nano_matter/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter1_config.h",
                         ["slcp/nano_matter/brd2707a.slcc", "hardware/board/component/"],
                         ["slcp/nano_matter/brd2707a_config.slcc", "hardware/board/config/component/"]]
}

nanomatter_ble_arduino_platform_config = {
    "name": "nano_matter_ble_arduino",
    "arduino_variant_name": "nano_matter",
    "board_opn": "brd2707a",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/nano_matter/nano_matter_ble_arduino.slcp",
    "additional_files": ["slcp/nano_matter/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nano_matter/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nano_matter/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter1_config.h",
                         ["slcp/nano_matter/brd2707a.slcc", "hardware/board/component/"],
                         ["slcp/nano_matter/brd2707a_config.slcc", "hardware/board/config/component/"],
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

nanomatter_ble_arduino_prebuilt_platform_config = {
    "name": "nano_matter_ble_arduino_precomp",
    "arduino_variant_name": "nano_matter",
    "board_opn": "brd2707a",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/nano_matter/nano_matter_ble_arduino.slcp",
    "additional_files": ["slcp/nano_matter/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nano_matter/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nano_matter/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter1_config.h",
                         ["slcp/nano_matter/brd2707a.slcc", "hardware/board/component/"],
                         ["slcp/nano_matter/brd2707a_config.slcc", "hardware/board/config/component/"],
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

nanomatter_ble_silabs_platform_config = {
    "name": "nano_matter_ble_silabs",
    "arduino_variant_name": "nano_matter",
    "board_opn": "brd2707a",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/nano_matter/nano_matter_ble_silabs.slcp",
    "additional_files": ["slcp/nano_matter/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nano_matter/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nano_matter/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter1_config.h",
                         ["slcp/nano_matter/brd2707a.slcc", "hardware/board/component/"],
                         ["slcp/nano_matter/brd2707a_config.slcc", "hardware/board/config/component/"]]
}

nanomatter_ble_silabs_prebuilt_platform_config = {
    "name": "nano_matter_ble_silabs_precomp",
    "arduino_variant_name": "nano_matter",
    "board_opn": "brd2707a",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/nano_matter/nano_matter_ble_silabs.slcp",
    "additional_files": ["slcp/nano_matter/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nano_matter/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nano_matter/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter1_config.h",
                         ["slcp/nano_matter/brd2707a.slcc", "hardware/board/component/"],
                         ["slcp/nano_matter/brd2707a_config.slcc", "hardware/board/config/component/"]]
}

nanomatter_matter_platform_config = {
    "name": "nano_matter_matter",
    "arduino_variant_name": "nano_matter",
    "board_opn": "brd2707a",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/nano_matter/nano_matter_matter.slcp",
    "additional_files": ["slcp/nano_matter/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nano_matter/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nano_matter/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter1_config.h",
                         ["slcp/nano_matter/brd2707a.slcc", "hardware/board/component/"],
                         ["slcp/nano_matter/brd2707a_config.slcc", "hardware/board/config/component/"],
                         ["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Arduino",
    "matter_vendor_id": "0x1515"
}

nanomatter_matter_prebuilt_platform_config = {
    "name": "nano_matter_matter_precomp",
    "arduino_variant_name": "nano_matter",
    "board_opn": "brd2707a",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/nano_matter/nano_matter_matter.slcp",
    "additional_files": ["slcp/nano_matter/sl_spidrv_eusart_nanomatter_config.h",
                         "slcp/nano_matter/sl_spidrv_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_iostream_usart_nanomatter_config.h",
                         "slcp/nano_matter/sl_iostream_eusart_nanomatter1_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter_config.h",
                         "slcp/nano_matter/sl_i2cspm_nanomatter1_config.h",
                         ["slcp/nano_matter/brd2707a.slcc", "hardware/board/component/"],
                         ["slcp/nano_matter/brd2707a_config.slcc", "hardware/board/config/component/"],
                         ["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Arduino",
    "matter_vendor_id": "0x1515"
}

lyra24p20_noradio_platform_config = {
    "name": "lyra24p20_noradio",
    "arduino_variant_name": "lyra24p20",
    "board_opn": "brd2904a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/lyra24p20/lyra24p20_noradio.slcp",
    "additional_files": ["slcp/lyra24p20/sl_spidrv_eusart_lyra24p20_config.h",
                         "slcp/lyra24p20/sl_iostream_eusart_lyra24p20_config.h"]
}

lyra24p20_noradio_prebuilt_platform_config = {
    "name": "lyra24p20_noradio_precomp",
    "arduino_variant_name": "lyra24p20",
    "board_opn": "brd2904a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/lyra24p20/lyra24p20_noradio.slcp",
    "additional_files": ["slcp/lyra24p20/sl_spidrv_eusart_lyra24p20_config.h",
                         "slcp/lyra24p20/sl_iostream_eusart_lyra24p20_config.h"]
}

lyra24p20_ble_arduino_platform_config = {
    "name": "lyra24p20_ble_arduino",
    "arduino_variant_name": "lyra24p20",
    "board_opn": "brd2904a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/lyra24p20/lyra24p20_ble_arduino.slcp",
    "additional_files": ["slcp/lyra24p20/sl_spidrv_eusart_lyra24p20_config.h",
                         "slcp/lyra24p20/sl_iostream_eusart_lyra24p20_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

lyra24p20_ble_arduino_prebuilt_platform_config = {
    "name": "lyra24p20_ble_arduino_precomp",
    "arduino_variant_name": "lyra24p20",
    "board_opn": "brd2904a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/lyra24p20/lyra24p20_ble_arduino.slcp",
    "additional_files": ["slcp/lyra24p20/sl_spidrv_eusart_lyra24p20_config.h",
                         "slcp/lyra24p20/sl_iostream_eusart_lyra24p20_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

lyra24p20_ble_silabs_platform_config = {
    "name": "lyra24p20_ble_silabs",
    "arduino_variant_name": "lyra24p20",
    "board_opn": "brd2904a",
    "ai_capable": False,
    "prebuild": False,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/lyra24p20/lyra24p20_ble_silabs.slcp",
    "additional_files": ["slcp/lyra24p20/sl_spidrv_eusart_lyra24p20_config.h",
                         "slcp/lyra24p20/sl_iostream_eusart_lyra24p20_config.h"]
}

lyra24p20_ble_silabs_prebuilt_platform_config = {
    "name": "lyra24p20_ble_silabs_precomp",
    "arduino_variant_name": "lyra24p20",
    "board_opn": "brd2904a",
    "ai_capable": False,
    "prebuild": True,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/lyra24p20/lyra24p20_ble_silabs.slcp",
    "additional_files": ["slcp/lyra24p20/sl_spidrv_eusart_lyra24p20_config.h",
                         "slcp/lyra24p20/sl_iostream_eusart_lyra24p20_config.h"]
}

xiao_mg24_noradio_platform_config = {
    "name": "xiao_mg24_noradio",
    "arduino_variant_name": "xiao_mg24",
    "board_opn": "brd4187c",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/xiao_mg24/xiao_mg24_noradio.slcp",
    "additional_files": ["slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_iostream_usart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_iostream_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_1_config.h"]
}

xiao_mg24_noradio_prebuilt_platform_config = {
    "name": "xiao_mg24_noradio_precomp",
    "arduino_variant_name": "xiao_mg24",
    "board_opn": "brd4187c",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'noradio',
    "slcp_file": "slcp/xiao_mg24/xiao_mg24_noradio.slcp",
    "additional_files": ["slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_iostream_usart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_iostream_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_1_config.h"]
}

xiao_mg24_ble_arduino_platform_config = {
    "name": "xiao_mg24_ble_arduino",
    "arduino_variant_name": "xiao_mg24",
    "board_opn": "brd4187c",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/xiao_mg24/xiao_mg24_ble_arduino.slcp",
    "additional_files": ["slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_iostream_usart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_iostream_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_1_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

xiao_mg24_ble_arduino_prebuilt_platform_config = {
    "name": "xiao_mg24_ble_arduino_precomp",
    "arduino_variant_name": "xiao_mg24",
    "board_opn": "brd4187c",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'ble_arduino',
    "slcp_file": "slcp/xiao_mg24/xiao_mg24_ble_arduino.slcp",
    "additional_files": ["slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_iostream_usart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_iostream_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_1_config.h",
                         ["slcp/common/bluetooth_hci_reset.slcc", "protocol/bluetooth/bgstack/ll/component/"]]
}

xiao_mg24_ble_silabs_platform_config = {
    "name": "xiao_mg24_ble_silabs",
    "arduino_variant_name": "xiao_mg24",
    "board_opn": "brd4187c",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/xiao_mg24/xiao_mg24_ble_silabs.slcp",
    "additional_files": ["slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_iostream_usart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_iostream_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_1_config.h"]
}

xiao_mg24_ble_silabs_prebuilt_platform_config = {
    "name": "xiao_mg24_ble_silabs_precomp",
    "arduino_variant_name": "xiao_mg24",
    "board_opn": "brd4187c",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'ble_silabs',
    "slcp_file": "slcp/xiao_mg24/xiao_mg24_ble_silabs.slcp",
    "additional_files": ["slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_iostream_usart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_iostream_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_1_config.h"]
}

xiao_mg24_matter_platform_config = {
    "name": "xiao_mg24_matter",
    "arduino_variant_name": "xiao_mg24",
    "board_opn": "brd4187c",
    "ai_capable": True,
    "prebuild": False,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/xiao_mg24/xiao_mg24_matter.slcp",
    "additional_files": ["slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_iostream_usart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_iostream_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_1_config.h",
                         ["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Silicon Labs",
    "matter_vendor_id": "0x1049"
}

xiao_mg24_matter_prebuilt_platform_config = {
    "name": "xiao_mg24_matter_precomp",
    "arduino_variant_name": "xiao_mg24",
    "board_opn": "brd4187c",
    "ai_capable": True,
    "prebuild": True,
    "protocol_stack": 'matter',
    "slcp_file": "slcp/xiao_mg24/xiao_mg24_matter.slcp",
    "additional_files": ["slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_spidrv_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_iostream_usart_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_iostream_eusart_xiao_mg24_1_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_config.h",
                         "slcp/xiao_mg24/sl_i2cspm_xiao_mg24_1_config.h",
                         ["slcp/common/MatterBLE.h", "extension/matter_extension/third_party/matter_sdk/examples/lighting-app/silabs/include/"]],
    "matter_zap_file": "slcp/common/arduino_matter.zap",
    "matter_vendor_name": "Silicon Labs",
    "matter_vendor_id": "0x1049"
}

platform_configurations = [all_platform_config,
                           noradio_precomp_all_platform_config,
                           ble_arduino_precomp_all_platform_config,
                           ble_silabs_precomp_all_platform_config,
                           matter_precomp_all_platform_config,
                           thingplusmatter_noradio_platform_config,
                           thingplusmatter_noradio_prebuilt_platform_config,
                           thingplusmatter_ble_arduino_platform_config,
                           thingplusmatter_ble_arduino_prebuilt_platform_config,
                           thingplusmatter_ble_silabs_platform_config,
                           thingplusmatter_ble_silabs_prebuilt_platform_config,
                           thingplusmatter_matter_platform_config,
                           thingplusmatter_matter_prebuilt_platform_config,
                           xg27devkit_noradio_platform_config,
                           xg27devkit_noradio_prebuilt_platform_config,
                           xg27devkit_ble_arduino_platform_config,
                           xg27devkit_ble_arduino_prebuilt_platform_config,
                           xg27devkit_ble_silabs_platform_config,
                           xg27devkit_ble_silabs_prebuilt_platform_config,
                           xg24explorerkit_noradio_platform_config,
                           xg24explorerkit_noradio_prebuilt_platform_config,
                           xg24explorerkit_ble_arduino_platform_config,
                           xg24explorerkit_ble_arduino_prebuilt_platform_config,
                           xg24explorerkit_ble_silabs_platform_config,
                           xg24explorerkit_ble_silabs_prebuilt_platform_config,
                           xg24explorerkit_matter_platform_config,
                           xg24explorerkit_matter_prebuilt_platform_config,
                           xg24devkit_noradio_platform_config,
                           xg24devkit_noradio_prebuilt_platform_config,
                           xg24devkit_ble_arduino_platform_config,
                           xg24devkit_ble_arduino_prebuilt_platform_config,
                           xg24devkit_ble_silabs_platform_config,
                           xg24devkit_ble_silabs_prebuilt_platform_config,
                           xg24devkit_matter_platform_config,
                           xg24devkit_matter_prebuilt_platform_config,
                           #wio_mg24_noradio_platform_config,
                           #wio_mg24_noradio_prebuilt_platform_config,
                           #wio_mg24_ble_arduino_platform_config,
                           #wio_mg24_ble_arduino_prebuilt_platform_config,
                           bgm220explorerkit_noradio_platform_config,
                           bgm220explorerkit_noradio_prebuilt_platform_config,
                           bgm220explorerkit_ble_arduino_platform_config,
                           bgm220explorerkit_ble_arduino_prebuilt_platform_config,
                           bgm220explorerkit_ble_silabs_platform_config,
                           bgm220explorerkit_ble_silabs_prebuilt_platform_config,
                           nanomatter_noradio_platform_config,
                           nanomatter_noradio_prebuilt_platform_config,
                           nanomatter_ble_arduino_platform_config,
                           nanomatter_ble_arduino_prebuilt_platform_config,
                           nanomatter_ble_silabs_platform_config,
                           nanomatter_ble_silabs_prebuilt_platform_config,
                           nanomatter_matter_platform_config,
                           nanomatter_matter_prebuilt_platform_config,
                           lyra24p20_noradio_platform_config,
                           lyra24p20_noradio_prebuilt_platform_config,
                           lyra24p20_ble_arduino_platform_config,
                           lyra24p20_ble_arduino_prebuilt_platform_config,
                           lyra24p20_ble_silabs_platform_config,
                           lyra24p20_ble_silabs_prebuilt_platform_config,
                           xiao_mg24_noradio_platform_config,
                           xiao_mg24_noradio_prebuilt_platform_config,
                           xiao_mg24_ble_arduino_platform_config,
                           xiao_mg24_ble_arduino_prebuilt_platform_config,
                           xiao_mg24_ble_silabs_platform_config,
                           xiao_mg24_ble_silabs_prebuilt_platform_config,
                           xiao_mg24_matter_platform_config,
                           xiao_mg24_matter_prebuilt_platform_config]


if __name__ == "__main__":
    main()
