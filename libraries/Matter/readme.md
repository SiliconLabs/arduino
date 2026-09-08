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

You can use the python script '*extra_firmware_flasher.py*' under '*extra/firmware/*' to automatically flash the appropriate OpenThread RCP firmware on your board.

To manually set up your board as an OpenThread RCP do the following:
 - Flash the bootloader on your device
 - Flash *'extra/firmware/openthread_rcp_\<boardname>.hex'* on your device
 - Plug the board into your Raspberry Pi via USB
 - Follow the steps in the Raspberry Pi OTBR guide

Read more and get the Raspberry Pi OTBR image [here](https://docs.silabs.com/matter/latest/matter-prerequisites/matter-artifacts).

## Supported Matter devices

 - Air purifier
 - Air quality sensor
 - Contact sensor
 - Concentration measurement (CO, CO2, TVOC)
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
 - Power source (battery)
 - Pressure measurement
 - Rain sensor
 - Switch
 - Temperature measurement
 - Thermostat
 - Water heater
 - Water valve
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

## class MatterCO

```void set_measured_value(float value);```

```float get_measured_value();```

```void set_measured_value(uint32_t value);```

## class MatterCO2

```void set_measured_value(float value);```

```float get_measured_value();```

```void set_measured_value(uint32_t value);```

## class MatterContact

Class for creating and controlling a Matter Contact Sensor appliance.

```void set_state(bool value);```

```bool get_state();```

## class MatterDoorLock

```bool is_locked();```

```void set_locked(bool locked);```

```void toggle();```

Door locks can also use PIN and RFID credentials. The
[`matter_door_lock_credentials`](examples/matter_door_lock_credentials/matter_door_lock_credentials.ino)
example demonstrates local provisioning, credential changes from a Matter controller, and
reporting an RFID credential unlock.

```bool set_credential(CredentialTypeEnum type, uint16_t index, const uint8_t *credential, size_t length);```

Adds or replaces a credential at the specified index.

```bool clear_credential(CredentialTypeEnum type, uint16_t index);```

Removes the credential at the specified index.

```void report_credential_unlock(CredentialTypeEnum type, uint16_t index);```

Reports that a physical credential was presented and unlocks the door. The example simulates an
RFID reader through the Serial Monitor by accepting `deadbeef`, which matches the demonstration
UID `DE:AD:BE:EF`. Replace the example PIN `1234` and RFID UID before using this on real hardware.

```bool setFeatures(uint32_t feature_map);```

```bool setFeatures(std::initializer_list<Feature> features);```

Selects the Door Lock features before calling `begin()`. The supported features are
`MatterDoorLock::Feature::kPinCredential`, `MatterDoorLock::Feature::kRfidCredential`, and
`MatterDoorLock::Feature::kUser`. The default is to enable all three. For example:

```cpp
matter_door_lock.setFeatures({ MatterDoorLock::Feature::kPinCredential,
                               MatterDoorLock::Feature::kRfidCredential });
matter_door_lock.begin();
```

The list can contain one or more features. `setFeatures()` returns `false` if called after `begin()`
or if unsupported feature bits are provided. Disabled PIN, RFID, and User operations are rejected
by the device.

## Selecting Matter features from a sketch

Features can be selected in the sketch by choosing the appropriate appliance class or its
initialization method. For example:

```cpp
MatterLightbulb lightbulb;             // On/Off
MatterDimmableLightbulb dimmable;      // On/Off + dimming
MatterColorLightbulb color;            // On/Off + dimming + color
```

Some appliances also provide specialized initialization methods, such as
`MatterSwitch::beginWithBinding()` for a switch with the Binding cluster, or `begin(false)` for
an appliance that should not use the default bridged endpoint. A sketch can instantiate several
appliances to expose multiple Matter endpoints.

For other appliance classes, the Matter `FeatureMap`, clusters, device types, and supported
commands are defined by the library. They cannot currently be changed through a generic API in the
sketch.

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

## class MatterSimplePowerSource

Class for creating and controlling a simple Matter Power Source appliance with battery percent and battery charge level support.

```void set_battery_percent(uint8_t value);```

```uint8_t get_battery_percent();```

```void set_battery_charge_level(uint8_t value);```

```uint8_t get_battery_charge_level();```

## class MatterPowerSource

Class for creating and controlling a Matter Power Source appliance with battery attributes.

```void set_status(uint8_t value);```

```uint8_t get_status();```

```void set_order(uint8_t value);```

```uint8_t get_order();```

```void set_description(const char* value);```

```const char* get_description();```

```void set_bat_voltage(uint32_t value);```

```uint32_t get_bat_voltage();```

```void set_bat_percent_remaining(uint8_t value);```

```uint8_t get_bat_percent_remaining();```

```void set_bat_time_remaining(uint32_t value);```

```uint32_t get_bat_time_remaining();```

```void set_bat_charge_level(uint8_t value);```

```uint8_t get_bat_charge_level();```

```void set_bat_replacement_needed(bool value);```

```bool get_bat_replacement_needed();```

```void set_bat_replaceability(uint8_t value);```

```uint8_t get_bat_replaceability();```

```void set_bat_present(bool value);```

```bool get_bat_present();```

## class MatterPressure

```void set_measured_value(int16_t value);```

```void set_measured_value(float value);```

```void set_measured_value(double value);```

```int16_t get_measured_value();```

## class MatterRain

```void set_measured_value(bool value);```

```bool get_measured_value();```

## class MatterSwitch

```bool beginWithBinding();```

Same as ```begin()```, but also adds the standard Matter Binding cluster to the switch's endpoint, so a Matter controller (e.g. Home Assistant or chip-tool) can bind it directly to another device (e.g. a light) and have button presses toggle it without a hub relaying every command. See the ```matter_switch_binding``` example.

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

## class MatterTimeSynchronization

Receives UTC time (and optional timezone / DST offsets) from a Matter controller via the Time Synchronization cluster on the root endpoint (not a bridged appliance).

The device does not pull time by itself. A Matter controller that supports time sync must push `SetUTCTime` (and, when the TimeZone feature is advertised, `SetTimeZone` / `SetDSTOffset`). Use `request_time()` to emit a `TimeFailure` event so a supporting controller can push time again.

### Enabling time sync in Home Assistant

Requires Matter Server **1.2.0** (Home Assistant Matter Server app **9.1.0**) or later versions.

#### Home Assistant OS / Supervised (Matter Server add-on)

1. Open **Settings → Add-ons → Matter Server → Configuration**.
2. Set **`time_sync`** to one of:
   - `auto` (default) — enable only when the host clock is NTP synchronized
   - `on` — always enable (use this if sync never starts under `auto`)
   - `off` — disable time sync
3. Ensure Home Assistant’s timezone is set correctly under **Settings → System → General** (the add-on uses the host timezone for `SetTimeZone` / DST).
4. Save and **restart** the Matter Server add-on.

#### Standalone Docker / Docker Compose (`ghcr.io/matter-js/matterjs-server`)

Time sync is **off by default** in the container image. Enable it with environment variables:

```yaml
services:
  matterjs-server:
    image: ghcr.io/matter-js/matterjs-server:stable
    network_mode: host
    restart: unless-stopped
    volumes:
      - "${HOME}/.matterjs-server:/data"
    environment:
      ENABLE_TIME_SYNC: "true"
      TZ: "Europe/Budapest"   # IANA zone; required for correct local offsets
```

Or with `docker run`:

```bash
docker run -d \
  --name matterjs-server \
  --restart=unless-stopped \
  --network=host \
  -v ${HOME}/.matterjs-server:/data \
  -e ENABLE_TIME_SYNC=true \
  -e TZ=Europe/Budapest \
  ghcr.io/matter-js/matterjs-server:stable
```

After changing `TZ` / `ENABLE_TIME_SYNC`, recreate the container so the environment is applied (`docker compose up -d --force-recreate`). Verify inside the container:

```bash
docker exec matterjs-server sh -c 'echo TZ=$TZ; date; node -e "console.log(Intl.DateTimeFormat().resolvedOptions().timeZone)"'
```

`TZ` must be non-empty and resolve to your IANA zone (not `UTC`), or timezone/DST offsets pushed to the device will be zero.

```bool request_time();```
Emits a `TimeFailure` event on the root endpoint so a supporting controller can push time again. Rate-limited to once per 60 seconds. Returns `false` if skipped by the rate limit or if logging fails. Controllers may still ignore the event during their own cooldown windows.

```bool has_time();```
Returns whether a valid wall-clock time is available.

```uint32_t get_unix_time();```
Returns seconds since 1970-01-01 UTC, or 0 if time is not available.

```uint64_t get_unix_time_millis();```
Returns milliseconds since 1970-01-01 UTC, or 0 if time is not available.

```bool has_timezone();```
Returns whether the controller has set a timezone via `SetTimeZone`.

```int32_t get_timezone_offset_seconds();```
Returns the active timezone base offset in seconds east of UTC (excluding DST), or 0 if unavailable.

```int32_t get_dst_offset_seconds();```
Returns the currently applicable DST offset in seconds, or 0 if unavailable / not in DST.

```int32_t get_local_offset_seconds();```
Returns timezone + DST offset in seconds east of UTC.

```uint32_t get_local_unix_time();```
Returns local Unix time in seconds (UTC + offsets). If timezone is not set yet, returns UTC.

```uint64_t get_local_unix_time_millis();```
Returns local Unix time in milliseconds (UTC + offsets). If timezone is not set yet, returns UTC.

```void set_time_update_callback(void (*cb)(void));```
Sets a callback invoked when UTC time becomes available or is updated by the controller.

```void set_timezone_update_callback(void (*cb)(void));```
Sets a callback invoked when the timezone list is updated by the controller.

## class MatterTVOC

```void set_measured_value(float value);```

```float get_measured_value();```

```void set_measured_value(uint32_t value);```

## class MatterWaterHeater

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

```water_heater_mode_t get_system_mode();```

```void set_system_mode(water_heater_mode_t system_mode);```

```uint8_t get_heater_types();```

```void set_heater_types(uint8_t heater_types);```

```uint8_t get_heat_demand();```

```uint16_t get_tank_volume();```

```void set_tank_volume(uint16_t tank_volume);```

```uint8_t get_tank_percentage();```

```void set_tank_percentage(uint8_t tank_percentage);```

```boost_state_t get_boost_state();```

```void set_boost_state(boost_state_t boost_state);```

```operation_mode_t get_operation_mode();```

```void set_operation_mode(operation_mode_t operation_mode);```

## class MatterWaterValve

```void open();```

```void open(uint32_t duration_seconds);```

```void close();```

```valve_state_t get_current_state();```

```valve_state_t get_target_state();```

```uint32_t get_open_duration();```

```uint32_t get_default_open_duration();```

```void set_default_open_duration(uint32_t duration_seconds);```

```uint32_t get_remaining_duration();```

```void set_remaining_duration(uint32_t remaining_duration_seconds);```

```uint16_t get_valve_fault();```

```void set_valve_fault(valve_fault_t valve_fault);```

## class MatterWindowCovering

```void set_current_operation(window_covering_current_operation_t current_operation);```

```void set_actual_lift_position_raw(uint16_t lift_position);```

```void set_actual_lift_position_percent(uint8_t lift_position_percent);```

```uint16_t get_actual_lift_position_raw();```

```uint8_t get_actual_lift_position_percent();```

```void set_requested_lift_position_raw(uint16_t lift_position);```

```void set_requested_lift_position_percent(uint8_t lift_position_percent);```

```uint16_t get_requested_lift_position_raw();```

```uint8_t get_requested_lift_position_percent();```

```void set_stop_request_callback(void (*window_covering_stop_request_cb)(void));```
