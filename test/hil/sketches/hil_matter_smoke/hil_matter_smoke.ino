#include <Matter.h>
#include <MatterLightbulb.h>

MatterLightbulb matter_bulb;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);

  Matter.begin();
  matter_bulb.begin();

  Serial.println("Matter smoke test");

  matter_bulb.set_onoff(false);
  matter_bulb.set_onoff(true);
}

void loop()
{
  Serial.print("Matter smoke test running!");
  Serial.print(" uptime: ");
  Serial.println(millis());

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }

  delay(1000);
}
