import util.hil_util as hil_util
import random

def testcase_hil_eeprom(current_board, variant, current_board_port):
    """
    Testcase: HIL EEPROM
    Description: Tests the EEPROM library by writing and reading back a random payload
    """
    did_run = True
    success = hil_util.arduino_cli_build_and_flash(current_board, variant, "sketches/hil_eeprom/hil_eeprom.ino", current_board_port)
    if not success:
        print(f"Build/upload failed for '{variant}' on '{current_board}'")
        return did_run, False

    # Generate a random address and payload
    eeprom_address = random.randint(0, 10239)
    eeprom_payload = random.randint(0, 255)
    print(f"EEPROM address: '{eeprom_address}'")
    print(f"EEPROM payload: '{eeprom_payload}'")

    # Set the EEPROM address
    set_address_payload = f"a{eeprom_address}\n"
    set_address_response = f"EEPROM address set to {eeprom_address}"
    success = hil_util.check_serial_response(current_board_port, set_address_response, set_address_payload)
    if not success:
        return did_run, False

    # Write the EEPROM payload
    write_payload = f"w{eeprom_payload}\n"
    write_response = f"EEPROM write: {eeprom_payload}"
    success = hil_util.check_serial_response(current_board_port, write_response, write_payload)
    if not success:
        return did_run, False

    # Read the EEPROM payload
    read_payload = "r\n"
    read_response = f"EEPROM read: {eeprom_payload}"
    success = hil_util.check_serial_response(current_board_port, read_response, read_payload)
    if not success:
        return did_run, False

    # Reboot the board
    reboot_payload = "b\n"
    reboot_response = "Rebooting..."
    success = hil_util.check_serial_response(current_board_port, reboot_response, reboot_payload)
    if not success:
        return did_run, False

    # Set the EEPROM address
    set_address_payload = f"a{eeprom_address}\n"
    set_address_response = f"EEPROM address set to {eeprom_address}"
    success = hil_util.check_serial_response(current_board_port, set_address_response, set_address_payload)
    if not success:
        return did_run, False

    # Read the EEPROM payload again
    read_payload = "r\n"
    read_response = f"EEPROM read: {eeprom_payload}"
    success = hil_util.check_serial_response(current_board_port, read_response, read_payload)
    if not success:
        return did_run, False
    return did_run, True
