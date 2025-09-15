import util.hil_util as hil_util
import random
import string

def testcase_hil_serial_echo(current_board, variant, current_board_port):
    """
    Testcase: HIL Serial Echo
    Description: Builds and uploads the HIL Serial Echo sketch to all supported boards and checks for a serial response
    """
    did_run = True
    success = hil_util.arduino_cli_build_and_flash(current_board, variant, "sketches/hil_serial_echo/hil_serial_echo.ino", current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False

    # Generate a random payload
    serial_payload = ''.join(random.choices(string.ascii_lowercase + string.digits, k=20))
    print(f"Serial payload: '{serial_payload}'")

    success = hil_util.check_serial_response(current_board_port, serial_payload, serial_payload)
    if not success:
        print(f"Serial response check failed for '{variant}' on '{current_board}'")
        return did_run, False
    return did_run, True
