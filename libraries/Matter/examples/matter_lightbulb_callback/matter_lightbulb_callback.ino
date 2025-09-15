/*
   Matter lightbulb example utilizing the device change callback

   The example shows how to create a simple on/off lightbulb using the Matter device change callback and semaphores.
   The Silicon Labs Arduino Core runs on FreeRTOS. In this example we utilize task synchronization with semaphores to
   effectively handles changes in a Matter lightbulb.
   Using this method allows the device to go to a low power mode and only react when something changes.

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

StaticSemaphore_t matter_device_event_semaphore_buf;
SemaphoreHandle_t matter_device_event_semaphore;

void matter_bulb_change_callback();

void setup()
{
  // Create a binary semaphore
  matter_device_event_semaphore = xSemaphoreCreateBinaryStatic(&matter_device_event_semaphore_buf);

  Serial.begin(115200);
  Matter.begin();
  matter_bulb.begin();
  // Register the device change callback function
  matter_bulb.set_device_change_callback(matter_bulb_change_callback);

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
  // Try to take the 'device_event' semaphore - the user task will be blocked here until the
  // 'matter_bulb_change_callback' gives the semaphore - allowing the device to go to a low power mode
  // and only react when something changes
  xSemaphoreTake(matter_device_event_semaphore, portMAX_DELAY);
  Serial.println("Matter bulb property changed");

  if (matter_bulb.get_onoff()) {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    Serial.println("Bulb ON");
  } else {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    Serial.println("Bulb OFF");
  }
}

// This callback will be called from the Matter stack's task
void matter_bulb_change_callback()
{
  // Giving the semaphore will unblock the user task and allow it to react to changes
  xSemaphoreGive(matter_device_event_semaphore);
}
