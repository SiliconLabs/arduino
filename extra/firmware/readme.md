# Extra firmware for Silicon Labs Arduino boards

This folder contains extra firmware for select Silicon Labs Arduino devices. There are two main categories of available firmware:
 - OpenThread RCP
 - Zigbee EZSP

## OpenThread RCP

This firmware will turn your device into an USB OpenThread RCP dongle which can be used with a host computer (usually a Raspberry Pi) for creating your own OpenThread Border Router.

## Zigbee EZSP

This firmware will turn your device into a USB Zigbee EZSP (EmberZNet Serial Protocol) dongle which can be used with a host computer (usually a Raspberry Pi) for creating your own Zigbee network. The firmware is compatible with Home Assistant and the Zigbee Home Automation (ZHA) integration and can act as your Zigbee Coordinator.

For setting up the ZHA integration in Home Assistant see: https://www.home-assistant.io/integrations/zha/

## Flasher script

An automatic flasher script is provided as `extra_firmware_flasher.py` which can flash your device with the selected firmware.

Prerequisites of running:
 - `python3` installed
 - `Silicon Labs Arduino Core` installed in the Arduino IDE

You can run the script in the following way:

`python3 extra_firmware_flasher.py <board_name> <firmware_type>`

Examples:

 - `python3 extra_firmware_flasher.py nano_matter thread`
    - This will flash the OpenThread RCP firmware on your Arduino Nano Matter device.

 - `python3 extra_firmware_flasher.py thingplus_matter zigbee`
    - This will flash the Zigbee EZSP firmware on your SparkFun Thing Plus Matter device.

You can also run the script without any arguments to print a help message with the supported boards and firmware types.
