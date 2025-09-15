# Arduino Matter Provision Tool

The purpose of this script to flash unique provisioning data to Arduino Matter devices.

The provisioning data is pre-calculated and unique for each device.
Selecting config #0 will return the device to it's original (test) provisioning data.

If you'd like to customize provisioning data instead of using pre-generated configurations - take a look at Silicon Labs' Matter [Provision tool](https://github.com/SiliconLabs/matter_extension/tree/v2.5.1/provision).

## Usage

`python arduino_matter_provision.py <board_name> <config_number>`

Example:

`python arduino_matter_provision.py nano_matter 1`

## Provisioning process

- Make sure the *Silicon Labs Arduino Core* and the *Arduino IDE* are both installed
- Make sure there's only one board connected to your computer at a time
- Click *'Tools > Burn Bootloader'* in the Arduino IDE (this'll erase the chip and burn the bootloader)
- Flash your desired Matter sketch
- Run the script to change the provisioning data
- Provision the device to your Matter controller

## Supported boards

 - Arduino Nano Matter (`nano_matter`)
 - SparkFun Thing Plus Matter MGM240P (`thingplus_matter`)
 - Silicon Labs xG24 Explorer Kit (`xg24explorerkit`)
 - Silicon Labs xG24 Dev Kit (`xg24devkit`)
 - Seeed Studio XIAO MG24 (Sense) (`xiao_mg24`)
