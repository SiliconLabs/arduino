/*
   Matter Time Synchronization example

   The example shows how to receive UTC time (and timezone / DST offsets) from a
   Matter controller and read them as Unix timestamps using the Arduino Matter API.

   The Time Synchronization cluster is enabled on the root endpoint. After the
   device is commissioned, a Matter hub that supports time sync (or chip-tool
   SetUTCTime / SetTimeZone / SetDSTOffset) can set the device clock. The sketch
   waits until time is available, then prints UTC and local timestamps.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus Matter
   - Silicon Labs xG24 Explorer Kit
   - Silicon Labs xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Matter.h>
#include <MatterTemperature.h>
#include <MatterTimeSynchronization.h>

MatterTemperature matter_temp_sensor;
MatterTimeSynchronization matter_time;

void print_date_time(const char* label, uint32_t unix_time);

void on_time_updated()
{
  Serial.println("Matter time updated");
}

void on_timezone_updated()
{
  Serial.println("Matter timezone updated");
}

void setup()
{
  Serial.begin(115200);
  Matter.set_product_name("Matter temperature sensor");
  Matter.begin();
  matter_temp_sensor.begin();
  matter_time.begin();
  matter_time.set_time_update_callback(on_time_updated);
  matter_time.set_timezone_update_callback(on_timezone_updated);

  Serial.println("Matter Time Synchronization");

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
  while (!matter_temp_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

  matter_time.request_time();
  Serial.println("Waiting for Matter controller time synchronization...");
  uint32_t time_request_cnt = 0u;
  while (!matter_time.has_time()) {
    delay(1000);
    time_request_cnt++;
    if (time_request_cnt % 60 == 0) {
      Serial.println("Re-requesting time from controller...");
      matter_time.request_time();
    }
  }
  Serial.printf("Time synchronized - Unix UTC: %lu\n", matter_time.get_unix_time());
  print_date_time("UTC time", matter_time.get_unix_time());
  if (matter_time.has_timezone()) {
    Serial.printf("Local offset: %ld s\n", matter_time.get_local_offset_seconds());
    print_date_time("Local time", matter_time.get_local_unix_time());
  }
  Serial.println("-----");
}

void loop()
{
  float current_cpu_temp = getCPUTemp();
  matter_temp_sensor.set_measured_value_celsius(current_cpu_temp);

  if (!matter_time.has_time()) {
    delay(1000);
    return;
  }

  if (matter_time.has_timezone()) {
    Serial.printf("Local Unix time: %lu | CPU temp: %.02f C\n", matter_time.get_local_unix_time(), current_cpu_temp);
    print_date_time("Local time", matter_time.get_local_unix_time());
  } else {
    Serial.printf("UTC Unix time: %lu | CPU temp: %.02f C\n", matter_time.get_unix_time(), current_cpu_temp);
    print_date_time("UTC time", matter_time.get_unix_time());
  }
  Serial.println("-----");
  delay(1000);
}

void print_date_time(const char* label, uint32_t unix_time)
{
  time_t timestamp = static_cast<time_t>(unix_time);
  tm* time_info = gmtime(&timestamp);
  char formatted_time[32];

  if (time_info == nullptr || strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S", time_info) == 0) {
    return;
  }

  Serial.print(label);
  Serial.print(": ");
  Serial.println(formatted_time);
}
