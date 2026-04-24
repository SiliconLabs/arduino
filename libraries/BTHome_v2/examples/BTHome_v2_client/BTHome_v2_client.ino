/*
   BTHome v2 client example

   This example is a basic implementation of a BTHome client device.
   Sends mock temperature and humidity values and a button press event.
   BTHome homepage: https://bthome.io/

   This example only works with the 'BLE (Silabs)' protocol stack variant.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG27 DevKit
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - BGM220 Explorer Kit

   Author: Peter Jolankai (Silicon Labs)
 */

#include <bthome_v2.h>

static const bool encryption = true;

bool btn_notification_enabled = false;
volatile bool btn_state_changed = false;
uint8_t btn_state = LOW;
static void btn_state_change_callback();
void print_BT_address();

static uint8_t name[] = "BTHome_client";
static uint8_t key[] = "11112222333344445555666677778888";

void setup()
{
  Serial.begin(115200);
  Serial.println("Silicon Labs BTHome v2 client example");

// If the board has a built-in button configure it for usage
#ifdef BTN_BUILTIN
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, btn_state_change_callback, CHANGE);
#else   // BTN_BUILTIN
  // Avoid warning for unused function on boards without a button
  (void)btn_state_change_callback;
#endif  // BTN_BUILTIN

  // Init BTHome functions and send first packet
  bthome_v2_init(name, encryption, key, false);
  bthome_v2_add_measurement_float(ID_TEMPERATURE_PRECISE, 25.06f); // Mock value
  bthome_v2_add_measurement_float(ID_PRESSURE, 1023.86f); // Mock value
  bthome_v2_send_packet();

  print_BT_address();
  if (encryption) {
    print_BTHome_key();
  }
}

void loop()
{
  const uint32_t MEASUREMENT_INTERVAL_MS = 1000u;
  static uint32_t previousMillis = 0u;

  uint32_t currentMillis = millis();
  if (currentMillis - previousMillis >= MEASUREMENT_INTERVAL_MS) {
    previousMillis = currentMillis;

    int button_press_type = EVENT_BUTTON_NONE;
    if (btn_state_changed) {
      button_press_type = EVENT_BUTTON_PRESS;
      btn_state_changed = false;
      Serial.println("Sending button press BTHome event");
    }

    bthome_v2_reset_measurement();
    bthome_v2_add_measurement_state(EVENT_BUTTON, button_press_type, 0);
    bthome_v2_build_packet();
    bthome_v2_send_packet();
  }
}

void sl_bt_on_event(sl_bt_msg_t *evt)
{
  bthome_v2_bt_on_event(evt);

  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_system_boot_id:
      break;

    default:
      break;
  }
}

// Button change callback
static void btn_state_change_callback()
{
// If the board has a built-in button
#ifdef BTN_BUILTIN
  // The real button state is inverted - most boards have an active low button configuration
  btn_state = !digitalRead(BTN_BUILTIN);
  btn_state_changed = true;
#endif  // BTN_BUILTIN
}

void print_BT_address()
{
  bd_addr address;
  uint8_t address_type;

  sl_bt_system_get_identity_address(&address, &address_type);
  Serial.print("Bluetooth ");
  Serial.print(address_type ? "static random" : "public device");
  Serial.print(" address: ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X",
                address.addr[5], address.addr[4], address.addr[3],
                address.addr[2], address.addr[1], address.addr[0]);
  Serial.println();
}

void print_BTHome_key()
{
  Serial.print("BTHome key: ");
  for (uint8_t i = 0; i < 32; i++) {
    Serial.print(key[i] - 48);
  }
  Serial.println(" ");
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
