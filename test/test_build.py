# Little helper script to build a minimal sketch on all variants
# Helps us to avoid regressions
# This script assumes that you have 'arduino-cli' installed and the core being developed/tested is also installed

import sys
import subprocess
import time

all_variants = [
    "thingplusmatter_ble",
    "thingplusmatter_ble_precomp",
    "thingplusmatter_matter",
    "thingplusmatter_matter_precomp",
    "xg27devkit",
    "xg27devkit_precomp",
    "xg24explorerkit",
    "xg24explorerkit_precomp",
    "xg24explorerkit_matter",
    "xg24explorerkit_matter_precomp",
    #"wio_mg24_ble",
    #"wio_mg24_ble_precomp",
    "bgm220explorerkit",
    "bgm220explorerkit_precomp",
    #"nano_matter_ble",
    #"nano_matter_ble_precomp",
    #"nano_matter_matter",
    #"nano_matter_matter_precomp"
]

all_ble = [
    "thingplusmatter_ble",
    "thingplusmatter_ble_precomp",
    "xg27devkit",
    "xg27devkit_precomp",
    "xg24explorerkit",
    "xg24explorerkit_precomp",
    #"wio_mg24_ble",
    #"wio_mg24_ble_precomp",
    "bgm220explorerkit",
    "bgm220explorerkit_precomp",
    #"nano_matter_ble",
    #"nano_matter_ble_precomp"
]

ble_with_button = [
    "xg27devkit",
    "xg27devkit_precomp",
    "xg24explorerkit",
    "xg24explorerkit_precomp",
    #"wio_mg24_ble",
    #"wio_mg24_ble_precomp",
    "bgm220explorerkit",
    "bgm220explorerkit_precomp",
    #"nano_matter_ble",
    #"nano_matter_ble_precomp"
]

all_matter = [
    "thingplusmatter_matter",
    "thingplusmatter_matter_precomp",
    "xg24explorerkit_matter",
    "xg24explorerkit_matter_precomp",
    #"nano_matter_matter",
    #"nano_matter_matter_precomp"
]

testlist_quick = {
    "test_sketch/test_sketch.ino":                                                                                  "all_variants",
}

testlist_all = {
    "test_sketch/test_sketch.ino":                                                                                  "all_variants",
    "../libraries/SiliconLabs/examples/ble_blinky/ble_blinky.ino":                                                  "all_ble",
    "../libraries/SiliconLabs/examples/ble_health_thermometer/ble_health_thermometer.ino":                          "all_ble",
    "../libraries/SiliconLabs/examples/ble_health_thermometer_client/ble_health_thermometer_client.ino":            "all_ble",
    "../libraries/SiliconLabs/examples/ble_lightswitch_client/ble_lightswitch_client.ino":                          "ble_with_button",
    "../libraries/SiliconLabs/examples/ble_lightswitch_server/ble_lightswitch_server.ino":                          "all_ble",
    "../libraries/SiliconLabs/examples/ble_minimal/ble_minimal.ino":                                                "all_ble",
    "../libraries/SiliconLabs/examples/thingplusmatter_debug_unix/thingplusmatter_debug_unix.ino":                  "all_ble",
    "../libraries/SiliconLabs/examples/thingplusmatter_debug_win/thingplusmatter_debug_win.ino":                    "all_ble",
    "../libraries/ezBLE/examples/ezBLE_callbacks/ezBLE_callbacks.ino":                                              "all_ble",
    "../libraries/ezBLE/examples/ezBLE_send_and_receive/ezBLE_send_and_receive.ino":                                "all_ble",
    "../libraries/ezBLE/examples/ezBLE_simple_client/ezBLE_simple_client.ino":                                      "all_ble",
    "../libraries/ezBLE/examples/ezBLE_simple_client_callback/ezBLE_simple_client_callback.ino":                    "all_ble",
    "../libraries/ezBLE/examples/ezBLE_simple_server/ezBLE_simple_server.ino":                                      "all_ble",
    "../libraries/Matter/examples/matter_flow_sensor/matter_flow_sensor.ino":                                       "all_matter",
    "../libraries/Matter/examples/matter_humidity_sensor/matter_humidity_sensor.ino":                               "all_matter",
    "../libraries/Matter/examples/matter_illuminance_sensor/matter_illuminance_sensor.ino":                         "all_matter",
    "../libraries/Matter/examples/matter_lightbulb/matter_lightbulb.ino":                                           "all_matter",
    "../libraries/Matter/examples/matter_lightbulb_color/matter_lightbulb_color.ino":                               "all_matter",
    "../libraries/Matter/examples/matter_lightbulb_dimmable/matter_lightbulb_dimmable.ino":                         "all_matter",
    "../libraries/Matter/examples/matter_lightbulb_dimmable_multiple/matter_lightbulb_dimmable_multiple.ino":       "all_matter",
    "../libraries/Matter/examples/matter_lightbulb_multiple/matter_lightbulb_multiple.ino":                         "all_matter",
    "../libraries/Matter/examples/matter_lightbulb_multiple_color/matter_lightbulb_multiple_color.ino":             "all_matter",
    "../libraries/Matter/examples/matter_occupancy_sensor/matter_occupancy_sensor.ino":                             "all_matter",
    "../libraries/Matter/examples/matter_pressure_sensor/matter_pressure_sensor.ino":                               "all_matter",
    "../libraries/Matter/examples/matter_sensor_and_bulb/matter_sensor_and_bulb.ino":                               "all_matter",
    "../libraries/Matter/examples/matter_sensor_multiple/matter_sensor_multiple.ino":                               "all_matter",
    "../libraries/Matter/examples/matter_temp_sensor/matter_temp_sensor.ino":                                       "all_matter",
    "../libraries/Matter/examples/matter_thermostat/matter_thermostat.ino":                                         "all_matter",
    "../libraries/Matter/examples/matter_fan/matter_fan.ino":                                                       "all_matter",
}


def main():
    print("Silabs Arduino Core build test")

    test_config = get_config_from_arguments()
    testlist = testlist_quick
    if test_config == "quick":
        testlist = testlist_quick
    else:
        testlist = testlist_all

    start_time = time.time()
    total_builds = 0
    successful_builds = 0
    failed_build_names = []
    # Go through each testcase on the testlist
    for sketch_to_test in testlist:
        # Determine the variants to be used in the current test
        test_variants = []
        if testlist[sketch_to_test] == "all_ble":
            test_variants = all_ble
        if testlist[sketch_to_test] == "all_matter":
            test_variants = all_matter
        if testlist[sketch_to_test] == "all_variants":
            test_variants = all_variants

        # Build the sketch for the specified variants
        for variant in test_variants:
            total_builds += 1
            if arduino_cli_build(variant, sketch_to_test, total_builds):
                successful_builds += 1
            else:
                failed_build_names.append(sketch_to_test + " on " + variant)

    # Calculate and display results
    failed_builds = total_builds - successful_builds
    print("-"*30)
    print(f"Test finished. {successful_builds} out of {total_builds} builds were successful!")
    test_run_time = int(time.time() - start_time)
    print(f"Total time: {test_run_time // 60}m {test_run_time % 60}s")
    # If we have failures give more info on them
    if failed_builds != 0:
        print(f"{failed_builds} build(s) failed!")
        print("Failing builds: ")
        for failedvar in failed_build_names:
            print(failedvar)


def arduino_cli_build(variant, sketch_path, build_num):
    fqbn = "SiliconLabs:silabs:" + variant
    print("-"*40)
    print(f"[{build_num:02d}] Building '{sketch_path}' for '{fqbn}'")
    print("-"*40)
    build_start_time = time.time()
    build_process = subprocess.Popen(
        ["arduino-cli", "compile", sketch_path, "-b", fqbn]
    )
    build_process.communicate(timeout=300)
    build_end_time = time.time()
    total_build_time = int(build_end_time - build_start_time)
    if build_process.returncode == 0:
        print("Build successful!")
        print(f"Build time: {total_build_time // 60}m {total_build_time % 60}s")
        return True
    print("Build failed!")
    return False


def get_config_from_arguments():
    """
    Determines the test configuration from the command line arguments
    """
    try:
        input_config_name = sys.argv[1]
    except Exception:
        print("No configuration argument provided - running all available tests")
        return "all"

    if input_config_name == "quick":
        print("Running quick tests only")
        return "quick"
    return "all"


if __name__ == "__main__":
    main()
