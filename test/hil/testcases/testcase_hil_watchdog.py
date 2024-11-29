import util.hil_util as hil_util

def testcase_hil_watchdog(current_board, variant, current_board_port):
    """
    Testcase: HIL Watchdog
    Description: Tests the Watchdog timer by triggering a watchdog reset
    """
    did_run = True
    success = hil_util.arduino_cli_build_and_flash(current_board, variant, "sketches/hil_watchdog/hil_watchdog.ino", current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False

    serial_response = hil_util.get_serial_response(current_board_port, timeout=4)
    if serial_response is None:
        return did_run, False

    success = True
    success &= ("Silicon Labs Watchdog timer test" in serial_response)
    success &= ("Watchdog reset imminent at" in serial_response)
    success &= ("Device restarted after a watchdog reset" in serial_response)
    if not success:
        print(f"Serial response check failed for '{variant}' on '{current_board}'")
    else:
        print("Watchdog test successful!")
    return did_run, success
