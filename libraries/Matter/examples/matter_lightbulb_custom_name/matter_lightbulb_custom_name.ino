/*
   Matter lightbulb with custom properties

   This example show how custom properties like 'device name', 'manufacturer name', product name' and 'serial number'
   can be set to customize Matter devices. The example creates a simple on/off lightbulb,
   then sets the mentioned properties to a custom value with the Arduino Matter API.
   These properties can be changed on all Matter devices - this example just focuses on a single lightbulb.

   NOTE: some Matter ecosystems may ignore or choose not to display the values set here.

   The example lets users control the onboard LED through Matter.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Matter.h>
#include <MatterLightbulb.h>

MatterLightbulb matter_bulb;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_bulb.begin();

  // Initial display name that can be changed by the user in the client apps
  matter_bulb.set_device_name("Chell's bulb");
  // Vendor/manufacturer name - cannot be changed by users
  matter_bulb.set_vendor_name("Aperture Science");
  // Product/model name - cannot be changed by users
  matter_bulb.set_product_name("Weighted Storage Bulb");
  // Serial number - cannot be changed by users
  // Set the serial number to the chip's unique ID
  matter_bulb.set_serial_number(getDeviceUniqueIdStr().c_str());

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  Serial.println("Matter lightbulb");

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
  }

  Serial.println("Waiting for Thread network...");
  while (!Matter.isDeviceThreadConnected()) {
    delay(200);
  }
  Serial.println("Connected to Thread network");

  Serial.println("Waiting for Matter device discovery...");
  while (!matter_bulb.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_bulb.get_onoff();

  // If the current state is ON and the previous was OFF - turn on the LED
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    Serial.println("Bulb ON");
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    Serial.println("Bulb OFF");
  }
}
