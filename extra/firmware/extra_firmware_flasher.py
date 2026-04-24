#
# This file is part of the Silicon Labs Arduino Core
#
# The MIT License (MIT)
#
# Copyright 2025 Silicon Laboratories Inc. www.silabs.com
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

import platform
import getpass
import subprocess
import sys


def main():
    print("Arduino Extra Firmware Flasher Tool")
    print("------------------------------------------")
    selected_board_config, selected_firmware_type = get_selected_board_config_from_arguments()
    print(f"Selected board: {selected_board_config['name']}")
    print(f"Selected firmware type: {selected_firmware_type}")

    # Get the flasher tool executable path
    arduino_tools_dir = get_arduino_tools_dir_path()
    if arduino_tools_dir is None:
        print("Arduino Core tools directory not found; Silabs Core not installed or unsupported OS")
        sys.exit(1)

    flasher_tool_executable_path = get_flasher_tool_executable_path(selected_board_config['flasher_tool'], arduino_tools_dir)
    if flasher_tool_executable_path is None:
        print("Unsupported flasher tool!")
        sys.exit(1)
    print("Flasher tool location: {}".format(flasher_tool_executable_path))

    firmware_binary_path = None
    if selected_firmware_type == 'thread':
        firmware_binary_path = selected_board_config['thread_rcp_binary']
    elif selected_firmware_type == 'zigbee':
        firmware_binary_path = selected_board_config['zigbee_ezsp_binary']
    else:
        print("Unsupported firmware type!")
        sys.exit(1)

    # Flash the bootloader binary file for the selected board
    flash_binary(flasher_tool_executable_path,
                 selected_board_config['bootloader_binary'],
                 arduino_tools_dir,
                 selected_board_config['flasher_tool'])

    # Flash the binary file for the selected board
    flash_binary(flasher_tool_executable_path,
                 firmware_binary_path,
                 arduino_tools_dir,
                 selected_board_config['flasher_tool'])

    print("Finished successfully")


def flash_binary(flasher_tool_executable_path, binary_file, arduino_tools_dir, flasher_tool):
    print("")
    print("Flashing binary file: {}".format(binary_file))
    if flasher_tool == 'openocd':
        flasher_process = subprocess.Popen(
            [flasher_tool_executable_path, "-d2", "-s", arduino_tools_dir + "/openocd/0.12.0-arduino1-static/share/openocd/scripts/", "-f", "interface/cmsis-dap.cfg", "-f", "target/efm32s2_g23.cfg", "-c", "init; reset_config srst_nogate; reset halt; program {" + binary_file + "}; reset; exit"]
        )
        flasher_process.communicate(timeout=30)
    elif flasher_tool == 'simplicitycommander':
        flasher_process = subprocess.Popen(
            [flasher_tool_executable_path, "flash", binary_file, "-v"]
        )
        flasher_process.communicate(timeout=30)
    else:
        print("Unsupported flasher tool")
        sys.exit(1)

    if flasher_process.returncode != 0:
        print(f"Error: Flashing failed with return code '{flasher_process.returncode}'")
        sys.exit(flasher_process.returncode)


def get_selected_board_config_from_arguments():
    def print_available_configs():
        print("")
        print("Supported boards:")
        for board in supported_boards:
            print(board['name'])
        print("")
        print("Supported firmware types:")
        print("thread")
        print("zigbee")

    def print_usage():
        print("")
        print("Usage: python extra_firmware_flasher.py <board_name> <firmware_type>")
        print("Example: python extra_firmware_flasher.py thingplus_matter thread")
        print("Example: python extra_firmware_flasher.py nano_matter zigbee")

    try:
        input_board_name = sys.argv[1]
    except IndexError:
        print("Please provide a target board name in the first argument!")
        print_available_configs()
        print_usage()
        sys.exit(1)

    try:
        input_firmware_type = sys.argv[2]
    except IndexError:
        print("Please provide a firmware type in the second argument!")
        print_available_configs()
        print_usage()
        sys.exit(-1)

    found = False
    selected_board_config = None
    for board in supported_boards:
        if board['name'] == input_board_name:
            selected_board_config = board
            found = True
            break
    if not found:
        print(f"Board with the name '{input_board_name}' not found!")
        print_available_configs()
        sys.exit(1)

    if (input_firmware_type != 'thread') and (input_firmware_type != 'zigbee'):
        print("Please provide a valid firmware type ('thread' or 'zigbee') in the second argument!")
        print_usage()
        sys.exit(1)

    return selected_board_config, input_firmware_type


def get_arduino_tools_dir_path():
    username = getpass.getuser()
    arduino_tools_dir = None
    if platform.system() == 'Windows':
        arduino_tools_dir = 'C:\\Users\\{}\\AppData\\Local\\Arduino15\\packages\\SiliconLabs\\tools'.format(username)
    elif platform.system() == 'Linux':
        arduino_tools_dir = '/home/{}/.arduino15/packages/SiliconLabs/tools'.format(username)
    elif platform.system() == 'Darwin':
        arduino_tools_dir = '/Users/{}/Library/Arduino15/packages/SiliconLabs/tools'.format(username)

    return arduino_tools_dir


def get_flasher_tool_executable_path(flasher_tool, arduino_tools_dir):
    flasher_tool_executable_path = None
    if flasher_tool == 'openocd':
        flasher_tool_executable_path = arduino_tools_dir + "/openocd/0.12.0-arduino1-static/bin/openocd"
    elif flasher_tool == 'simplicitycommander':
        if platform.system() == 'Darwin':
            flasher_tool_executable_path = arduino_tools_dir + "/simplicitycommander/1.16.4/Commander.app/Contents/MacOS/commander"
        else:
            flasher_tool_executable_path = arduino_tools_dir + "/simplicitycommander/1.16.4/commander"

    return flasher_tool_executable_path


nanomatter_config = {
    'name': 'nano_matter',
    'flasher_tool': 'openocd',
    'bootloader_binary': "../../bootloaders/arduino-nano-matter-bootloader-storage-internal-single-512k.hex",
    'thread_rcp_binary': "openthread_rcp/openthread_rcp_nano_matter.hex",
    'zigbee_ezsp_binary': "zigbee_ezsp/zigbee_ncp_uart_hw_ezsp_nano_matter.hex"
}

thingplusmatter_config = {
    'name': 'thingplus_matter',
    'flasher_tool': 'simplicitycommander',
    'bootloader_binary': "../../bootloaders/sparkfun-thingplus-matter-bootloader-storage-internal-single-512k.hex",
    'thread_rcp_binary': "openthread_rcp/openthread_rcp_sparkfun_thingplusmatter.hex",
    'zigbee_ezsp_binary': "zigbee_ezsp/zigbee_ncp_uart_hw_ezsp_thingplus_matter.hex"
}

xiaomg24_config = {
    'name': 'xiao_mg24',
    'flasher_tool': 'openocd',
    'bootloader_binary': "../../bootloaders/seeed-studio-xiao-mg24-bootloader-storage-internal-single-512k.hex",
    'thread_rcp_binary': "openthread_rcp/openthread_rcp_xiao_mg24.hex",
    'zigbee_ezsp_binary': "zigbee_ezsp/zigbee_ncp_uart_hw_ezsp_xiao_mg24.hex"
}


supported_boards = [nanomatter_config,
                    thingplusmatter_config,
                    xiaomg24_config]


if __name__ == "__main__":
    main()
