#ifndef PLATFORM_CHIPDEVICEBUILDCONFIG_H_
#define PLATFORM_CHIPDEVICEBUILDCONFIG_H_

// stack lock tracking config options
#define SL_MATTER_STACK_LOCK_TRACKING_NONE      0
#define SL_MATTER_STACK_LOCK_TRACKING_LOG       1
#define SL_MATTER_STACK_LOCK_TRACKING_FATAL     2

#include "sl_matter_config.h"

// inferred options
#include "sl_component_catalog.h"

// TODO: infer from wifi stack component
#define SL_MATTER_ENABLE_WIFI               0

// TODO: infer from OTA requestor component (split from generated matter efr32 platform component)
#define SL_MATTER_ENABLE_OTA                1

#if defined(SL_CATALOG_OPENTHREAD_STACK_PRESENT) || SL_OPENTHREAD_CERT_LIB
#define CHIP_ENABLE_OPENTHREAD                  1
#if defined(OPENTHREAD_FTD)
#define CHIP_DEVICE_CONFIG_THREAD_FTD           1
#else
#define CHIP_DEVICE_CONFIG_THREAD_FTD           0
#endif
#endif

#define CHIP_DEVICE_CONFIG_ENABLE_WPA           SL_MATTER_ENABLE_WIFI
#define CHIP_WITH_GIO                           SL_MATTER_ENABLE_WIFI
#define OPENTHREAD_CONFIG_ENABLE_TOBLE          0
//#define CHIP_STACK_LOCK_TRACKING_ENABLED        (SL_MATTER_STACK_LOCK_TRACKING_MODE != SL_MATTER_STACK_LOCK_TRACKING_NONE)
#define CHIP_STACK_LOCK_TRACKING_ERROR_FATAL    (SL_MATTER_STACK_LOCK_TRACKING_MODE == SL_MATTER_STACK_LOCK_TRACKING_FATAL)
#define CHIP_DEVICE_CONFIG_RUN_AS_ROOT          1
#define CHIP_DISABLE_PLATFORM_KVS               0
#define CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR SL_MATTER_ENABLE_OTA
#define CHIP_DEVICE_PROJECT_CONFIG_INCLUDE      <CHIPProjectConfig.h>
#define CHIP_DEVICE_PLATFORM_CONFIG_INCLUDE     <platform/silabs/CHIPDevicePlatformConfig.h>
#define CHIP_DEVICE_LAYER_TARGET_EFR32          1
#define CHIP_DEVICE_LAYER_TARGET                silabs

// Enable default/generic test-mode CommissionableDataProvider in GenericConfigurationManagerImpl
// === FOR TRANSITION UNTIL ALL EXAMPLES PROVIDE THEIR OWN ===
#define CHIP_USE_TRANSITIONAL_COMMISSIONABLE_DATA_PROVIDER  0

#endif // PLATFORM_CHIPDEVICEBUILDCONFIG_H_
