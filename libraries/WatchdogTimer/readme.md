# 🐶 WatchdogTimer
*Watchdog timer* functionality for the *Silicon Labs Arduino Core*.

The watchdog timer can be used to reset the MCU if it gets stuck or misses a timing deadline.
The timeout of the watchdog timer is configurable as well as the action on timer expiration.

The watchdog timer is not active by default.

## Usage

Include ```WatchdogTimer.h``` in your sketch to make the watchdog functionality available.

Check out the built-in watchdog timer examples under **File > Examples > Watchdog Timer >** which demonstrate the usage of the WDT.

## API

```void WatchdogTimer.begin();``` - starts the watchdog timer with the default timeout period of 1 second.

```void WatchdogTimer.begin(WatchdogPeriod timeout_period);``` - starts the watchdog timer with the specified timeout period.

The following periods can be specified:

```
WDOG_PERIOD_9_MS
WDOG_PERIOD_17_MS
WDOG_PERIOD_33_MS
WDOG_PERIOD_65_MS
WDOG_PERIOD_129_MS
WDOG_PERIOD_257_MS
WDOG_PERIOD_513_MS
WDOG_PERIOD_1_S
WDOG_PERIOD_2_S
WDOG_PERIOD_4_S
WDOG_PERIOD_8_S
WDOG_PERIOD_16_S
WDOG_PERIOD_32_S
WDOG_PERIOD_64_S
WDOG_PERIOD_128_S
WDOG_PERIOD_256_S
```

```void WatchdogTimer.end();``` - stops the watchdog timer.

```void WatchdogTimer.feed();``` - feeds the watchdog timer to prevent a reset and signal that the execution is not stuck.

```void WatchdogTimer.setTimeout(WatchdogPeriod timeout_period);``` - sets the timeout period of the watchdog timer. The same periods can be specified as mentioned above.

```void WatchdogTimer.attachInterrupt(void (*callback)(void));``` - attaches a callback function to the watchdog timer. When a callback function is attached, the watchdog timer will not reset the device when it overflows. Instead, the callback function will be called.

```void WatchdogTimer.detachInterrupt();``` - detaches the interrupt from the watchdog timer.

```bool WatchdogTimer.watchdogResetHappened();``` - returns true if the device was previously reset by the watchdog timer.

```void WatchdogTimer.setWatchdogOffWhileSleeping(bool off_while_sleeping);``` - sets the watchdog timer to be enabled/disabled while the device is in sleep mode. The watchdog timer is running in sleep mode by default. This function can be used to disable the watchdog timer while the device is in sleep mode to avoid resets when the device is sleeping for a longer period.
