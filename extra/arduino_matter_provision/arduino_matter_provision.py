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
    print("Arduino Matter Provision Data Flasher Tool")
    print("------------------------------------------")
    selected_board, config_num = get_selected_board_and_config_from_arguments()
    print(f"Selected board: {selected_board}")
    print(f"Selected config number: {config_num}")

    # Get the config for the selected board
    selected_board_config = None
    for board in supported_boards:
        if board['name'] == selected_board:
            selected_board_config = board
            break
    if selected_board_config is None:
        print("Invalid board selected!")
        exit(-1)

    # Get the flasher tool executable path
    arduino_tools_dir = get_arduino_tools_dir_path()
    if arduino_tools_dir is None:
        print("Arduino Core tools directory not found / Silabs Core not installed / unsupported OS")
        exit(-1)

    flasher_tool_executable_path = get_flasher_tool_executable_path(selected_board_config, arduino_tools_dir)
    if flasher_tool_executable_path is None:
        print("Unsupported flasher tool!")
        exit(-1)
    print("Flasher tool location: {}".format(flasher_tool_executable_path))

    # Get the binary file for the selected config
    provision_binary_file = None
    try:
        provision_binary_file = selected_board_config['configs'][config_num]
    except Exception:
        pass
    if provision_binary_file is None or config_num < 0:
        max_config_num = len(selected_board_config['configs']) - 1
        print("Selected config does not exist! Valid config numbers are: 0 to {}".format(max_config_num))
        exit(-1)

    provision_binary_file = "binaries/" + provision_binary_file

    print("Provision binary file: {}".format(provision_binary_file))

    # Flash the binary
    if selected_board_config['flasher_tool'] == 'openocd':
        flasher_process = subprocess.Popen(
            [flasher_tool_executable_path, "-d2", "-s", arduino_tools_dir + "/openocd/0.12.0-arduino1-static/share/openocd/scripts/", "-f", "interface/cmsis-dap.cfg", "-f", "target/efm32s2_g23.cfg", "-c", "init; reset_config srst_nogate; reset halt; program {" + provision_binary_file + "}; reset; exit"]
        )
        flasher_process.communicate(timeout=30)
    elif selected_board_config['flasher_tool'] == 'simplicitycommander':
        flasher_process = subprocess.Popen(
            [flasher_tool_executable_path, "flash", provision_binary_file, "-v"]
        )
        flasher_process.communicate(timeout=30)


def get_selected_board_and_config_from_arguments():
    def print_available_configs():
        print("")
        print("Supported boards:")
        for board in supported_boards:
            print(board['name'])

    def print_usage():
        print("")
        print("Usage: python arduino_matter_provision.py <board_name> <config_number>")
        print("Example: python arduino_matter_provision.py nano_matter 1")

    try:
        input_board_name = sys.argv[1]
    except Exception:
        print("Please provide a target board name in the first argument!")
        print_available_configs()
        print_usage()
        exit(-1)

    try:
        input_config_num = sys.argv[2]
    except Exception:
        print("Please provide a valid config number in the second argument!")
        print_usage()
        exit(-1)

    found = False
    for board in supported_boards:
        if board['name'] == input_board_name:
            found = True
            break
    if not found:
        print(f"Board with the name '{input_board_name}' not found!")
        print_available_configs()
        exit(-1)

    try:
        config_num = int(input_config_num)
    except Exception:
        print("Please provide a valid config number!")
        print_usage()
        exit(-1)
    return input_board_name, config_num


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


def get_flasher_tool_executable_path(selected_board_config, arduino_tools_dir):
    flasher_tool_executable_path = None
    if selected_board_config['flasher_tool'] == 'openocd':
        flasher_tool_executable_path = arduino_tools_dir + "/openocd/0.12.0-arduino1-static/bin/openocd"
    elif selected_board_config['flasher_tool'] == 'simplicitycommander':
        if platform.system() == 'Darwin':
            flasher_tool_executable_path = arduino_tools_dir + "/simplicitycommander/1.16.4/Commander.app/Contents/MacOS/commander"
        else:
            flasher_tool_executable_path = arduino_tools_dir + "/simplicitycommander/1.16.4/commander"

    return flasher_tool_executable_path


nanomatter_config = {
    'name': 'nano_matter',
    'flasher_tool': 'openocd',
    'configs': [
        "0_nanomatter_nvm3.hex",  # default config (empty nvm3)
        "1_nanomatter_nvm3.hex",  # provision.py -vi 0x1515 -sd 0x0040 -pi 0x4240 -sp 62034000 -sn 4267
        "2_nanomatter_nvm3.hex",  # provision.py -vi 0x1515 -sd 0x0041 -pi 0x4241 -sp 62034001 -sn 4268
        "3_nanomatter_nvm3.hex",  # provision.py -vi 0x1515 -sd 0x0050 -pi 0x4250 -sp 62034009 -sn 4277
        "4_nanomatter_nvm3.hex",  # provision.py -vi 0x1515 -sd 0x0051 -pi 0x4251 -sp 62034010 -sn 4278
        "5_nanomatter_nvm3.hex",  # provision.py -vi 0x1515 -sd 0x0052 -pi 0x4252 -sp 62034011 -sn 4279
        "6_nanomatter_nvm3.hex",  # provision.py -vi 0x1515 -sd 0x0053 -pi 0x4253 -sp 62034012 -sn 4280
        "7_nanomatter_nvm3.hex",  # provision.py -vi 0x1515 -sd 0x0054 -pi 0x4254 -sp 62034013 -sn 4281
        "8_nanomatter_nvm3.hex",  # provision.py -vi 0x1515 -sd 0x0055 -pi 0x4255 -sp 62034014 -sn 4282
        "9_nanomatter_nvm3.hex",  # provision.py -vi 0x1515 -sd 0x0056 -pi 0x4256 -sp 62034015 -sn 4283
        "10_nanomatter_nvm3.hex", # provision.py -vi 0x1515 -sd 0x0057 -pi 0x4257 -sp 62034016 -sn 4284
    ]
}

thingplusmatter_config = {
    'name': 'thingplus_matter',
    'flasher_tool': 'simplicitycommander',
    'configs': [
        "0_thingplusmatter_nvm3.hex", # default config (empty nvm3)
        "1_thingplusmatter_nvm3.hex", # provision.py -vi 0x1049 -sd 0x0042 -pi 0x4242 -sp 62034002 -sn 4269
        "2_thingplusmatter_nvm3.hex", # provision.py -vi 0x1049 -sd 0x0043 -pi 0x4243 -sp 62034003 -sn 4270
    ]
}

xg24explorerkit_config = {
    'name': 'xg24explorerkit',
    'flasher_tool': 'simplicitycommander',
    'configs': [
        "0_xg24explorerkit_nvm3.hex", # default config (empty nvm3)
        "1_xg24explorerkit_nvm3.hex", # provision.py -vi 0x1049 -sd 0x0044 -pi 0x4244 -sp 62034004 -sn 4271
        "2_xg24explorerkit_nvm3.hex", # provision.py -vi 0x1049 -sd 0x0045 -pi 0x4245 -sp 62034005 -sn 4272
    ]
}

xg24devkit_config = {
    'name': 'xg24devkit',
    'flasher_tool': 'simplicitycommander',
    'configs': [
        "0_xg24devkit_nvm3.hex", # default config (empty nvm3)
        "1_xg24devkit_nvm3.hex", # provision.py -vi 0x1049 -sd 0x0046 -pi 0x4246 -sp 62034006 -sn 4273
        "2_xg24devkit_nvm3.hex", # provision.py -vi 0x1049 -sd 0x0047 -pi 0x4247 -sp 62034007 -sn 4274
    ]
}

xiaomg24_config = {
    'name': 'xiao_mg24',
    'flasher_tool': 'openocd',
    'configs': [
        "0_xiao_mg24_nvm3.hex", # default config (empty nvm3)
        "1_xiao_mg24_nvm3.hex", # provision.py -vi 0x1049 -sd 0x0048 -pi 0x4248 -sp 62034008 -sn 4275
        "2_xiao_mg24_nvm3.hex", # provision.py -vi 0x1049 -sd 0x0049 -pi 0x4249 -sp 62034009 -sn 4276
    ]
}


supported_boards = [nanomatter_config,
                    thingplusmatter_config,
                    xg24explorerkit_config,
                    xg24devkit_config,
                    xiaomg24_config]


if __name__ == "__main__":
    main()
