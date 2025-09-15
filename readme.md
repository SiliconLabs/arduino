# Silicon Labs Arduino Core

![](doc/silabs_logo.png)

This project enables **Silicon Labs** hardware to be used with the **Arduino** ecosystem.

[![Latest release](https://img.shields.io/github/release/SiliconLabs/arduino)](https://github.com/SiliconLabs/arduino/releases/latest/)
[![Latest release date](https://img.shields.io/github/release-date/SiliconLabs/arduino)](https://github.com/SiliconLabs/arduino/releases/latest/)

#### Currently supported hardware:
 - [Arduino Nano Matter](https://store.arduino.cc/pages/nano-matter) ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)
 - [SparkFun Thing Plus Matter MGM240P](https://www.sparkfun.com/products/20270) ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)
 - [Silicon Labs xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit) ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)
 - [Silicon Labs xG24 Dev Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit) ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)
 - [Silicon Labs xG27 Dev Kit](https://www.silabs.com/development-tools/wireless/efr32xg27-development-kit) ![BLE](doc/bluetooth_logo_icon.png)
 - [Silicon Labs BGM220 Explorer Kit](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit) ![BLE](doc/bluetooth_logo_icon.png)
 - [Ezurio Lyra 24P 20dBm Dev Kit](https://www.ezurio.com/part/453-00145-k1) ![BLE](doc/bluetooth_logo_icon.png)
 - [Seeed Studio XIAO MG24 (Sense)](https://www.seeedstudio.com/Seeed-Studio-XIAO-MG24-p-6247.html) ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)

![Arduino Nano Matter](doc/nanomatter.png)
![SparkFun Thing Plus Matter MGM240P](doc/thingplusmatter.png)
![xG24 Explorer Kit](doc/xg24explorerkit.png)
![xG24 Dev Kit](doc/xg24devkit.png)
![xG27 Dev Kit](doc/xg27devkit.png)
![BGM220 Explorer Kit](doc/bgm220explorerkit.png)
![Ezurio Lyra 24P 20dBm Dev Kit](doc/lyra24p20.png)
![Seeed Studio XIAO MG24 (Sense)](doc/xiao_mg24.png)

## Installation

 - Download and install the latest [Arduino IDE](https://www.arduino.cc/en/software) (only the newer 2.x versions are supported - the legacy 1.x versions are not)
 - Open the *'Settings / Preferences'* window
 - Open the *'Boards Manager'* from the side menu and search for *'Silicon Labs'*
   - If it does not show up - enter `https://siliconlabs.github.io/arduino/package_arduinosilabs_index.json` in the *'Additional Boards Manager URLs'* field (add a comma in between if you have multiple URLs)
 - Install the `Silicon Labs` platform

## Getting started

If you're using your board for the first time with Arduino IDE you need to burn the bootloader first.
Burning the bootloader fully erases the flash memory first, then burns the bootloader. You only need to do this once during the initial setup or if you are changing Matter device types and/or want to move your device to a different Matter network.
 - Select the board you have from the *'Tools > Board'* menu
 - Go to the *'Tools > Programmer'* menu and select *'Simplicity Commander'* or *'openOCD'* for the Nano Matter
 - After that select the *'Tools > Burn bootloader'* option
 - If you have trouble with flashing you may need to install the *J-Link USB drivers* - it's part of the [J-Link Software and Documentation pack](https://www.segger.com/downloads/jlink/)
 - Verify that everything works by creating a *Blink* example (*File > Examples > 01.Basics > Blink*) and flashing it to your board
 - If you see the built-in LED blinking, you're ready to go! Happy coding! 💻

## Radio protocol variants
Each board supports a number of different radio protocol stacks (like *'Matter'*, *'BLE (Arduino)'*, *'BLE (Silabs)'* and *'None'*) which can be selected under *'Tools > Protocol stack'* in the Arduino IDE.

 - Selecting `Matter` includes the Matter stack - which is quite resource heavy. Selecting this is only recommended when developing Matter applications. BLE is included, but cannot be used by users as the Matter SDK takes ownership of it. Matter examples will only work with this option.
 - Selecting `BLE (Arduino)` provides compatibility with the [ArduinoBLE](https://github.com/arduino-libraries/ArduinoBLE) library and is moderately resource heavy. This variant is compatible with all ArduinoBLE examples and applications based on the library.
 - Selecting `BLE (Silabs)` includes Silicon Labs' BLE stack and API ([BGAPI](https://docs.silabs.com/bluetooth/latest/bluetooth-stack-api/)) - which is moderately resource heavy. Select this if you're developing BLE applications based on BGAPI. The included Silicon Labs BLE examples will only work with this option.
 - Selecting `None` does not include a radio protocol stack in your sketch - and saves a considerable amount of Flash/RAM. You can use your board as a regular Arduino without wireless hardware.

You can see the list/icon of available radio protocols for each board under [*currently supported hardware*](#currently-supported-hardware) or by going to this menu.

## Matter library
![Matter](doc/matter_logo.png)

The core comes bundled with the `Matter` library. The library's goal is to make creating and managing Matter devices easy and user-friendly.

The library sits on top of Silicon Labs' Matter SDK which is included in the *Matter* protocol stack variant of the supported boards. Make sure to select *Matter* in the *'Tools > Protocol stack'* menu to have the Matter SDK / radio stack included in your sketch.

See the docs for the Matter library [here](libraries/Matter/readme.md).

## Libraries

### Included with the core:

 - **ArduinoLowPower 🔋** - for accessing the low power features of the devices [[docs](libraries/ArduinoLowPower/README.md)]
 - **EEPROM 💾** - permanent storage in flash [[docs](libraries/EEPROM/README.md)]
 - **ezBLE 🛜** - send and receive data over BLE in a simple and user-friendly way on '*BLE (Silabs)*' variants [[docs](libraries/ezBLE/readme.md)]
 - **ezWS2812 💡** - driver for WS2812 LEDs using the hardware SPI or GPIO
 - **Matter** ![Matter](doc/matter_logo_icon.png) - [[docs](libraries/Matter/readme.md)]
 - **OneWire 🧵** - driver for the OneWire protocol for Silicon Labs devices
 - **Si7210_hall** - driver for Si7210 hall sensors
 - **SilabsMicrophoneAnalog 🎙️** - driver for analog microphones [[docs](libraries/SilabsMicrophonerAnalog/readme.md)]
 - **SilabsMicrophonePDM 🎤** - driver for PDM microphones
 - **SilabsTFLiteMicro 🤖** - TensorFlow Lite for Microcontrollers AI/ML library [[docs](libraries/SilabsTFLiteMicro/readme.md)]
 - **SiliconLabs** - various example sketches for Silicon Labs devices
 - **SPI** - the standard Arduino SPI library
 - **WatchdogTimer 🐶** - for keeping an eye on correct behavior - [[docs](libraries/WatchdogTimer/readme.md)]
 - **Wire** - the standard Arduino Wire library

### Separately supplied:

 - **ArduinoBLE ![BLE](doc/bluetooth_logo_icon.png)** - Bluetooth Low Energy library for the '*BLE (Arduino)*' variants [[docs](https://github.com/arduino-libraries/ArduinoBLE)]

## Additional APIs
There are some additional functions besides the standard Arduino API you can call on Silicon Labs boards:
 - `getCPUTemp()` - returns the die temperature in Celsius
 - `systemReset()` - issues a system reset and restarts the MCU
 - `getDeviceUniqueId()` - returns the unique ID of the microcontroller
 - `getDeviceUniqueIdStr()` - returns the unique ID of the microcontroller in hexadecimal as a string
 - `getCoreVersion()` - returns the current core version as a string
 - `setCPUClock()` - sets the CPU clock speed - it can be one of `CPU_39MHZ`, `CPU_76MHZ`, `CPU_78MHZ`, `CPU_80MHZ`
 - `getCPUClock()` - returns the current CPU speed in hertz
 - `getCPUCycleCount()` - returns the current CPU cycle counter value - overflows often - useful for precision timing
 - `analogReferenceDAC()` - selects the voltage reference for the DAC hardware
 - `getCurrentBoardType()` - returns the current hardware platform (board) the sketch is running on
 - `getCurrentRadioStackType()` - returns the type of the radio stack the sketch was compiled with
 - `isBoardAiMlCapable()` - returns whether the board with the currently selected protocol stack is AI/ML capable
 - `getTotalHeapSize()` - returns the total heap (dynamic memory) size in bytes
 - `getFreeHeapSize()` - returns the current free heap size in bytes
 - `getUsedHeapSize()` - returns the current used heap size in bytes
 - `getHeapHighWatermark()` - returns the highest recorded heap usage in bytes
 - `resetHeapHighWatermark()` - resets the highest recorded heap usage


## Debugging with J-Link on Silicon Labs boards
All Silicon Labs boards (except for the Nano Matter) come equipped with an onboard *SEGGER J-Link debugger* which can be used from the Arduino IDE. In order to utilize this feature you'll need the following:
 - The [J-Link Software and Documentation pack](https://www.segger.com/downloads/jlink/) installed on your system
 - A debugger configuration file next to your sketch

If you'd like to debug on the Nano Matter see [*this*](#debugging-on-openocd-compatible-boards) chapter.

### Debugger configuration file
Go to the folder where your sketch is located and create a file named `debug_custom.json` - and add the following content to it:
```json
[
    {
        "configId": "SiliconLabs:silabs:thingplusmatter:programmer=commander",
        "servertype": "jlink",
        "device": "MGM240PB32VNA",
        "interface": "swd",
        "serverpath": "JLinkGDBServer"
    }
]
```
This is a sample debugging configuration for the SparkFun Thing Plus Matter. You most likely need to change some values to make it work on your system and board.

Change the `device` property according to the MCU on your board. Here's a table for help:
| Board                                  | MCU           |
| -------------                          |:-------------:|
| SparkFun Thing Plus Matter MGM240P     | MGM240PB32VNA          |
| Silicon Labs xG24 Explorer Kit         | EFR32MG24B210F1536IM48 |
| Silicon Labs xG24 Dev Kit              | EFR32MG24B310F1536IM48 |
| Silicon Labs xG27 Dev Kit              | EFR32BG27C140F768IM40  |
| Silicon Labs BGM220 Explorer Kit       | BGM220PC22HNA          |
| Ezurio Lyra 24P 20dBm Dev Kit          | BGM240PB32VNA3         |


Change the `(variant name)` in the `configId` property according to the FQBN (Fully Qualified Board Name) of the variant you're using:

*"configId": SiliconLabs:silabs:`(variant name)`:programmer=commander*

Here's a table listing all the `variant name`s:

| Board                                         | Variant name          |
| -------------                                 |:-------------:|
| SparkFun Thing Plus Matter                    | thingplusmatter       |
| Silicon Labs xG24 Explorer Kit                | xg24explorerkit       |
| Silicon Labs xG24 Dev Kit                     | xg24devkit            |
| Silicon Labs xG27 Dev Kit                     | xg27devkit            |
| Silicon Labs BGM220 Explorer Kit              | bgm220explorerkit     |
| Ezurio Lyra 24P 20dBm Dev Kit                 | lyra24p20             |

Change the `serverpath` property to the location of *JLinkGDBServer* on your system. *JLinkGDBServer* is installed with the *J-Link Software and Documentation pack*.

 - On Windows it's usually at `C:/Program Files/SEGGER/JLink/JLinkGDBServerCL`
 - On Linux and macOS you can just use `JLinkGDBServer` without the full path if it's on your PATH

Make sure 'Simplicity Commander' is selected in *Tools > Programmer*.

You can begin a debug session by pressing the `Start debugging` button in the top left corner next to the *Verify* and *Upload* buttons in the Arduino IDE.

There are two examples provided with a debugger configuration for the *SparkFun Thing Plus Matter MGM240P* board - one for Unix-based systems (Linux and macOS) and one for Windows.
You can find them under *File > Examples > SiliconLabs > thingplusmatter_debug_unix/win*.
These examples can be easily modified to work with the other boards just by changing the `device` property in `debug_custom.json`.

Here's the [official Arduino guide](https://docs.arduino.cc/tutorials/mkr-wifi-1010/mkr-jlink-setup#installing-the-j-link-gdb-server-software) on using SEGGER J-Link debuggers.

## Debugging on OpenOCD compatible boards

OpenOCD capable boards like the *Arduino Nano Matter* come equipped with a *Atmel SAMD11* board controller which can be used to debug the main CPU.

You'll need to perform the following steps to debug on OpenOCD boards:

### Debugger configuration file
Go to the folder where your sketch is located and create a file named `debug_custom.json` - and add the following content to it:
```json
[
  {
      "configId": "SiliconLabs:silabs:nano_matter:programmer=openocd",
      "servertype": "openocd",
      "device": "MGM240SD22VNA",
      "interface": "swd",
      "serverpath": "openocd"
  }
]
```

Change the `device` property according to the MCU on your board. Here's a table for reference:
| Board                      | MCU           |
| -------------              |:-------------:|
| Arduino Nano Matter        | MGM240SD22VNA          |
| Seeed Studio XIAO MG24     | EFR32MG24B220F1536IM48 |

Change the `(variant name)` in the `configId` property according to the FQBN (Fully Qualified Board Name) of the variant you're using:

*"configId": SiliconLabs:silabs:`(variant name)`:programmer=openocd*

Here's a table listing all the OpenOCD `variant name`s:

| Board                      | Variant name  |
| -------------              |:-------------:|
| Arduino Nano Matter        | nano_matter   |
| Seeed Studio XIAO MG24     | xiao_mg24     |


### Launch a debugging session

 - Make sure 'OpenOCD' is selected in *Tools > Programmer*.
 - Compile and upload your sketch
 - Press the *Start Debugging* button next to the *Upload* button

## Limitations

### USB-UART baud rate
The UART baud rate of Serial can be changed freely - however if you're using it through the USB-UART bridge then it only works with *115200 bps* by default.
This is because the Silicon Labs boards use an EFM32 microcontroller as a board controller/debugger/flasher/USB-UART converter and this controller has a separate configuration.
If you wish to change the baud rate used through the USB-UART bridge, then you can configure the board controller to use a different speed from it's admin console. The admin console can be reached from [Simplicity Studio](https://www.silabs.com/developers/simplicity-studio). Use [this](https://community.silabs.com/s/article/wstk-virtual-com-port-baudrate-setting?language=en_US) guide to change the baud rate in the board controller. The baud rate in your sketch must match the baud rate configured in the board controller - otherwise communication won't work.
This limitation **does NOT affect the Arduino Nano Matter or other OpenOCD compatible boards** as they use a different board controller.

## Questions and help

Have a question or stuck somewhere? Made something cool? 🕹️ Hit us up on Reddit at [r/silabs](https://www.reddit.com/r/silabs/)!

If you encounter an issue you can also submit it to the project issues.

## Resources for the supported boards

### Arduino Nano Matter ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)
[Product page](https://store.arduino.cc/pages/nano-matter) |
[Overview & guide](https://docs.arduino.cc/tutorials/nano-matter/user-manual/) | [Pinout diagram](https://docs.arduino.cc/tutorials/nano-matter/user-manual/#pinout)

![](doc/nano_matter_drawing.png)

### SparkFun Thing Plus Matter MGM240P ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)
[Product page](https://www.sparkfun.com/products/20270) |
[Overview & guide](https://learn.sparkfun.com/tutorials/sparkfun-thing-plus-matter---mgm240p-hookup-guide)

#### Pinout diagram
![](doc/sparkfunthingplusmatter_pinout.png)

### xG24 Explorer Kit ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)

[Product page](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit) |
[User guide](https://www.silabs.com/documents/public/user-guides/ug533-xg24-ek2703a.pdf)
#### Pinout diagram
![](doc/xg24explorerkit_pinout.png)

### xG24 Dev Kit ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)

[Product page](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit) |
[User guide](https://www.silabs.com/documents/public/user-guides/ug524-brd2601b-user-guide.pdf)
#### Pinout diagram
![](doc/xg24devkit_pinout.png)

### xG27 Dev Kit ![BLE](doc/bluetooth_logo_icon.png)

[Product page](https://www.silabs.com/development-tools/wireless/efr32xg27-development-kit) |
[User guide](https://www.silabs.com/documents/public/user-guides/ug554-brd2602a-user-guide.pdf)
#### Pinout diagram
![](doc/xg27devkit_pinout.png)

### BGM220 Explorer Kit ![BLE](doc/bluetooth_logo_icon.png)

[Product page](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit) |
[User guide](https://www.silabs.com/documents/public/user-guides/ug465-brd4314a.pdf)
#### Pinout diagram
![](doc/bgm220explorerkit_pinout.png)

### Ezurio Lyra 24P 20dBm Dev Kit ![BLE](doc/bluetooth_logo_icon.png)

[Product page](https://www.ezurio.com/part/453-00145-k1)

#### Pinout diagram
![](doc/lyra24p20_pinout.png)

### Seeed Studio XIAO MG24 (Sense) ![BLE](doc/bluetooth_logo_icon.png) ![Matter](doc/matter_logo_icon.png)

[Product page](https://www.seeedstudio.com/Seeed-Studio-XIAO-MG24-p-6247.html)

## Issue reporting, feature requests and discussions

We're encouraging the community to report any issues encountered using the core. You're also welcome to submit feature requests if you have a concrete idea on new features or improvements.

Please **always use the appropriate templates** when filing these in order to be accepted.

You can also open a *Discussion* if you have general or usability questions, or you'd like to share your project/ideas. Please, **do not to open issues for general questions** - always use *Discussion*s instead.

## Core development

### Contributing
Currently the development is in-house with each release open-sourced at publication.
We're currently committed and working towards opening up the development for the community - we're going to get there soon - but unfortunately at the moment we can't yet accept community PRs
into this repository. Until we open things up feel free to submit feature requests, bug reports and ideas for improvement.

### Environment setup

If you want to clone the core and develop with it you'll need to perform a few set up steps after cloning.

The core builds on Arduino's [Core-API](https://github.com/arduino/ArduinoCore-API) which is a submodule of this project under *extra/core-api*.

First, initialize the submodules with: ```git submodule update --init --recursive```

You'll need to create a symlink to ```extra/core-api/api``` in the ```cores/silabs/``` folder.

There's a script which does all this for you on macOS/Linux:

```
cd package
./bootstrap.sh
```

For further details see [this page](./package/README.md).

##

![Silabs <3 Arduino](doc/silabs_h_arduino.gif)

###### Made with ❤️ at Silicon Labs & Arduino
