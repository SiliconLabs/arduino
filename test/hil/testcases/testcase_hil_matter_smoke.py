import util.hil_util as hil_util

def testcase_hil_matter_smoke(current_board, variant, current_board_port):
    """
    Testcase: HIL Matter Smoke
    Description: Builds and uploads the HIL Matter Smoke sketch to all supported boards and checks for a serial response
    """
    did_run = False
    if variant != "matter":
        return did_run, True
    else:
        did_run = True

    success = hil_util.arduino_cli_build_and_flash(current_board, variant, "sketches/hil_matter_smoke/hil_matter_smoke.ino", current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False

    success = hil_util.check_serial_response(current_board_port, "Matter device is not commissioned")
    if not success:
        print(f"Serial response check failed for '{variant}' on '{current_board}'")
        return did_run, False

    success = hil_util.check_serial_response(current_board_port, "QR code URL: https://project-chip.github.io/connectedhomeip/qrcode.html?data=")
    if not success:
        print(f"Serial response check failed for '{variant}' on '{current_board}'")
        return did_run, False

    success = hil_util.check_serial_response(current_board_port, "Matter smoke test running! uptime:")
    if not success:
        print(f"Serial response check failed for '{variant}' on '{current_board}'")
        return did_run, False

    return did_run, True
