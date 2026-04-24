/*
   BTHome v2 switch example

   BTHome homepage: https://bthome.io/
   This example implements a BTHome client which transmits button press events to a BTHome server device.

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

#include "bthome_v2.h"

static bool encryption = true;

static uint8_t device_name[] = "BTSwitch";
static uint8_t device_key[] = "11112222333344445555666677778888";

// Variables, flags, and timestamps for button logic
int pulseButtonState;
int lastButtonState = LOW;

uint32_t lastDebounceTime = 0u;
uint32_t debounceDelay = 50u;

uint32_t button_press_previous_timestamp = 0u;
uint32_t button_measure_interval = 2000u;
uint8_t button_press_count = 0u;

int longButtonState = 0;
int longLastButtonState = 0;
uint32_t startPressed = 0u;
uint32_t endPressed = 0u;
uint32_t holdTime = 0u;

// Local functions
void buttonPressToEvent(uint8_t number_of_button_press);
void print_BT_address();
void print_BTHome_key();

// If there's no built-in button set a pin where a button is connected
#ifndef BTN_BUILTIN
  #define BTN_BUILTIN D0
#endif

void setup()
{
  Serial.begin(115200);

  Serial.println("BTHome v2 - Switch Application ");
  bthome_v2_init(device_name, encryption, device_key, true);

  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  print_BT_address();

  if (encryption) {
    print_BTHome_key();
  }
}

void loop()
{
  // Logic for button press debounce and count button presses
  int reading = digitalRead(BTN_BUILTIN);
  if (reading != lastButtonState) {
    // Reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != pulseButtonState) {
      pulseButtonState = reading;

      if (pulseButtonState == LOW) {
        if (button_press_count < 3u) {
          Serial.print(".");
          button_press_count++;
        }
        button_press_previous_timestamp = lastDebounceTime;
      }
    }
  }
  lastButtonState = reading;

  uint32_t timestamp = millis();
  if (timestamp - button_press_previous_timestamp >= button_measure_interval) {
    button_press_previous_timestamp = timestamp;

    buttonPressToEvent(button_press_count);
    button_press_count = 0u;
  }

  // Logic for measuring long button press
  longButtonState = digitalRead(BTN_BUILTIN);

  if (longButtonState != longLastButtonState) {
    if (longButtonState == LOW) {
      startPressed = millis();
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      endPressed = millis();
      digitalWrite(LED_BUILTIN, LOW);
      holdTime = endPressed - startPressed;
      if (holdTime >= 250u && holdTime < button_measure_interval) {
        button_press_count = 0u;
        buttonPressToEvent(EVENT_BUTTON_LONG_PRESS);
      }
    }
  }
  longLastButtonState = longButtonState;
}

void buttonPressToEvent(uint8_t number_of_button_press)
{
  switch (number_of_button_press) {
    case 0:
      Serial.println("[BTHome]: Sending BUTTON_NONE event");
      bthome_v2_stop();
      break;

    case 1:
      Serial.println("[BTHome]: Sending EVENT_BUTTON_PRESS event");
      break;

    case 2:
      Serial.println("[BTHome]: Sending EVENT_BUTTON_DOUBLE_PRESS event");
      break;

    case 3:
      Serial.println("[BTHome]: Sending EVENT_BUTTON_TRIPLE_PRESS event");
      break;

    case 4:
      Serial.println("[BTHome]: Sending EVENT_BUTTON_LONG_PRESS event");
      break;

    default:
      break;
  }

  bthome_v2_reset_measurement();
  bthome_v2_add_measurement_state(EVENT_BUTTON, number_of_button_press, 0);
  bthome_v2_build_packet();
  bthome_v2_send_packet();
}

void sl_bt_on_event(sl_bt_msg_t *evt)
{
  bthome_v2_bt_on_event(evt);
  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      bthome_v2_add_measurement_state(EVENT_BUTTON, EVENT_BUTTON_NONE, 0);
      bthome_v2_send_packet();
      break;

    default:
      break;
  }
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
    Serial.print(device_key[i] - 48);
  }
  Serial.println(" ");
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
