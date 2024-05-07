#ifndef BLE_BLEBUILDCONFIG_H_
#define BLE_BLEBUILDCONFIG_H_

#include "sl_component_catalog.h"

#if defined(SL_CATALOG_MATTER_BLE_PRESENT)
#define CONFIG_NETWORK_LAYER_BLE        1
#else
#define CONFIG_NETWORK_LAYER_BLE        0
#endif
#define CHIP_ENABLE_CHIPOBLE_TEST       0
#define BLE_PROJECT_CONFIG_INCLUDE      <CHIPProjectConfig.h>
#define BLE_PLATFORM_CONFIG_INCLUDE     <platform/silabs/BlePlatformConfig.h>

#endif  // BLE_BLEBUILDCONFIG_H_
