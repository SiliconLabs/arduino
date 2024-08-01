# Arduino Low Power library for Silicon Labs boards

This library allows the use of the low power features of the EFR32 MCU. This means your battery powered projects will have a longer battery life.

Read more about the EFR32 energy modes here: https://www.silabs.com/mcu/32-bit-microcontrollers/efm32-energy-modes

## ⚠️ Flashing while in deep sleep

When in deep sleep the devices will turn off most of their peripherals - including their debug/flashing interfaces.

This means that **when the device is deep sleeping it's not possible to flash or erase them**.

On Silicon Labs boards the board controller will take care of this by issuing a reset after the first failed communication attempt.

On the Nano Matter an escape method is utilized when using this library to force and hold the device in an awake state before any user code can run.

To enter this escape mode on your Nano Matter do the following:

 - Press and hold the user button
 - Press and release the reset button
 - Wait for the onboard LED to be continuously lit
 - Release the user button
 - Your board will now stay awake and can be reflashed

## Methods

## `LowPower.idle(milliseconds)`

#### Description

Requests EM1/EM2 mode for the specified amount of time.
The device is not guaranteed to be in the requested energy mode during for the entire specified period - as the radio stacks and other peripherals can and will periodically wake the device to perform their necessary tasks - then put the device back to sleep.


#### Syntax

```
LowPower.idle(milliseconds);
```

#### Parameters

milliseconds: the number of milliseconds to put the board in idle mode.

## `LowPower.sleep(milliseconds)`

#### Description

Requests EM1/EM2 mode for the specified amount of time.
The device is not guaranteed to be in the requested energy mode during for the entire specified period - as the radio stacks and other peripherals can and will periodically wake the device to perform their necessary tasks - then put the device back to sleep.

#### Syntax

```
LowPower.sleep(milliseconds);
```

#### Parameters

milliseconds: the number of milliseconds to put the board in sleep mode.

## `LowPower.deepSleep()`

#### Description

Puts the MCU in deep sleep mode. The deep sleep mode allows the device to an enter extremely low power consumption state (EM4). Every peripheral is stopped except for a few - like the back-up RTC and back-up RAM. The CPU can only wake up using the RTC or a pulse on a wake-up interrupt capable pin.
In this mode the CPU is completely halted and RAM contents are lost. Upon waking up the device starts from the beginning of the sketch.


#### Syntax

```
LowPower.deepSleep();
LowPower.deepSleep(milliseconds);
```

#### Parameters

milliseconds: the number of milliseconds to put the board in deep sleep mode. If void the deep sleep mode is used till a wake-up event.

## `LowPower.attachInterruptWakeup()`

#### Description

Attaches a function to call on wake-up and sets the deep sleep wake-up pins.


#### Syntax

```
LowPower.attachInterruptWakeup(pin, callback, mode);
```

#### Parameters

pin: the pin used as external wakeup

callback: the function to call on wakeup

mode: the transitions to sense on the indicated pin. Can be one between:

- FALLING
- RISING


## `LowPower.wokeUpFromDeepSleep()`

#### Description

Returns 'true' if the MCU has woken up after a deep sleep.

Returns 'false' if the MCU was reset by another cause.


#### Syntax

```
LowPower.wokeUpFromDeepSleep();
```

#### Parameters

None


## `LowPower.deepSleepMemoryWrite(address, data)`

#### Description

Writes 4 bytes (uint32_t) to the deep sleep memory (Back-up RAM) to the specified address.
Contents of the deep sleep memory are retained during deep sleep.


#### Syntax

```
LowPower.deepSleepMemoryRead(address, data);
```

#### Parameters

address: address to write

data: data to write


## `LowPower.deepSleepMemoryRead(address)`

#### Description

Reads 4 bytes (uint32_t) from the deep sleep memory (Back-up RAM) from the specified address.


#### Syntax

```
uint32_t data_read = LowPower.deepSleepMemoryRead(address);
```

#### Parameters

None


## `LowPower.deepSleepMemorySize()`

#### Description

Reads the size of the deep sleep memory (in writeable addresses).
To get the size in bytes multiple the return value by 4.


#### Syntax

```
uint32_t deep_sleep_memory_size = LowPower.deepSleepMemorySize();
```

#### Parameters

None