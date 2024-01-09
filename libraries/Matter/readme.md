# Arduino Matter library

![Matter](../../doc/matter_logo.png)

This library makes using Matter easy and accessible on the Arduino ecosystem.

The supported boards use Matter over Thread. To connect your device you'll need a smart home device which can act as an Open Thread Boarder Router (OTBR) and provide a gateway between the Thread network and your WiFi.

## Supported devices, ecosystems and OTBRs

Supported Arduino boards:
 - Sparkfun ThingPlus Matter
 - Silicon Labs xG24 Explorer Kit

Supported Thread Boarder Routers:

| Manufacturer / Ecosystem              | Device                        |
| -------------                         |:-------------:|
| Google Home                           | Nest Hub Gen2                 |
| Apple HomeKit                         | HomePod Gen2, HomePod mini    |
| Amazon Alexa                          | Echo Gen4                     |
| Raspberry Pi OTBR                     | Raspberry Pi                  |

Read more on setting up your Raspberry Pi as an OTBR [here](https://docs.silabs.com/matter/2.2.0/matter-thread/raspi-img).

## Supported Matter devices

 - Fan
 - Flow measurement
 - Humidity measurement
 - Illuminance measurement
 - On/Off lightbulb
 - Dimmable lightbulb
 - Color lightbulb
 - Occupancy sensor
 - Pressure measurement
 - Switch
 - Temperature measurement
 - Thermostat

## Setting up your Arduino device

 - Flash you Arduino board with a Matter application/example/sketch
 - **Flash the bootloader (Matter variants require a different bootloader than the BLE variants!)**
 - Open the Serial Monitor for you board
 - If you device is not commissioned to any Matter network it'll print the link for the QR code and manual code required for pairing
 - Open the smart-home app of your choice on your smartphone (Google Home, Apple Home, Amazon Alexa)
 - Add a new Matter device in the app (if the app asks whether the device you're adding has a Matter logo - say yes)
 - Scan the QR code or enter the manual pin provided by your Arduino
 - Wait for the pairing to finish
 - Congratulations - you successfully paired your Arduino as a Matter device
 - You only need to do this once - pairing persists even after you restart/reflash your device

# API documentation

Each Matter appliance class has a ```begin()``` and an ```end()``` function.

```begin()``` must be called to create the endpoint and present it on the Matter network, and to be able to receive and send data through it.

Calling ```end()``` will take the appliance offline and destroy it.

Instances of the Matter classes automatically update themselves with the values received from the network - and provide getter functions for the users to obtain their current state.

Users can publish measurement/control data through the Matter classes which will be automatically transmitted through the Matter network.

## class MatterFan

Class for creating and controlling a Matter Fan appliance.

```void set_onoff(bool value)```

```bool get_onoff()```

```void set_percent(uint8_t percent);```

```uint8_t get_percent();```

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
