import util.hil_util as hil_util

def testcase_hil_thingplus_battery(current_board, variant, current_board_port):
    """
    Testcase: HIL ThingPlus Battery
    Description: Tests the Wire library by utilizing the ThingPlus's onboard MAX1704x fuel gauge to get voltage readings
    """
    did_run = False
    if current_board != "thingplusmatter":
        return did_run, True
    else:
        did_run = True

    success = hil_util.arduino_cli_build_and_flash(current_board, variant, "sketches/hil_thingplus_battery/hil_thingplus_battery.ino", current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False
    serial_response = hil_util.get_serial_response(current_board_port)
    if serial_response is None:
        return did_run, False
    for line in serial_response.split("\n"):
        try:
            voltage = float(line)
            if voltage > 2.0 and voltage < 5.5:
                print(f"Voltage: {voltage} V")
                print("Successfully got a voltage reading!")
                return did_run, True
        except Exception:
            pass
    print("Failed to get a valid voltage reading!")
    return did_run, False
