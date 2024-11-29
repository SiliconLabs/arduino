import util.hil_util as hil_util

def testcase_hil_ble_silabs_advertise(current_board, variant, current_board_port):
    """
    Testcase: HIL BLE Silabs advertise
    Description: Builds and uploads a sketch which advertises on BLE then checks for the BLE advertisement
    """
    did_run = False
    if variant != "ble_silabs":
        return did_run, True
    else:
        did_run = True

    success = hil_util.arduino_cli_build_and_flash(current_board,
                                                   variant,
                                                   "sketches/hil_ble_silabs_advertise/hil_ble_silabs_advertise.ino",
                                                   current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False

    success = hil_util.check_for_ble_device_advertisement("Silabs BLE HIL")
    if not success:
        print(f"BLE advertisement check failed for '{variant}' on '{current_board}'")
        return did_run, False

    return did_run, True
