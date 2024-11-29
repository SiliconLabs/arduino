import util.hil_util as hil_util

def testcase_hil_imu_spi(current_board, variant, current_board_port):
    """
    Testcase: HIL IMU SPI
    Description: Tests the SPI library by utilizing the xG24/xG27 Dev Kit's onboard IMU to get the WHOAMI register value
    """
    did_run = False
    if current_board != "xg27devkit" and current_board != "xg24devkit":
        return did_run, True
    else:
        did_run = True

    success = hil_util.arduino_cli_build_and_flash(current_board, variant, "sketches/hil_imu_spi/hil_imu_spi.ino", current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False
    success = hil_util.check_serial_response(current_board_port, "IMU WHOAMI: 0x98")
    if not success:
        print(f"Serial response check failed for '{variant}' on '{current_board}'")
        return did_run, False
    return did_run, True
