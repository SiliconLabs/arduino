/*
   Hardware info

   This sketch will print various information about the hardware it's running on to the Serial Monitor.

   Compatible with all Silicon Labs Arduino boards.

   Author: Tamas Jozsi (Silicon Labs)
 */

void print_device_info();
String get_board_name();
String get_radio_stack_name();
String get_ai_ml_capable();

void setup()
{
  Serial.begin(115200);
  Serial.println(millis());
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  print_device_info();
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  delay(1000);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  delay(2000);
}

void print_device_info()
{
  Serial.print("Hardware: ");
  Serial.println(get_board_name());

  Serial.print("Radio stack: ");
  Serial.println(get_radio_stack_name());

  Serial.print("AI/ML capable: ");
  Serial.println(get_ai_ml_capable());

  Serial.print("Unique ID: ");
  Serial.println(getDeviceUniqueIdStr());

  Serial.print("CPU speed: ");
  float cpu_clock_mhz = (float)getCPUClock() / 1000000.0f;
  Serial.print(cpu_clock_mhz);
  Serial.println(" MHz");

  Serial.print("CPU temperature: ");
  Serial.print(getCPUTemp());
  Serial.println("°C");

  Serial.print("Heap usage: ");
  size_t total_heap = getTotalHeapSize();
  size_t used_heap = getUsedHeapSize();
  float heap_usage_percent = (float)used_heap * 100.0f / (float)total_heap;
  Serial.print(used_heap);
  Serial.print(" bytes / ");
  Serial.print(total_heap);
  Serial.print(" bytes (");
  Serial.print(heap_usage_percent);
  Serial.print("%)");
  Serial.println();

  Serial.print("Highest heap usage: ");
  size_t max_heap_usage = getHeapHighWatermark();
  float max_heap_usage_percent = (float)max_heap_usage * 100.0f / (float)total_heap;
  Serial.print(max_heap_usage);
  Serial.print(" bytes (");
  Serial.print(max_heap_usage_percent);
  Serial.print("%)");
  Serial.println();

  Serial.print("Core version: ");
  Serial.println(getCoreVersion());

  Serial.print("Uptime: ");
  unsigned long uptime = millis();
  Serial.print(uptime);
  Serial.print(" ms ");
  unsigned long up_hr = uptime / 3600000;
  unsigned long up_min = (uptime / 60000) % 60;
  unsigned long up_s = (uptime / 1000) % 60;
  Serial.printf("(%luh %lum %lus)", up_hr, up_min, up_s);
  Serial.println();

  Serial.println("------------------------------");
}

String get_board_name()
{
  silabs_board_t current_board = getCurrentBoardType();
  switch (current_board) {
    case BOARD_ARDUINO_NANO_MATTER:
      return "Arduino Nano Matter";
    case BOARD_SPARKFUN_THINGPLUS_MATTER:
      return "SparkFun ThingPlus Matter";
    case BOARD_SILABS_XG24_EXPLORER_KIT:
      return "Silicon Labs xG24 Explorer Kit";
    case BOARD_SILABS_XG24_DEV_KIT:
      return "Silicon Labs xG24 Dev Kit";
    case BOARD_SILABS_XG27_DEV_KIT:
      return "Silicon Labs xG27 Dev Kit";
    case BOARD_SILABS_BGM220_EXPLORER_KIT:
      return "Silicon Labs BGM220 Explorer Kit";
    case BOARD_EURIO_LYRA_24P_20_DBM_DEV_KIT:
      return "Ezurio Lyra 24P 20dBm Dev Kit";
    case BOARD_SEEED_STUDIO_XIAO_MG24:
      return "Seeed Studio XIAO MG24";
    default:
      return "unknown";
  }
}

String get_radio_stack_name()
{
  silabs_radio_stack_t radio_stack = getCurrentRadioStackType();
  switch (radio_stack) {
    case RADIO_STACK_NONE:
      return "None";
    case RADIO_STACK_BLE_SILABS:
      return "BLE (Silabs)";
    case RADIO_STACK_BLE_ARDUINO:
      return "BLE (Arduino)";
    case RADIO_STACK_MATTER:
      return "Matter";
    default:
      return "unknown";
  }
}

String get_ai_ml_capable()
{
  if (isBoardAiMlCapable()) {
    return "yes";
  }
  return "no";
}
