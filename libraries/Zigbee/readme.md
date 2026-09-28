# Arduino Zigbee Library

![Zigbee](../../doc/zigbee_logo.png)

The Arduino Zigbee library lets users create Zigbee 3.0 smart home
devices on Silicon Labs hardware. It wraps the Silicon Labs EmberZNet stack and
the generated Zigbee endpoint configuration with Arduino-style classes for
lights, outlets, contact sensors, switches, temperature sensors, humidity
sensors, light sensors, and thermostats.

The library is intended for Silicon Labs based Arduino boards with the
Zigbee protocol stack selected.

## Supported Boards

- **Arduino Nano Matter**
- **Silicon Labs xG24 Explorer Kit**
- **SparkFun Thing Plus Matter**
- **Seeed Studio XIAO MG24 (Sense)**

Make sure `Tools > Protocol Stack > Zigbee` is selected in order to use the Zigbee library.
Sketches that include `Zigbee.h` intentionally fail to compile on non-Zigbee
board variants. If you see an error saying the selected board or variant is not
Zigbee compatible, select a board and protocol stack combination that supports
Zigbee.

## Coordinator Firmware

The examples in this library run as Zigbee devices that join an existing
network, but select Silicon Labs Arduino boards can also be turned into the
Zigbee Coordinator for that network. The bundled Zigbee EZSP firmware in
[`extra/firmware`](../../extra/firmware/readme.md) makes a supported board act
as a USB EmberZNet Serial Protocol dongle for a host such as Home Assistant.

Home Assistant's Zigbee Home Automation (ZHA) integration can use this EZSP
dongle as the coordinator. With one compatible board running the coordinator
firmware and other Arduino-compatible Silicon Labs boards running Zigbee
sketches, you can set up a complete Zigbee network using these boards.

Current Zigbee EZSP firmware images are provided for:

- **Arduino Nano Matter**
- **SparkFun Thing Plus Matter**
- **Seeed Studio XIAO MG24 (Sense)**

See the [`extra/firmware`](../../extra/firmware/readme.md) folder for the
flasher script, firmware files, and Home Assistant ZHA setup link.

## Included Examples

| Example | Description |
| --- | --- |
| `zigbee_lightbulb` | Creates an On/Off light controlled by a Zigbee coordinator. The onboard LED follows the Zigbee On/Off state. |
| `zigbee_on_off_outlet` | Creates an On/Off plug-in unit controlled by a Zigbee coordinator. The onboard LED follows the Zigbee On/Off state. |
| `zigbee_contact_sensor` | Creates an IAS Zone contact sensor. The onboard button drives the reported contact state and the onboard LED follows it. |
| `zigbee_power_source` | Creates a standalone Zigbee Power Configuration endpoint that publishes simulated battery percentage data. |
| `zigbee_lightbulb_dimmable` | Creates a dimmable light controlled by a Zigbee coordinator. The onboard LED follows the Zigbee On/Off state and brightness. |
| `zigbee_lightbulb_color` | Creates a color dimmable light controlled by a Zigbee coordinator. The Nano Matter RGB LED follows the Zigbee On/Off state, brightness, hue, and saturation. |
| `zigbee_lightbulb_identify` | Adds Identify cluster behavior to the lightbulb example. The onboard LED blinks while a coordinator is identifying the device. |
| `zigbee_switch` | Creates an On/Off switch that sends On, Off, and Toggle commands to bound lights. |
| `zigbee_switch_dimmer` | Creates a switch that sends On/Off and Level Control dimming commands to bound lights. |
| `zigbee_temp_sensor` | Publishes the board CPU temperature through the Zigbee Temperature Measurement cluster. |
| `zigbee_thermostat` | Creates a Zigbee thermostat with local temperature, heating setpoint, configurable heat setpoint limits, OFF/HEAT system mode, and IDLE/HEATING HVAC action. |
| `zigbee_end_device` | Publishes the board CPU temperature through the Zigbee Temperature Measurement cluster and joins the Zigbee network as an end device. |
| `zigbee_humidity_sensor` | Publishes a simulated relative humidity value through the Zigbee Relative Humidity Measurement cluster. |
| `zigbee_light_sensor` | Publishes a simulated illuminance value through the Zigbee Illuminance Measurement cluster. |
| `zigbee_time` | Requests the current timestamp from the Zigbee coordinator and calculates the current date and time. |

Open the examples from the Arduino IDE with **File > Examples > Zigbee**.

## Basic Flow

Every sketch follows the same shape:

1. Include `Zigbee.h` and the device class header you want to use.
2. Create one or more device objects globally.
3. Optionally set device metadata and pairing channel.
4. Call `Zigbee.begin()`.
5. Call `begin()` on each device object.
6. Put your Zigbee coordinator in pairing mode and wait for the device to join.

Minimal lightbulb:

```cpp
#include <Zigbee.h>
#include <ZigbeeLightbulb.h>

ZigbeeLightbulb bulb;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Zigbee.setVendorName("Arduino");
  Zigbee.setProductName("Zigbee Lightbulb");
  Zigbee.setFirmwareVersion(0x00000001);

  Zigbee.begin();
  bulb.begin();
}

void loop()
{
  if (bulb.get_onoff()) {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  } else {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  }
  delay(50);
}
```

Call `Zigbee.begin()` before calling `begin()` on the device objects. The global
`Zigbee` object initializes the stack-side endpoint state, and each appliance
then allocates and enables its endpoint.

## Pairing and Network Steering

`Zigbee.begin()` starts Zigbee network steering if the device is not already on a
network. Network steering is the Zigbee joining procedure: the device scans for
open Zigbee networks, tries to join one, and stores the network credentials when
joining succeeds.

To pair a sketch:

1. Upload a Zigbee sketch to the board.
2. Open the Serial Monitor at the sketch's configured baud rate.
3. Put your coordinator, such as Home Assistant ZHA, in pairing or permit-join mode.
4. Wait for the sketch to print that it joined the network.

Network credentials are stored in non-volatile memory, so a paired board
rejoins automatically after reset or reflash.

By default, devices join as Zigbee routers. To join as an end device instead,
set the device type before calling `Zigbee.begin()`:

```cpp
Zigbee.setDeviceType(ZIGBEE_DEVICE_TYPE_END_DEVICE);
Zigbee.begin();
```

`setDeviceType()` returns `true` before `Zigbee.begin()` and `false` afterward.
Changing the device type does not convert an already joined network identity.
If you switch between router and end device for a device that has already
paired, factory reset it and pair again.

By default, devices also advertise themselves as mains powered. To advertise a
device as battery powered instead, set the power source before calling
`Zigbee.begin()`:

```cpp
Zigbee.setPowerSource(ZIGBEE_POWER_SOURCE_TYPE_BATTERY);
Zigbee.begin();
```

Changing the advertised power source after a device has already paired may
require re-pairing so coordinators like Home Assistant ZHA refresh their cached
device interview data. In Home Assistant ZHA, battery entities are discovered
from the initial device interview, so if you switch between mains and battery
power sources, factory reset the device, remove the old ZHA device entry, and
pair again from scratch.

### Restricting the Pairing Channel

By default, network steering can use Zigbee channels 11 through 26. You can
restrict pairing to one channel:

```cpp
Zigbee.setPairingChannel(15);
Zigbee.begin();
```

`setPairingChannel()` returns `true` for a valid Zigbee channel and `false` for
invalid channels. This only controls joining scans. It does not move an already
joined device to another network channel.

## Factory Reset

The examples support holding the built-in button during boot to factory reset:

```cpp
if (digitalRead(BTN_BUILTIN) == LOW) {
  Zigbee.factoryReset();
}
```

`factoryReset()` leaves the current network when joined, clears stored Zigbee
network and token data from NVM3, and resets the MCU. Arduino `EEPROM` data in
the NVM3 user key range is preserved. Use this when you want the board to forget
its network and pair again from a clean state.

Alternatively you can use `Tools > Burn Bootloader` in the Arduino IDE to erase your board and any saved networks.

If your coordinator still shows an old device entry after factory reset, remove
that stale entry from the coordinator before pairing again.

## Light state persistence

On/Off Light, Dimmable Light, Color Light, and On/Off Plug-in Unit endpoints
persist the Zigbee **On/Off** attribute across reboot. Dimmable and color lights
also persist **Current Level** (brightness). After `begin()`, `get_onoff()` and
`get_level()` / `get_brightness_percent()` reflect those restored values, so your
sketch can drive the LED to match the last state.

Avoid calling `set_onoff()` or `set_brightness_percent()` / `set_level()`
unconditionally right after `begin()` unless you intentionally want to override
the restored state. Color hue and saturation are not persisted; only On/Off and
Current Level survive reboot.

`Zigbee.factoryReset()` clears this stored light state along with network
credentials. Arduino `EEPROM` contents are not erased.

## Device Metadata

Set metadata before starting the device:

```cpp
Zigbee.setVendorName("Arduino");
Zigbee.setProductName("Zigbee Temperature Sensor");
Zigbee.setFirmwareVersion(0x00000420);
```

Available metadata APIs:

```cpp
void Zigbee.setVendorName(const char* name);
void Zigbee.setProductName(const char* name);
void Zigbee.setFirmwareVersion(const char* version);
void Zigbee.setFirmwareVersion(uint32_t file_version);
bool Zigbee.setDeviceType(ZigbeeDeviceType node_type);
ZigbeeDeviceType Zigbee.getDeviceType();
```

`setVendorName()` writes the Basic cluster Manufacturer Name attribute.
`setProductName()` writes the Basic cluster Model Identifier attribute.

The string firmware overload writes the Basic cluster SW Build ID attribute and
is limited to 16 characters. The numeric overload writes Basic Application
Version and OTA Current File Version attributes, which are the fields many
coordinators use for firmware display and OTA metadata. This library does not
implement a Zigbee OTA update server.

## Appliance Lifecycle

All Zigbee appliance classes inherit from a common base and provide:

```cpp
bool begin();
void end();
bool is_online();
bool get_identify_in_progress();
void set_device_change_callback(void (*cb)(void));
```

`begin()` allocates, registers, and enables an endpoint of the correct device
type. Endpoint IDs are assigned automatically (up to three instances of each
appliance type). It returns `true` when the appliance was created successfully.

`end()` unregisters the endpoint and frees the object resources.

`is_online()` returns whether the appliance has been marked online by the
library. In the current implementation this becomes true after the local endpoint
is created.

`get_identify_in_progress()` returns true while the Zigbee Identify cluster is
active for that endpoint.

`set_device_change_callback()` registers a no-argument callback called when the
device state changes, for example when an On/Off attribute changes, a sensor
measurement is written, or Identify starts/stops. Register the callback after the
appliance `begin()` call, because the underlying endpoint object is created
during `begin()`.

The generated endpoint configuration currently supports 33 application
endpoints: three On/Off lights, three temperature sensors, three humidity
sensors, three On/Off switches, three dimmable lights, three light sensors,
three color dimmable lights, three On/Off plug-in units, three contact
sensors, three power sources, and three thermostats. It also provides one opt-in Time Client endpoint.

## Identifying a Device

Zigbee coordinators can send an Identify command to make a device physically
signal which device it is. The library tracks this state per endpoint:

```cpp
if (bulb.get_identify_in_progress()) {
  // Blink an LED, play a tone, or otherwise identify the device
}
```

The `zigbee_lightbulb_identify` example blinks the onboard LED while Identify is
active, then restores the LED to the current light on/off state when Identify
ends.

## Find and Bind (Light Target)

Find and Bind is a Zigbee 3.0 commissioning flow used by some controllers and
remotes to create cluster bindings without the coordinator writing them
manually. It is separate from network joining (`Zigbee.begin()` / network
steering). Coordinator-created bindings (for example in Home Assistant ZHA) work without this API.

Light appliances can act as a Find and Bind **target**. Calling
`start_find_and_bind()` starts Identify on the light endpoint for 180 seconds so
an initiator can discover it and create bindings. The library does **not** start
Find and Bind automatically; call this when the user requests it (for example
from a button press or once during power-on). Make sure to call `start_find_and_bind()` after `begin()`.

```cpp
#include <ZigbeeLightbulb.h>
ZigbeeLightbulb bulb;

void setup()
{
  pinMode(BTN_BUILTIN, INPUT);
  Zigbee.begin();
  bulb.begin();
}

void loop()
{
  if (digitalRead(BTN_BUILTIN) == BTN_BUILTIN_ACTIVE) {
    bulb.start_find_and_bind();
    delay(500);
  }

  if (bulb.get_identify_in_progress()) {
    // Optionally blink an LED while Find and Bind / Identify is active
  }
}
```

`start_find_and_bind()` returns `true` if Identify was started successfully.
It is available on `ZigbeeLightbulb` and classes that inherit from it
(`ZigbeeDimmableLightbulb`, `ZigbeeColorLightbulb`, and
`ZigbeeOnOffPluginUnit`).

## ZigbeeLightbulb

Header:

```cpp
#include <ZigbeeLightbulb.h>
```

Creates an On/Off Light endpoint. Remote On, Off, and Toggle commands update the
local state, and local calls update the Zigbee On/Off attribute.

API:

```cpp
bool begin();
void end();

void set_onoff(bool value);
bool get_onoff();
void toggle();
bool start_find_and_bind();

operator bool();
void operator=(bool state);
```

`start_find_and_bind()` makes this light a Find and Bind target for 180 seconds.
See [Find and Bind (Light Target)](#find-and-bind-light-target).

Example:

```cpp
#include <ZigbeeLightbulb.h>
ZigbeeLightbulb bulb;

void setup()
{
  Zigbee.begin();
  bulb.begin();
}

void loop()
{
  if (bulb.get_onoff()) {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  } else {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  }
}
```

You can also use the assignment and bool operators:

```cpp
bulb = true;

if (bulb) {
  // Bulb state is on
}
```

## ZigbeeOnOffPluginUnit

Header:

```cpp
#include <ZigbeeOnOffPluginUnit.h>
```

Creates an On/Off plug-in unit endpoint for outlets and smart plugs. It
inherits the `ZigbeeLightbulb` On/Off and Find and Bind API and behaves like an outlet from the
coordinator side. Remote On, Off, and Toggle commands update the local state,
and local calls update the Zigbee On/Off attribute.
On/Off attribute is restored after reboot;
See [Light state persistence](#light-state-persistence).

API:

```cpp
bool begin();
void end();

void set_onoff(bool value);
bool get_onoff();
void toggle();
bool start_find_and_bind();

operator bool();
void operator=(bool state);
```

Example:

```cpp
#include <ZigbeeOnOffPluginUnit.h>
ZigbeeOnOffPluginUnit outlet;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Zigbee.begin();
  outlet.begin();
}

void loop()
{
  digitalWrite(LED_BUILTIN, outlet.get_onoff() ? LED_BUILTIN_ACTIVE : LED_BUILTIN_INACTIVE);
  delay(50);
}
```

## ZigbeeDimmableLightbulb

Header:

```cpp
#include <ZigbeeDimmableLightbulb.h>
```

Creates a Dimmable Light endpoint. It inherits the `ZigbeeLightbulb` On/Off and
Find and Bind API and adds Level Control server support. Remote level changes update the local
brightness, and local calls update the Zigbee Current Level attribute.
On/Off and Current Level are restored after reboot;
See [Light state persistence](#light-state-persistence).

API:

```cpp
bool begin();
void end();

void set_onoff(bool value);
bool get_onoff();
void toggle();
bool start_find_and_bind();

void set_level(uint8_t level);
uint8_t get_level();
void set_brightness_percent(uint8_t percent);
uint8_t get_brightness_percent();
```

`set_level()` and `get_level()` use raw Zigbee Level Control values from 0-254.
`set_brightness_percent()` and `get_brightness_percent()` use percent values from 0-100.
`start_find_and_bind()` is inherited from `ZigbeeLightbulb`; see
[Find and Bind (Light Target)](#find-and-bind-light-target).

Example:

```cpp
#include <ZigbeeDimmableLightbulb.h>
ZigbeeDimmableLightbulb bulb;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Zigbee.begin();
  bulb.begin();
}

void loop()
{
  if (bulb.get_onoff()) {
    uint8_t pwm = (static_cast<uint16_t>(bulb.get_brightness_percent()) * 255 + 50) / 100;
    analogWrite(LED_BUILTIN, pwm);
  } else {
    analogWrite(LED_BUILTIN, 0);
  }
}
```

## ZigbeeColorLightbulb

Header:

```cpp
#include <ZigbeeColorLightbulb.h>
```

Creates a color dimmable light endpoint. It inherits the
`ZigbeeDimmableLightbulb` On/Off, brightness, and Find and Bind API and adds Color Control
server support for hue and saturation. Remote color changes update the local
color state, and local calls update the Zigbee Color Control attributes.
On/Off and Current Level are restored after reboot; hue and saturation are not.
See [Light state persistence](#light-state-persistence).

API:

```cpp
bool begin();
void end();

void set_onoff(bool value);
bool get_onoff();
void toggle();

void set_level(uint8_t level);
uint8_t get_level();
void set_brightness_percent(uint8_t percent);
uint8_t get_brightness_percent();

void set_hue(uint8_t hue);
uint8_t get_hue();
void set_saturation(uint8_t saturation);
uint8_t get_saturation();
void set_color(uint8_t hue, uint8_t saturation);

void set_true_hue(uint16_t true_hue);
uint16_t get_true_hue();
void set_saturation_percent(uint8_t saturation);
uint8_t get_saturation_percent();

void get_rgb(uint8_t* r, uint8_t* g, uint8_t* b);
void get_rgb_raw(uint8_t* r, uint8_t* g, uint8_t* b);
void set_rgb(uint8_t r, uint8_t g, uint8_t b);
void boost_saturation(uint8_t amount);
```

`set_hue()` and `get_hue()` use raw Zigbee Color Control values from 0-254.
`set_true_hue()` and `get_true_hue()` use degrees from 0-360.
`set_saturation()` and `get_saturation()` use raw Zigbee values from 0-254.
`set_saturation_percent()` and `get_saturation_percent()` use percent values
from 0-100.

`get_rgb()` returns the current color converted to RGB and scaled by the current
brightness. `get_rgb_raw()` returns the color at full brightness. `set_rgb()`
converts the supplied RGB value to hue, saturation, and brightness.

Example:

```cpp
#include <ZigbeeColorLightbulb.h>
ZigbeeColorLightbulb bulb;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN_1, OUTPUT);
  pinMode(LED_BUILTIN_2, OUTPUT);
  Zigbee.begin();
  bulb.begin();
}

void loop()
{
  uint8_t r;
  uint8_t g;
  uint8_t b;

  if (bulb.get_onoff()) {
    bulb.get_rgb(&r, &g, &b);
    analogWrite(LED_BUILTIN, r);
    analogWrite(LED_BUILTIN_1, g);
    analogWrite(LED_BUILTIN_2, b);
  }
  delay(50);
}
```

## ZigbeeSwitch

Header:

```cpp
#include <ZigbeeSwitch.h>
```

Creates an On/Off Switch endpoint. The switch sends On, Off, Toggle, and Level
Control commands to the coordinator and to configured bindings. After pairing,
create bindings from the switch's On/Off and Level Control client clusters to
the target light in your coordinator.

API:

```cpp
bool begin();
void end();

void on();
void off();
void toggle();
void dim_up(uint8_t rate_percent = 50);
void dim_down(uint8_t rate_percent = 50);
void stop_dimming();
void move_to_level(uint8_t level, uint32_t transition_time_ms = 0);
void move_to_percent(uint8_t percent, uint32_t transition_time_ms = 0);
```

`dim_up()` and `dim_down()` use percent-per-second rates converted to raw Level
Control rates. `move_to_percent()` converts 0-100 brightness percent values to
raw Level Control levels. `move_to_level()` is the raw Level Control command for
devices that need direct Zigbee level values.
Transition time arguments are specified in milliseconds.

Example:

```cpp
#include <ZigbeeSwitch.h>
ZigbeeSwitch zigbee_switch;

void setup()
{
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  Zigbee.begin();
  zigbee_switch.begin();
}

void loop()
{
  static bool last_button_state = HIGH;
  bool button_state = digitalRead(BTN_BUILTIN);

  if (last_button_state == HIGH && button_state == LOW) {
    zigbee_switch.toggle();
  }

  last_button_state = button_state;
  delay(50);
}
```

## ZigbeeContact

Header:

```cpp
#include <ZigbeeContact.h>
```

Creates an IAS Zone contact sensor endpoint. The contact state is reported to
the coordinator using the IAS Zone cluster.

If a Zone Status Change Notification fails to reach the CIE (for example during
a brief radio outage), the stack queues it and retries with backoff until the
CIE is reachable again. The local Zone Status attribute stays up to date either
way, so a coordinator attribute read can still recover the current state.

The raw state API follows IAS semantics: `false` means contact detected
(closed), and `true` means contact lost (open). The convenience `set_open()`,
`set_closed()`, `is_open()`, and `is_closed()` helpers are usually easier to
read in sketches.

API:

```cpp
bool begin();
void end();

void set_state(bool state);
bool get_state();

bool is_open();
void set_open();
bool is_closed();
void set_closed();

operator bool();
void operator=(bool state);
```

Example:

```cpp
#include <ZigbeeContact.h>
ZigbeeContact contact;

void setup()
{
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Zigbee.begin();
  contact.begin();
  contact.set_closed();
}

void loop()
{
  bool open = (digitalRead(BTN_BUILTIN) == LOW);

  if (open) {
    contact.set_open();
  } else {
    contact.set_closed();
  }

  digitalWrite(LED_BUILTIN, open ? LED_BUILTIN_ACTIVE : LED_BUILTIN_INACTIVE);
  delay(50);
}
```

## ZigbeeTemperatureSensor

Header:

```cpp
#include <ZigbeeTemperatureSensor.h>
```

Creates a Temperature Measurement endpoint.

Zigbee temperature values are represented as signed hundredths of a degree
Celsius. For example, `2350` means `23.50 C`. The convenience Celsius APIs do
the conversion for you. Non-finite values (`NaN` / `Inf`) are dropped (the
previous Measured Value is kept). Finite out-of-range values are clamped to
`-273.15 C` … `327.67 C` before writing.

`send_attribute_report()` queues the current Measured Value attribute for
transmission. `get_attribute_report_sent()` returns `true` once the Zigbee send
callback reports that the attribute report was sent successfully. This is
useful for end-device workflows that wake up, take a reading, report it, and go
back to sleep. `set_reporting_interval(min_interval_s, max_interval_s)` updates
the Zigbee reporting configuration for the temperature Measured Value attribute
on this endpoint. Call it after `begin()` and after the device has joined the
network.

API:

```cpp
bool begin();
void end();

void set_measured_value(int16_t value);
void set_measured_value_celsius(float celsius);
bool set_reporting_interval(uint16_t min_interval_s, uint16_t max_interval_s);
bool send_attribute_report();
bool get_attribute_report_sent();
int16_t get_measured_value();
float get_measured_value_celsius();
void set_min_value(int16_t value);
void set_max_value(int16_t value);

operator float();
void operator=(float celsius);
```

Example:

```cpp
#include <ZigbeeTemperatureSensor.h>
ZigbeeTemperatureSensor temperature;

void setup()
{
  Zigbee.begin();
  temperature.begin();
  temperature.set_min_value(-4000);  // -40.00 C
  temperature.set_max_value(12500);  // 125.00 C
}

void loop()
{
  temperature.set_measured_value_celsius(getCPUTemp());
  delay(2000);
}
```

## ZigbeeThermostat

Header:

```cpp
#include <ZigbeeThermostat.h>
```

Creates a heat-only Thermostat endpoint. Local temperature, heating setpoint,
and heating limits use signed hundredths of a degree Celsius in the raw API.
For example, `2150` means `21.50 C`. The convenience Celsius APIs do the
conversion for you.

The system mode API exposes `OFF` and `HEAT`.
Remote writes to Occupied Heating Setpoint, Min Heat Setpoint Limit,
Max Heat Setpoint Limit, and System Mode update the local object, and local
calls write the Zigbee Thermostat server attributes. If a new limit excludes
the current heating setpoint, the setpoint is clamped into the new range.
The HVAC action API reports whether the thermostat is actually idle or actively
heating using the Zigbee Thermostat Running Mode and Running State attributes.

API:

```cpp
enum thermostat_mode_t {
  OFF = 0,
  HEAT = 4
};

enum thermostat_hvac_action_t {
  IDLE = 0,
  HEATING = 1
};

bool begin();
void end();

int16_t get_local_temperature_raw();
void set_local_temperature_raw(int16_t local_temp);
int16_t get_heating_setpoint_raw();
void set_heating_setpoint_raw(int16_t heating_setpoint);

float get_local_temperature();
void set_local_temperature(float local_temp);
float get_heating_setpoint();
void set_heating_setpoint(float heating_setpoint);

void set_absolute_minimum_heating_setpoint(float abs_min_heating_setpoint);
void set_minimum_heating_setpoint(float min_heating_setpoint);
void set_absolute_maximum_heating_setpoint(float abs_max_heating_setpoint);
void set_maximum_heating_setpoint(float max_heating_setpoint);
float get_absolute_minimum_heating_setpoint();
float get_minimum_heating_setpoint();
float get_absolute_maximum_heating_setpoint();
float get_maximum_heating_setpoint();

thermostat_mode_t get_system_mode();
void set_system_mode(thermostat_mode_t system_mode);
thermostat_hvac_action_t get_hvac_action();
void set_hvac_action(thermostat_hvac_action_t hvac_action);
bool send_attribute_report();
```

Example:

```cpp
#include <ZigbeeThermostat.h>
ZigbeeThermostat thermostat;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Zigbee.begin();
  thermostat.begin();
  thermostat.set_local_temperature(21.5f);
  thermostat.set_heating_setpoint(23.0f);
  thermostat.set_system_mode(ZigbeeThermostat::HEAT);
  thermostat.send_attribute_report();
}

void loop()
{
  ZigbeeThermostat::thermostat_hvac_action_t action = ZigbeeThermostat::IDLE;
  if (thermostat.get_system_mode() == ZigbeeThermostat::HEAT && thermostat.get_local_temperature() < thermostat.get_heating_setpoint()) {
    action = ZigbeeThermostat::HEATING;
  }
  thermostat.set_hvac_action(action);
  digitalWrite(LED_BUILTIN, action == ZigbeeThermostat::HEATING ? LED_BUILTIN_ACTIVE : LED_BUILTIN_INACTIVE);
  delay(50);
}
```

## ZigbeePowerSource

Header:

```cpp
#include <ZigbeePowerSource.h>
```

Creates a standalone Power Configuration endpoint that exposes battery
percentage data.

The public API uses regular 0-100 percent values. Zigbee stores battery
percentage in half-percent units internally, so the library handles that
conversion for you.

`send_attribute_report()` queues the current Battery Percentage Remaining
attribute for transmission. `get_attribute_report_sent()` returns `true` once
the Zigbee send callback reports success. `set_reporting_interval(min_interval_s,
max_interval_s)` updates the Zigbee reporting configuration for the battery
percentage attribute in seconds.

API:

```cpp
bool begin();
void end();

void set_battery_percent(uint8_t value);
bool send_attribute_report();
bool get_attribute_report_sent();
bool set_reporting_interval(uint16_t min_interval_s, uint16_t max_interval_s);
uint8_t get_battery_percent();

operator uint8_t();
void operator=(uint8_t value);
```

Example:

```cpp
#include <ZigbeePowerSource.h>
ZigbeePowerSource power_source;

void setup()
{
  Zigbee.begin();
  power_source.begin();
  power_source.set_battery_percent(100);
}

void loop()
{
  static uint8_t battery_percent = 100;

  power_source.set_battery_percent(battery_percent);
  if (battery_percent > 0) {
    battery_percent -= 5;
  } else {
    battery_percent = 100;
  }
  delay(10000);
}
```

## ZigbeeHumiditySensor

Header:

```cpp
#include <ZigbeeHumiditySensor.h>
```

Creates a Relative Humidity Measurement endpoint.

Zigbee humidity values are represented as unsigned hundredths of a percent. For
example, `5030` means `50.30%`. The convenience percent APIs do the conversion
for you. Non-finite values (`NaN` / `Inf`) are dropped (the previous Measured
Value is kept). Finite out-of-range values are clamped to `0%` … `100.00%`
before writing.

`send_attribute_report()` queues the current Measured Value attribute for
transmission. `get_attribute_report_sent()` returns `true` once the Zigbee send
callback reports that the attribute report was sent successfully.
`set_reporting_interval(min_interval_s, max_interval_s)` updates the Zigbee
reporting configuration for the humidity Measured Value attribute on this
endpoint. Call it after `begin()` and after the device has joined the network.

API:

```cpp
bool begin();
void end();

void set_measured_value_raw(uint16_t value);
void set_measured_value_percent(float percent);
bool set_reporting_interval(uint16_t min_interval_s, uint16_t max_interval_s);
bool send_attribute_report();
bool get_attribute_report_sent();
uint16_t get_measured_value_raw();
float get_measured_value_percent();
void set_min_value_raw(uint16_t value);
void set_max_value_raw(uint16_t value);

operator float();
void operator=(float percent);
```

Example:

```cpp
#include <ZigbeeHumiditySensor.h>
ZigbeeHumiditySensor humidity;

void setup()
{
  Zigbee.begin();
  humidity.begin();
  humidity.set_min_value_raw(0);      // 0.00%
  humidity.set_max_value_raw(10000);  // 100.00%
}

void loop()
{
  humidity.set_measured_value_percent(52.5f);
  delay(2000);
}
```

## ZigbeeLightSensor

Header:

```cpp
#include <ZigbeeLightSensor.h>
```

Creates an Illuminance Measurement endpoint.

Zigbee illuminance values are represented as a logarithmic raw value. The
convenience lux APIs convert to and from lux using the Zigbee Illuminance
Measurement formula.

`send_attribute_report()` queues the current Measured Value attribute for
transmission. `get_attribute_report_sent()` returns `true` once the Zigbee send
callback reports that the attribute report was sent successfully.
`set_reporting_interval(min_interval_s, max_interval_s)` updates the Zigbee
reporting configuration for the illuminance Measured Value attribute on this
endpoint. Call it after `begin()` and after the device has joined the network.

API:

```cpp
bool begin();
void end();

void set_measured_value_raw(uint16_t value);
void set_measured_value_lux(float lux);
bool set_reporting_interval(uint16_t min_interval_s, uint16_t max_interval_s);
bool send_attribute_report();
bool get_attribute_report_sent();
uint16_t get_measured_value_raw();
float get_measured_value_lux();
void set_min_value_raw(uint16_t value);
void set_min_value_lux(float lux);
void set_max_value_raw(uint16_t value);
void set_max_value_lux(float lux);
void set_light_sensor_type(LightSensorType type);

operator float();
void operator=(float lux);
```

Example:

```cpp
#include <ZigbeeLightSensor.h>
ZigbeeLightSensor light;

void setup()
{
  Zigbee.begin();
  light.begin();
  light.set_min_value_lux(1.0f);
  light.set_max_value_lux(100000.0f);
}

void loop()
{
  light.set_measured_value_lux(250.0f);
  delay(2000);
}
```

## ZigbeeTimeClient

Header:

```cpp
#include <ZigbeeTimeClient.h>
```

Creates a dedicated Time Client endpoint. Instantiate this class in
sketches that need the current date and time.

API:

```cpp
bool begin();
void end();

bool request_time(uint8_t coordinator_endpoint_id = 1);
bool has_time();
uint32_t get_zigbee_time();
uint32_t get_unix_time();
uint32_t get_local_unix_time();
bool has_timezone();
int32_t get_timezone();
uint8_t get_time_status();
void set_time_update_callback(void (*cb)(void));
```

`request_time()` reads the Time cluster from the coordinator endpoint. The
default coordinator endpoint is `1`.

`get_zigbee_time()` returns ZCL time in seconds since 2000-01-01 00:00:00 UTC.
`get_unix_time()` returns UTC Unix time in seconds since 1970-01-01 00:00:00 UTC.
`get_local_unix_time()` returns the coordinator's local Unix time if local time or
timezone data was provided, otherwise it returns UTC Unix time.

Example:

```cpp
#include <ZigbeeTimeClient.h>
ZigbeeTimeClient zigbee_time;

void setup()
{
  zigbee_time.set_time_update_callback(onZigbeeTimeUpdated);
  Zigbee.begin();
  zigbee_time.begin();
}

void loop()
{
  if (Zigbee.isConnectedToNetwork()) {
    zigbee_time.request_time();
  }
  delay(10000);
}
```

## Global Zigbee API

The global `Zigbee` object manages the Zigbee stack, network state, metadata, and
dynamic endpoint allocation.

```cpp
enum ZigbeeDeviceType {
  ZIGBEE_DEVICE_TYPE_ROUTER = 0,
  ZIGBEE_DEVICE_TYPE_END_DEVICE = 1
};

enum ZigbeePowerSourceType {
  ZIGBEE_POWER_SOURCE_TYPE_MAINS = 0,
  ZIGBEE_POWER_SOURCE_TYPE_BATTERY = 1
};

void begin();

void setVendorName(const char* name);
void setProductName(const char* name);
void setFirmwareVersion(const char* version);
void setFirmwareVersion(uint32_t file_version);
bool setPairingChannel(uint8_t channel);
bool setDeviceType(ZigbeeDeviceType device_type);
ZigbeeDeviceType getDeviceType();
bool setPowerSource(ZigbeePowerSourceType power_source);
ZigbeePowerSourceType getPowerSource();

bool isPaired();
bool isConnectedToNetwork();
uint8_t getChannel();
uint16_t getPanId();
uint16_t getNodeId();

void leaveNetwork();
void factoryReset();
```

Network status helpers:

- `isPaired()` returns whether the device has Zigbee network information stored.
- `isConnectedToNetwork()` returns whether the stack is joined to a Zigbee network.
- `getChannel()` returns the current radio channel.
- `getPanId()` returns the current PAN ID.
- `getNodeId()` returns the current Zigbee node ID.

Network management helpers:

- `leaveNetwork()` requests a Zigbee network leave when currently joined.
- `factoryReset()` leaves the network, clears stored Zigbee network/token data, and resets.

Device type helpers:

- `setDeviceType()` selects whether the device joins as a router or end device.
  Call it before `begin()`. The default is `ZIGBEE_DEVICE_TYPE_ROUTER`.
- `getDeviceType()` returns the currently configured join device type.

Power source helpers:

- `setPowerSource()` selects whether the device advertises itself as mains or
  battery powered. Call it before `begin()` for the cleanest coordinator
  interview. The default is `ZIGBEE_POWER_SOURCE_TYPE_MAINS`. Home Assistant
  ZHA creates battery entities from that first interview, so changing this for
  an already paired device usually requires a factory reset, removing the old
  ZHA device entry, and pairing again.
- `getPowerSource()` returns the currently configured advertised power source.

## Home Assistant Notes

The examples are commonly tested with Home Assistant ZHA:

1. In Home Assistant, open **Settings > Devices & services > Zigbee Home Automation**.
2. Choose **Add device** to permit joining.
3. Reset or power the Arduino board running a Zigbee sketch.
4. Wait for the device to appear.

For the switch example, pairing only adds the switch device. To make it control a
light directly, add a Zigbee binding from the switch On/Off cluster to the target
light. The exact UI depends on your Home Assistant and ZHA version.

If a firmware, model, or manufacturer value does not update immediately after
you change a sketch, remove the old device from Home Assistant and pair it again,
or use the coordinator's reconfigure/read-attributes tools where available.

## Troubleshooting

### The sketch does not compile

Make sure the board is Arduino Nano Matter and the selected protocol stack is
Zigbee. `Matter`, `BLE`, and `None` protocol stack variants cannot compile sketches
that include `Zigbee.h`.

### The device does not appear in the coordinator

Check that the coordinator is in permit-join mode. If the board was paired to a
different network before, factory reset it by holding the built-in button during
boot in one of the examples, or call `Zigbee.factoryReset()` from your sketch.

If you restricted the pairing channel, make sure it matches the coordinator's
Zigbee channel.

Be patient during pairing - sometimes it can take up to several minutes depending on your network setup.

### The device reboots repeatedly while trying to pair

If joining gets far enough to store a network key but trust-center link-key
setup fails (for example against a polluted or stale coordinator network), the
library clears Zigbee network credentials and reboots so steering can start
cleanly again. A reboot loop usually means the coordinator side still has bad
state for this device - form a fresh Zigbee network, or remove the old device
entry and call `Zigbee.factoryReset()` after fixing the host.

### The device rejoins an old network after reflash

This is expected. Zigbee network credentials are stored in non-volatile memory
and survive sketch upload. Use `Zigbee.factoryReset()` when you want the board to
forget the old network.

### The switch pairs but does not control a light

Create a Zigbee binding from the switch's On/Off cluster to the light you want to
control. The switch sends commands to configured bindings.

### Home Assistant shows stale metadata

Many coordinators cache Basic cluster values. Remove and re-pair the device, or
force a reconfigure/read of the device from the coordinator.

## Library scope

This library is an Arduino-friendly wrapper around Silicon Labs' Zigbee
stack. It is intentionally smaller than the full Silicon Labs Zigbee SDK.
It exposes common smart-home device types and selected cluster
attributes. For advanced Zigbee clusters, custom device types, or changes to the
generated endpoint layout, you can use Simplicity Studio to build your own custom fine-tuned Zigbee applications.
