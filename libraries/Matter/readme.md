# Arduino Matter library

![Matter](../../doc/matter_logo.png)

This library makes using Matter easy and accessible on the Arduino ecosystem.

The supported boards use Matter over Thread. To connect your device you'll need a smart home device which can act as an Open Thread Border Router (OTBR) and provide a gateway between the Thread network and your WiFi.

## Supported devices, ecosystems and OTBRs

Supported Arduino boards:
 - Arduino Nano Matter
 - SparkFun Thing Plus Matter
 - Silicon Labs xG24 Explorer Kit
 - Silicon Labs xG24 Dev Kit
 - Seeed Studio XIAO MG24 (Sense)

Supported Thread Border Routers:

| Manufacturer / Ecosystem              | Device                        |
| -------------                         |:-------------:|
| Google Home                           | Nest Hub Gen2                 |
| Apple HomeKit                         | HomePod Gen2, HomePod mini    |
| Amazon Alexa                          | Echo Gen4                     |
| Raspberry Pi OTBR                     | Raspberry Pi                  |

Read more on setting up your Raspberry Pi as an OTBR [here](https://docs.silabs.com/matter/latest/matter-thread/raspi-img).

### OpenThread RCP firmware

Supported boards can also be used as a USB-OpenThread bridge (RCP - Radio Co-Processor) with a Raspberry Pi OTBR.

Boards supporting OpenThread RCP:
 - Arduino Nano Matter
 - Seeed Studio XIAO MG24 (Sense)
 - SparkFun Thing Plus Matter MGM240P

To set up your board as an OpenThread RCP do the following:
 - Flash the bootloader on your device
 - Flash *'extra/openthread_rcp/openthread_rcp_\<boardname>.hex'* on your device
 - Plug the board into your Raspberry Pi via USB
 - Follow the steps in the Raspberry Pi OTBR guide

Read more and get the Raspberry Pi OTBR image [here](https://docs.silabs.com/matter/latest/matter-prerequisites/matter-artifacts).

## Supported Matter devices

 - Air quality sensor
 - Contact sensor
 - Door lock
 - Fan
 - Flow measurement
 - Humidity measurement
 - Illuminance measurement
 - On/Off lightbulb
 - Dimmable lightbulb
 - Color lightbulb
 - Occupancy sensor
 - On/Off Plug-in Unit / Outlet
 - Pressure measurement
 - Switch
 - Temperature measurement
 - Thermostat
 - Window covering

## Setting up your Arduino device

 - Flash the bootloader to your Arduino board if this is your first time using it
 - Flash your Arduino board with a Matter application/example/sketch
 - Open the Serial Monitor for you board
 - If you device is not commissioned to any Matter network it'll print the link for the QR code and manual code required for pairing
 - If you're using Google's ecosystem you'll need to complete additional setup steps in the *Google Home Developer Console*.

   This [video](https://www.youtube.com/watch?v=R_kpSO7PtUo) will guide through the setup process. You can use the same **Vendor ID (0xfff1)** and **Product ID (0x8005)** as shown in the video.

   You only need to do this once.
 - Open the smart-home app of your choice on your smartphone (Google Home, Apple Home, Amazon Alexa)
 - Add a new Matter device in the app (if the app asks whether the device you're adding has a Matter logo - say yes)
 - Scan the QR code or enter the manual pin provided by your Arduino
 - Wait for the pairing to finish
 - Congratulations - you successfully paired your Arduino as a Matter device
 - You only need to do this once - pairing persists even after you restart/reflash your device

Note, that an additional *Matter Hub* device will be added alongside your Matter devices to your smart home environment. This is required to ensure correct operation on Apple and Amazon ecosystems and can be ignored/hidden after the onboarding completes.

## Additional info

### Unpairing / decommissioning

Matter variants save the network keys and credentials in permanent memory, so you don't have to re-pair them when they're restarted.

If you need to make your device forget the network credentials but you're unable to unpair them from the network - you can erase the chip with it's permanent storage.

You can do this by re-burning the bootloader in the Arduino IDE (*Tools > Burn Bootloader*). The IDE will perform a full chip erase before re-flashing your bootloader.
After this you'll need to upload your sketch again. Your sketch will start with a blank slate prompting you to pair it.

You can use the Matter library as well to unpair your devices from code. See the **matter_decommission** example for details.

### Changing Matter device identities and provisioning data

Some ecosystems (as of now Google) requires each Matter device to have unique identities in order to pair.

We provide a python script for changing the Matter device identities and provisioning data easily.

See the [Arduino Matter Provision Tool](../../extra/arduino_matter_provision/readme.md) under *'extra/arduino_matter_provision'*.

## Arduino Nano Matter - Matter setup guide

You can also refer to Arduino's [Matter setup guide](https://docs.arduino.cc/tutorials/nano-matter/user-manual/#matter) for the *Nano Matter* to get started with your Matter setup. The guide is for the *Nano Matter* - but most of the contents are applicable to all boards supporting Matter.

## Matter with BLE

You can use the Silicon Labs BLE stack simultaneously with Matter/Thread to communicate with BLE devices.

There are some limitations compared to using a BLE only radio stack:

 - Make sure you're not interfering with the BLE stack before the Matter commissioning finishes
 - After Matter commissioning finished the BLE stack is freely usable with all features
 - The Matter commissioning GATT services can be disabled after commissioning
 - Most BLE examples can work alongside with Matter with some small modifications

For detailed usage - see the provided Matter+BLE examples in the Matter library's *example* folder.

# API documentation

## class Matter

This class manages the global state of Matter, responsible for starting up the endpoint management and for providing the current status and onboarding codes.

The class has the following methods available:

```void begin();```
Starts Matter and the dynamic endpoint handler on the device. Must be called before calling ```begin()``` on any appliance class.

```String getManualPairingCode();```
Returns the manual pairing code required for onboarding the device on a Matter network.

```String getOnboardingQRCodeUrl();```
Returns a link to the QR code required for onboarding the device on a Matter network.

```String getOnboardingQRCodePayload();```
Returns the QR code payload for onboarding the device on a Matter network. Ideal when generating your own QR codes.

```bool isDeviceCommissioned();```
Returns whether the device has been commissioned to a Matter network.

```bool isDeviceThreadConnected();```
Returns whether the device is connected to the Thread network.

```void decommission();```
Decommissions the device from its paired network by erasing the stored network keys, then performs a reboot.

## Appliance classes

Each Matter appliance class has a ```begin()``` and an ```end()``` function.

```begin()``` must be called to create the endpoint and present it on the Matter network, and to be able to receive and send data through it.

Calling ```end()``` will take the appliance offline and destroy it.

Instances of the Matter classes automatically update themselves with the values received from the network - and provide getter functions for the users to obtain their current state.

Users can publish measurement/control data through the Matter classes which will be automatically transmitted through the Matter network.

```is_online()``` can be called on all Matter appliances to check whether they have been discovered on the network by the appropriate client - which is most likely your Matter hub.
When this function returns *true* it means that your appliance is able to communicate with your Matter hub - and therefore it's online.

```set_device_change_callback(void (*matter_device_changed_cb)(void))``` can be called to set a callback function which will be called when any of the device's properties are changed by the Matter controller.

All appliance classes have the following methods for setting it's custom properties:

```void set_device_name(const char* device_name);```
Sets the name of the device.

```void set_vendor_name(const char* vendor_name);```
Sets the manufacturer/vendor name of the device.

```void set_product_name(const char* product_name);```
Sets the model/product name of the device.

```void set_serial_number(const char* serial_number);```
Sets the serial number of the device.

## class MatterAirPurifier

```void set_air_quality(MatterAirQuality::AirQuality_t value);```

```MatterAirQuality::AirQuality_t get_air_quality();```

```void set_fan_onoff(bool value);```

```bool get_fan_onoff();```

```void set_fan_percent(uint8_t percent);```

```uint8_t get_fan_percent();```

## class MatterAirQuality

```void set_air_quality(MatterAirQuality::AirQuality_t value);```

```MatterAirQuality::AirQuality_t get_air_quality();```

## class MatterContact

Class for creating and controlling a Matter Contact Sensor appliance.

```void set_state(bool value);```

```bool get_state();```

## class MatterDoorLock

```bool is_locked();```

```void set_locked(bool locked);```

```void toggle();```

## class MatterFan

Class for creating and controlling a Matter Fan appliance.

```void set_onoff(bool value);```

```bool get_onoff();```

```void set_percent(uint8_t percent);```

```uint8_t get_percent();```

```void set_mode(DeviceFan::fan_mode_t fan_mode);```

```DeviceFan::fan_mode_t get_mode();```

## class MatterFlow

Class for creating and controlling a Matter Flow measurement appliance.

```void set_measured_value_raw(uint16_t value);```

```uint16_t get_measured_value_raw();```

```void set_measured_value_cubic_meters_per_hour(double value);```

```double get_measured_value_cubic_meters_per_hour();```

## class MatterHumidity

```void set_measured_value_raw(uint16_t value);```

```void set_measured_value(float value);```

```void set_measured_value(double value);```

```uint16_t get_measured_value_raw();```

```float get_measured_value();```

## class MatterIlluminance

```void set_measured_value_raw(uint16_t value);```

```void set_measured_value_lux(double lux);```

```void set_measured_value_lux(float lux);```

```void set_measured_value_lux(uint16_t lux);```

```double get_measured_value_lux();```


## class MatterLightbulb

Class for creating and controlling a Matter Lightbulb appliance.

```void set_onoff(bool value);```

```bool get_onoff();```

## class MatterDimmableLightbulb

Has all the methods of ```MatterLightbulb``` and adds the following:

```uint8_t get_brightness();```

```void set_brightness(uint8_t brightness);```

```uint8_t get_brightness_percent();```

```void set_brightness_percent(uint8_t brightness);```

## class MatterColorLightbulb

Has all the methods of ```MatterDimmableLightbulb``` and adds the following:

```uint8_t get_hue();```

```uint8_t get_saturation();```

```void set_hue(uint8_t hue);```

```void set_saturation(uint8_t saturation);```

```uint16_t get_true_hue();```

```uint8_t get_saturation_percent();```

```void set_true_hue(uint16_t true_hue);```

```void set_saturation_percent(uint8_t saturation);```

```void get_rgb(uint8_t* r, uint8_t* g, uint8_t* b);```

```void get_rgb_raw(uint8_t* r, uint8_t* g, uint8_t* b);```

```void set_rgb(uint8_t r, uint8_t g, uint8_t b);```

```void boost_saturation(uint8_t amount);```

## class MatterOccupancy

```void set_occupancy(bool occupied);```

```bool get_occupancy();```

## class MatterOnOffPluginUnit

```void set_onoff(bool value);```

```bool get_onoff();```

```void toggle();```

## class MatterPressure

```void set_measured_value(int16_t value);```

```void set_measured_value(float value);```

```void set_measured_value(double value);```

```int16_t get_measured_value();```

## class MatterSwitch

```void set_state(bool state);```

```bool get_state();```

## class MatterTemperature

```void set_measured_value_raw(int16_t value);```

```void set_measured_value_celsius(float value);```

```void set_measured_value_celsius(double value);```

```int16_t get_measured_value_raw();```

```float get_measured_value_celsius();```

## class MatterThermostat

```int16_t get_local_temperature_raw();```

```void set_local_temperature_raw(int16_t local_temp);```

```int16_t get_heating_setpoint_raw();```

```void set_heating_setpoint_raw(int16_t heating_setpoint);```

```float get_local_temperature();```

```void set_local_temperature(float local_temp);```

```float get_heating_setpoint();```

```void set_heating_setpoint(float heating_setpoint);```

```void set_absolute_minimum_heating_setpoint(float abs_min_heating_setpoint);```

```void set_minimum_heating_setpoint(float min_heating_setpoint);```

```void set_absolute_maximum_heating_setpoint(float abs_max_heating_setpoint);```

```void set_maximum_heating_setpoint(float max_heating_setpoint);```

```float get_absolute_minimum_heating_setpoint();```

```float get_minimum_heating_setpoint();```

```float get_absolute_maximum_heating_setpoint();```

```float get_maximum_heating_setpoint();```

```thermostat_mode_t get_system_mode();```

```void set_system_mode(thermostat_mode_t system_mode);```

## class MatterWindowCovering

```void set_current_operation(window_covering_current_operation_t current_operation);```

```void set_actual_lift_position_raw(uint16_t lift_position);```

```void set_actual_lift_position_percent(uint8_t lift_position_percent);```

```uint16_t get_actual_lift_position_raw();```

```uint8_t get_actual_lift_position_percent();```

```uint16_t get_requested_lift_position_raw();```

```uint8_t get_requested_lift_position_percent();```
