import util.hil_util as hil_util

def testcase_hil_adc(current_board, variant, current_board_port):
    """
    Testcase: HIL ADC
    Description: Tests the ADC/Analog driver by getting readings from an analog pin
    """
    did_run = True

    success = hil_util.arduino_cli_build_and_flash(current_board, variant, "sketches/hil_adc/hil_adc.ino", current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False
    serial_response = hil_util.get_serial_response(current_board_port)
    if serial_response is None:
        return did_run, False
    for line in serial_response.split("\n"):
        try:
            reading = int(line)
            if reading > 0 and reading < 4096:
                print(f"ADC reading: {reading}")
                print("Successfully got a valid ADC reading!")
                return did_run, True
        except Exception:
            pass
    print("Failed to get a valid ADC reading!")
    return did_run, False
