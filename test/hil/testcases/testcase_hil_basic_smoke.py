import util.hil_util as hil_util

def testcase_hil_basic_smoke(current_board, variant, current_board_port):
    """
    Testcase: HIL Basic Smoke
    Description: Builds and uploads the HIL Basic Smoke sketch to all supported boards and checks for a serial response
    """
    did_run = True
    success = hil_util.arduino_cli_build_and_flash(current_board, variant, "sketches/hil_basic_smoke/hil_basic_smoke.ino", current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False
    success = hil_util.check_serial_response(current_board_port, "Hello, Silabs Arduino!")
    if not success:
        print(f"Serial response check failed for '{variant}' on '{current_board}'")
        return did_run, False
    return did_run, True
