# Little helper script to build example sketches on all variants
# Helps us to avoid regressions
# This script assumes that you have 'arduino-cli' installed and the core being developed/tested is also installed

import sys
import subprocess
import time

all_variants = [
    ["nano_matter", "none"],
    ["nano_matter", "ble_arduino"],
    ["nano_matter", "ble_silabs"],
    ["nano_matter", "matter"],
    ["thingplusmatter", "none"],
    ["thingplusmatter", "ble_arduino"],
    ["thingplusmatter", "ble_silabs"],
    ["thingplusmatter", "matter"],
    ["xg24explorerkit", "none"],
    ["xg24explorerkit", "ble_arduino"],
    ["xg24explorerkit", "ble_silabs"],
    ["xg24explorerkit", "matter"],
    ["xg24devkit", "none"],
    ["xg24devkit", "ble_arduino"],
    ["xg24devkit", "ble_silabs"],
    ["xg24devkit", "matter"],
    ["xg27devkit", "none"],
    ["xg27devkit", "ble_arduino"],
    ["xg27devkit", "ble_silabs"],
    ["bgm220explorerkit", "none"],
    ["bgm220explorerkit", "ble_silabs"],
    ["lyra24p20", "none"],
    ["lyra24p20", "ble_arduino"],
    ["lyra24p20", "ble_silabs"],
    ["xiao_mg24", "none"],
    ["xiao_mg24", "ble_arduino"],
    ["xiao_mg24", "ble_silabs"],
    ["xiao_mg24", "matter"],
]

all_ble = [
    ["nano_matter", "ble_arduino"],
    ["nano_matter", "ble_silabs"],
    ["thingplusmatter", "ble_arduino"],
    ["thingplusmatter", "ble_silabs"],
    ["xg24explorerkit", "ble_arduino"],
    ["xg24explorerkit", "ble_silabs"],
    ["xg24devkit", "ble_arduino"],
    ["xg24devkit", "ble_silabs"],
    ["xg27devkit", "ble_arduino"],
    ["xg27devkit", "ble_silabs"],
    ["bgm220explorerkit", "ble_silabs"],
    ["lyra24p20", "ble_arduino"],
    ["lyra24p20", "ble_silabs"],
    ["xiao_mg24", "ble_arduino"],
    ["xiao_mg24", "ble_silabs"],
]

all_ble_silabs = [
    ["nano_matter", "ble_silabs"],
    ["thingplusmatter", "ble_silabs"],
    ["xg24explorerkit", "ble_silabs"],
    ["xg24devkit", "ble_silabs"],
    ["xg27devkit", "ble_silabs"],
    ["bgm220explorerkit", "ble_silabs"],
    ["lyra24p20", "ble_silabs"],
    ["xiao_mg24", "ble_silabs"],
]

thingplusmatter_ble_silabs = [
    ["thingplusmatter", "ble_silabs"],
]

xg27devkit_ble_silabs = [
    ["xg27devkit", "ble_silabs"],
]

xiao_mg24_ble_silabs = [
    ["xiao_mg24", "ble_silabs"],
]

boards_with_pdm = [
    ["xg27devkit", "ble_arduino"],
    ["xg27devkit", "ble_silabs"],
    ["xg27devkit", "none"],
]

ble_silabs_boards_with_imu = [
    ["xg24devkit", "ble_silabs"],
    ["xg27devkit", "ble_silabs"],
]

boards_with_dac = [
    ["nano_matter", "none"],
    ["nano_matter", "ble_arduino"],
    ["nano_matter", "ble_silabs"],
    ["nano_matter", "matter"],
    ["thingplusmatter", "none"],
    ["thingplusmatter", "ble_arduino"],
    ["thingplusmatter", "ble_silabs"],
    ["thingplusmatter", "matter"],
    ["xg24explorerkit", "none"],
    ["xg24explorerkit", "ble_arduino"],
    ["xg24explorerkit", "ble_silabs"],
    ["xg24explorerkit", "matter"],
    ["xg24devkit", "none"],
    ["xg24devkit", "ble_arduino"],
    ["xg24devkit", "ble_silabs"],
    ["xg24devkit", "matter"],
    ["lyra24p20", "none"],
    ["lyra24p20", "ble_arduino"],
    ["lyra24p20", "ble_silabs"],
]

all_matter = [
    ["nano_matter", "matter"],
    ["thingplusmatter", "matter"],
    ["xg24explorerkit", "matter"],
    ["xg24devkit", "matter"],
    ["xiao_mg24", "matter"],
]

all_ai_ml = [
   ["nano_matter", "none"],
   ["nano_matter", "ble_arduino"],
   ["nano_matter", "ble_silabs"],
   ["nano_matter", "matter"],
   ["xiao_mg24", "none"],
   ["xiao_mg24", "ble_arduino"],
   ["xiao_mg24", "ble_silabs"],
   ["xiao_mg24", "matter"],
   ["xg24explorerkit", "none"],
   ["xg24explorerkit", "ble_arduino"],
   ["xg24explorerkit", "ble_silabs"],
   ["xg24explorerkit", "matter"],
   ["xg24devkit", "none"],
   ["xg24devkit", "ble_arduino"],
   ["xg24devkit", "ble_silabs"],
   ["xg24devkit", "matter"],
]

nano_matter_matter = [
    ["nano_matter", "matter"],
]

testlist_quick = {
    "test_sketch/test_sketch.ino":                                                                                     all_variants,
}

testlist_common = {
    # Silicon Labs example library
    "../../libraries/SiliconLabs/examples/ble_blinky/ble_blinky.ino":                                                  all_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_health_thermometer/ble_health_thermometer.ino":                          all_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_health_thermometer_client/ble_health_thermometer_client.ino":            all_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_hid_keyboard/ble_hid_keyboard.ino":                                      all_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_hid_mouse/ble_hid_mouse.ino":                                            ble_silabs_boards_with_imu,
    "../../libraries/SiliconLabs/examples/ble_hid_mouse_xiao_mg24/ble_hid_mouse_xiao_mg24.ino":                        xiao_mg24_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_lightswitch_client/ble_lightswitch_client.ino":                          all_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_lightswitch_server/ble_lightswitch_server.ino":                          all_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_minimal/ble_minimal.ino":                                                all_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_scan/ble_scan.ino":                                                      all_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_spp/ble_spp.ino":                                                        all_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_thingplus_battery_gauge/ble_thingplus_battery_gauge.ino":                thingplusmatter_ble_silabs,
    "../../libraries/SiliconLabs/examples/ble_xg27_devkit_sensors/ble_xg27_devkit_sensors.ino":                        xg27devkit_ble_silabs,
    "../../libraries/SiliconLabs/examples/dac_sawtooth/dac_sawtooth.ino":                                              boards_with_dac,
    "../../libraries/SiliconLabs/examples/hwinfo/hwinfo.ino":                                                          all_variants,
    "../../libraries/SiliconLabs/examples/xg27devkit_sensors/xg27devkit_sensors.ino":                                  xg27devkit_ble_silabs,
    "../../libraries/SiliconLabs/examples/thingplusmatter_debug_unix/thingplusmatter_debug_unix.ino":                  all_ble_silabs,
    "../../libraries/SiliconLabs/examples/thingplusmatter_debug_win/thingplusmatter_debug_win.ino":                    all_ble_silabs,
    # ezBLE
    "../../libraries/ezBLE/examples/ezBLE_callbacks/ezBLE_callbacks.ino":                                              all_ble_silabs,
    "../../libraries/ezBLE/examples/ezBLE_send_and_receive/ezBLE_send_and_receive.ino":                                all_ble_silabs,
    "../../libraries/ezBLE/examples/ezBLE_simple_client/ezBLE_simple_client.ino":                                      all_ble_silabs,
    "../../libraries/ezBLE/examples/ezBLE_simple_client_callback/ezBLE_simple_client_callback.ino":                    all_ble_silabs,
    "../../libraries/ezBLE/examples/ezBLE_simple_server/ezBLE_simple_server.ino":                                      all_ble_silabs,
    # ezWS2812
    "../../libraries/ezWS2812/examples/blink_all/blink_all.ino":                                                       all_variants,
    "../../libraries/ezWS2812/examples/colors/colors.ino":                                                             all_variants,
    "../../libraries/ezWS2812/examples/individual_leds/individual_leds.ino":                                           all_variants,
    # OneWire
    "../../libraries/OneWire/examples/DS18x20_Temperature/DS18x20_Temperature.ino":                                    all_variants,
    "../../libraries/OneWire/examples/DS250x_PROM/DS250x_PROM.ino":                                                    all_variants,
    "../../libraries/OneWire/examples/DS2408_Switch/DS2408_Switch.ino":                                                all_variants,
    # Si7210Hall
    "../../libraries/Si7210_hall/examples/Si7210_hall_measure/Si7210_hall_measure.ino":                                all_variants,
    # SilabsMicrophoneAnalog
    "../../libraries/SilabsMicrophoneAnalog/examples/MicrophoneVolume/MicrophoneVolume.ino":                           all_variants,
    # SilabsMicrophonePDM
    "../../libraries/SilabsMicrophonePDM/examples/microphone_sound_level/microphone_sound_level.ino":                  boards_with_pdm,
    # ArduinoLowPower
    "../../libraries/ArduinoLowPower/examples/DeepSleepExternalOrTimedWakeup/DeepSleepExternalOrTimedWakeup.ino":      all_variants,
    "../../libraries/ArduinoLowPower/examples/DeepSleepExternalWakeup/DeepSleepExternalWakeup.ino":                    all_variants,
    "../../libraries/ArduinoLowPower/examples/DeepSleepMemory/DeepSleepMemory.ino":                                    all_variants,
    "../../libraries/ArduinoLowPower/examples/DeepSleepTimedWakeup/DeepSleepTimedWakeup.ino":                          all_variants,
    "../../libraries/ArduinoLowPower/examples/SleepTimedWakeup/SleepTimedWakeup.ino":                                  all_variants,
    # WatchdogTimer
    "../../libraries/WatchdogTimer/examples/watchdog_timer_interrupt/watchdog_timer_interrupt.ino":                    all_variants,
    "../../libraries/WatchdogTimer/examples/watchdog_timer_reset/watchdog_timer_reset.ino":                            all_variants,
    # SilabsTFLiteMicro
    "../../libraries/SilabsTFLiteMicro/examples/magic_wand_imu_capture/magic_wand_imu_capture.ino":                    all_ai_ml,
    "../../libraries/SilabsTFLiteMicro/examples/magic_wand/magic_wand.ino":                                            all_ai_ml,
}

testlist_matter = {
    "../../libraries/Matter/examples/matter_air_purifier/matter_air_purifier.ino":                                     all_matter,
    "../../libraries/Matter/examples/matter_air_quality_sensor/matter_air_quality_sensor.ino":                         all_matter,
    "../../libraries/Matter/examples/matter_ble_blinky/matter_ble_blinky.ino":                                         all_matter,
    "../../libraries/Matter/examples/matter_ble_minimal/matter_ble_minimal.ino":                                       all_matter,
    "../../libraries/Matter/examples/matter_contact_sensor/matter_contact_sensor.ino":                                 all_matter,
    "../../libraries/Matter/examples/matter_decommission/matter_decommission.ino":                                     all_matter,
    "../../libraries/Matter/examples/matter_door_lock/matter_door_lock.ino":                                           all_matter,
    "../../libraries/Matter/examples/matter_fan/matter_fan.ino":                                                       all_matter,
    "../../libraries/Matter/examples/matter_flow_sensor/matter_flow_sensor.ino":                                       all_matter,
    "../../libraries/Matter/examples/matter_humidity_sensor/matter_humidity_sensor.ino":                               all_matter,
    "../../libraries/Matter/examples/matter_illuminance_sensor/matter_illuminance_sensor.ino":                         all_matter,
    "../../libraries/Matter/examples/matter_lightbulb/matter_lightbulb.ino":                                           all_matter,
    "../../libraries/Matter/examples/matter_lightbulb_callback/matter_lightbulb_callback.ino":                         all_matter,
    "../../libraries/Matter/examples/matter_lightbulb_color/matter_lightbulb_color.ino":                               all_matter,
    "../../libraries/Matter/examples/matter_lightbulb_custom_name/matter_lightbulb_custom_name.ino":                   all_matter,
    "../../libraries/Matter/examples/matter_lightbulb_dimmable/matter_lightbulb_dimmable.ino":                         all_matter,
    "../../libraries/Matter/examples/matter_lightbulb_dimmable_multiple/matter_lightbulb_dimmable_multiple.ino":       all_matter,
    "../../libraries/Matter/examples/matter_lightbulb_identify/matter_lightbulb_identify.ino":                         all_matter,
    "../../libraries/Matter/examples/matter_lightbulb_multiple/matter_lightbulb_multiple.ino":                         all_matter,
    "../../libraries/Matter/examples/matter_lightbulb_multiple_color/matter_lightbulb_multiple_color.ino":             all_matter,
    "../../libraries/Matter/examples/matter_lightbulb_with_button/matter_lightbulb_with_button.ino":                   all_matter,
    "../../libraries/Matter/examples/matter_occupancy_sensor/matter_occupancy_sensor.ino":                             all_matter,
    "../../libraries/Matter/examples/matter_on_off_outlet/matter_on_off_outlet.ino":                                   all_matter,
    "../../libraries/Matter/examples/matter_pressure_sensor/matter_pressure_sensor.ino":                               all_matter,
    "../../libraries/Matter/examples/matter_sensor_and_bulb/matter_sensor_and_bulb.ino":                               all_matter,
    "../../libraries/Matter/examples/matter_sensor_multiple/matter_sensor_multiple.ino":                               all_matter,
    "../../libraries/Matter/examples/matter_switch/matter_switch.ino":                                                 all_matter,
    "../../libraries/Matter/examples/matter_temp_sensor/matter_temp_sensor.ino":                                       all_matter,
    "../../libraries/Matter/examples/matter_thermostat/matter_thermostat.ino":                                         all_matter,
    "../../libraries/Matter/examples/matter_window_covering/matter_window_covering.ino":                               all_matter,
    "../../libraries/Matter/examples/nano_matter_lightbulb_color/nano_matter_lightbulb_color.ino":                     nano_matter_matter,
}


def main():
    print("Silabs Arduino Core build test")

    test_config = get_config_from_arguments()
    testlist = testlist_quick
    if test_config == "quick":
        testlist = testlist_quick
    elif test_config == "common":
        testlist = testlist_common
    elif test_config == "matter":
        testlist = testlist_matter
    else:
        testlist = dict()
        testlist.update(testlist_quick)
        testlist.update(testlist_common)
        testlist.update(testlist_matter)

    testcase_count = count_testcases(testlist)
    start_time = time.time()
    total_builds = 0
    successful_builds = 0
    failed_build_names = []
    builds_with_warnings = []
    build_in_progress = False
    test_interrupted = False

    try:
        # Go through each testcase on the testlist
        for sketch_to_test in testlist:
            # Determine the variants to be used in the current test
            test_variants = testlist[sketch_to_test]

            # Build the sketch for the specified variants
            for variant in test_variants:
                build_in_progress = True
                total_builds += 1
                board = variant[0]
                protocol_stack = variant[1]
                success, warnings = arduino_cli_build(board, protocol_stack, sketch_to_test, total_builds, testcase_count)
                if success:
                    successful_builds += 1
                else:
                    failed_build_names.append(sketch_to_test + " on '" + board + "' with '" + protocol_stack + "'")
                if warnings:
                    builds_with_warnings.append(sketch_to_test + " on '" + board + "' with '" + protocol_stack + "'")
                build_in_progress = False
    except KeyboardInterrupt:
        test_interrupted = True
        print("\nTest execution interrupted by user!")
        if build_in_progress:
            total_builds -= 1
            print("Last build was not completed, removing it from the total count")

    # Calculate and display results
    failed_builds = total_builds - successful_builds
    print("-"*30)
    print(f"Test finished. {successful_builds} out of {total_builds} builds were successful!")
    if test_interrupted:
        print(f"{testcase_count-total_builds} builds have been skipped due to interruption")
    test_run_time = int(time.time() - start_time)
    print(f"Total time: {test_run_time // 60}m {test_run_time % 60}s")
    print()

    # If we have builds with warnings give more info on them
    if len(builds_with_warnings) != 0:
        print(f"{len(builds_with_warnings)} build(s) compiled with warnings!")
        print("Builds with warnings: ")
        for warningbuild in builds_with_warnings:
            print(warningbuild)
        print()
    # If we have failures give more info on them
    if failed_builds != 0:
        print(f"{failed_builds} build(s) failed!")
        print("Failing builds: ")
        for failedvar in failed_build_names:
            print(failedvar)
    # If we had errors or warnings exit with a different code
    if failed_builds != 0 or len(builds_with_warnings) != 0:
        exit(200)


def arduino_cli_build(board, protocol_stack, sketch_path, build_num, testcase_count):
    """
    Builds the specified sketch for the specified variant with Arduino CLI
    """
    fqbn = "SiliconLabs:silabs:" + board + ":protocol_stack=" + protocol_stack
    progress_percent = int(build_num * 100 / testcase_count)
    print("-"*40)
    print(f"[{build_num}/{testcase_count}] ({progress_percent}%) Building '{sketch_path}' for '{fqbn}'")
    print("-"*40)
    build_start_time = time.time()
    build_process = subprocess.Popen(
        ["arduino-cli", "compile", sketch_path, "-b", fqbn, "--warnings", "all"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    stdout, stderr = build_process.communicate(timeout=300)
    print(stderr.decode("utf-8"))
    print(stdout.decode("utf-8"))

    build_has_warnings = False
    if "warning:" in stderr.decode("utf-8"):
        build_has_warnings = True

    build_end_time = time.time()
    total_build_time = int(build_end_time - build_start_time)
    if build_process.returncode == 0:
        if build_has_warnings:
            print("Build successful with warnings!")
        else:
            print("Build successful!")
        print(f"Build time: {total_build_time // 60}m {total_build_time % 60}s")
        return True, build_has_warnings
    print("Build failed!")
    return False, build_has_warnings


def count_testcases(testlist):
    """
    Counts the number of total testcases in a test list
    """
    testcase_count = 0
    for testpath,variant in testlist.items():
        testcase_count += len(variant)
    return testcase_count


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
    elif input_config_name == "common":
        print("Running common tests only")
        return "common"
    elif input_config_name == "matter":
        print("Running Matter tests only")
        return "matter"
    return "all"


if __name__ == "__main__":
    main()
