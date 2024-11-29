import util.hil_util as hil_util

def testcase_hil_si7021_wire(current_board, variant, current_board_port):
    """
    Testcase: HIL Si7021 Wire
    Description: Tests the Wire library by utilizing the xG24/xG27 Dev Kit's onboard Si7021 to get the temperature and humidity
    """
    did_run = False
    if current_board != "xg27devkit" and current_board != "xg24devkit":
        return did_run, True
    else:
        did_run = True

    success = hil_util.arduino_cli_build_and_flash(current_board, variant, "sketches/hil_si7021_wire/hil_si7021_wire.ino", current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False
    serial_response = hil_util.get_serial_response(current_board_port)
    if serial_response is None:
        return did_run, False
    for line in serial_response.split("\n"):
        try:
            temperature = float(line.split(" ")[0])
            humidity = float(line.split(" ")[1])
            if temperature > 0 and temperature < 100 and humidity > 0 and humidity < 100:
                print(f"Temperature: {temperature} C")
                print(f"Humidity: {humidity} %")
                print("Successfully got a temperature and humidity reading!")
                return did_run, True
        except Exception:
            pass
    print("Failed to get a valid temperature/humidity reading!")
    return did_run, False
