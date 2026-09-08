/*
   Matter door lock with PIN/RFID credentials example

   The example demonstrates how to use the Arduino Matter API to build a door lock that supports
   PIN and RFID credentials and Matter's User (USR) management, on top of the basic
   matter_door_lock example.

   - A Matter controller (Home Assistant, Apple Home, ...) can provision PIN codes or RFID/NFC
     tags through its native "add code"/"add tag" UI, using the standard SetUser/SetCredential
     commands. A keypad PIN sent along with a LockDoor/UnlockDoor command is validated against
     those credentials automatically, and reported back to the controller with the matching
     CredentialType/CredentialIndex.
   - The sketch can also provision a credential locally with set_credential(), e.g. to have a
     default PIN available on first boot.
   - Because this core does not include an NFC/RFID reader driver, presenting a physical RFID tag
     is simulated here over Serial: type the digits printed as the tag's "UID" to simulate a scan.
     On real hardware, replace read_simulated_rfid_uid() with code that reads the tag's UID from
     an NFC/RFID reader wired over Wire (I2C), then call report_credential_unlock() the same way.

   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)
 */
#include <Matter.h>
#include <MatterDoorLock.h>

MatterDoorLock matter_door_lock;

using CredentialTypeEnum = MatterDoorLock::CredentialTypeEnum;

// Default PIN provisioned locally at boot - credential index 1
static const uint8_t default_pin[] = { '1', '2', '3', '4' };
// Placeholder RFID tag UID provisioned locally at boot - credential index 1
static const uint8_t default_rfid_uid[] = { 0xDE, 0xAD, 0xBE, 0xEF };

void on_credential_changed(CredentialTypeEnum type, uint16_t index);
void read_simulated_rfid_uid();

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_door_lock.setFeatures({ MatterDoorLock::Feature::kPinCredential,
                                 MatterDoorLock::Feature::kRfidCredential,
                                 MatterDoorLock::Feature::kUser });
  matter_door_lock.begin();
  matter_door_lock.set_credential_changed_callback(on_credential_changed);

  Serial.println("Matter door lock with PIN/RFID credentials");

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
  while (!matter_door_lock.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

  // Provision a default PIN and RFID tag - a controller can still add more (or clear these) later
  matter_door_lock.set_credential(CredentialTypeEnum::kPin, 1, default_pin, sizeof(default_pin));
  matter_door_lock.set_credential(CredentialTypeEnum::kRfid, 1, default_rfid_uid, sizeof(default_rfid_uid));
  Serial.println("Default PIN '1234' and RFID tag DE:AD:BE:EF provisioned");
  Serial.println("Type 'deadbeef' and press enter to simulate presenting that RFID tag");
}

void loop()
{
  static bool locked_last = false;
  bool locked = matter_door_lock.is_locked();
  if (locked != locked_last) {
    locked_last = locked;
    Serial.println(locked ? "Locked" : "Unlocked");
  }

  read_simulated_rfid_uid();
}

// Called whenever a credential is provisioned or cleared, whether by a Matter controller or by
// this sketch's own set_credential()/clear_credential() calls.
void on_credential_changed(CredentialTypeEnum type, uint16_t index)
{
  Serial.printf("Credential changed: type=%d index=%d\n", (int)type, index);
}

// Reads a simulated RFID UID typed over Serial (e.g. "deadbeef") and, if it matches the
// provisioned tag, reports it as a physical credential presentation - unlocking the door and
// letting the controller see which credential authorized it.
//
// On real hardware, replace this with code that reads the UID from an NFC/RFID reader wired over
// Wire (I2C), compares it against what was provisioned, and calls report_credential_unlock() with
// the matching credential index the same way.
void read_simulated_rfid_uid()
{
  if (!Serial.available()) {
    return;
  }
  String line = Serial.readStringUntil('\n');
  line.trim();
  if (line.equalsIgnoreCase("deadbeef")) {
    Serial.println("Simulated RFID tag matched provisioned credential - unlocking");
    matter_door_lock.report_credential_unlock(CredentialTypeEnum::kRfid, 1);
  } else if (line.length() > 0) {
    Serial.println("Simulated RFID tag not recognized");
  }
}
