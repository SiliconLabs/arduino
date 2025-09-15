#ifndef CORE_CHIPBUILDCONFIG_H_
#define CORE_CHIPBUILDCONFIG_H_

#define SL_MATTER_LOG_NONE          0
#define SL_MATTER_LOG_ERROR         1
#define SL_MATTER_LOG_PROGRESS      3
#define SL_MATTER_LOG_DETAIL        4
#define SL_MATTER_LOG_AUTOMATION    5

#include "sl_matter_config.h"

#define CHIP_FUZZING_ENABLED                0
#define CHIP_CONFIG_TEST                    0
#define CHIP_ERROR_LOGGING                  (SL_MATTER_LOG_LEVEL >= SL_MATTER_LOG_ERROR)
#define CHIP_PROGRESS_LOGGING               (SL_MATTER_LOG_LEVEL >= SL_MATTER_LOG_PROGRESS)
#define CHIP_DETAIL_LOGGING                 (SL_MATTER_LOG_LEVEL >= SL_MATTER_LOG_DETAIL)
#define CHIP_AUTOMATION_LOGGING             (SL_MATTER_LOG_LEVEL >= SL_MATTER_LOG_AUTOMATION)
#define CHIP_CONFIG_LOG_MESSAGE_MAX_SIZE    256
#define CHIP_PW_TOKENIZER_LOGGING           0
#define CHIP_USE_PW_LOGGING                 0
#define CHIP_CONFIG_SHORT_ERROR_STR         1 // always 1 on embedded
#define CHIP_CONFIG_ENABLE_ARG_PARSER       SL_MATTER_CLI_ARG_PARSER

#define CHIP_TARGET_STYLE_UNIX              0
#define CHIP_TARGET_STYLE_EMBEDDED          1

#define CHIP_CONFIG_MEMORY_MGMT_MALLOC      0
#define HAVE_MALLOC                         0
#define HAVE_FREE                           0
#define HAVE_NEW                            0
#define CHIP_CONFIG_MEMORY_MGMT_PLATFORM    1 // always use "platform" allocator
#define CHIP_CONFIG_MEMORY_DEBUG_CHECKS     0
#define CHIP_CONFIG_MEMORY_DEBUG_DMALLOC    0

#define CHIP_CONFIG_PROVIDE_OBSOLESCENT_INTERFACES              0
#define CHIP_CONFIG_TRANSPORT_TRACE_ENABLED                     0
#define CHIP_CONFIG_TRANSPORT_PW_TRACE_ENABLED                  0
#define CHIP_CONFIG_MINMDNS_DYNAMIC_OPERATIONAL_RESPONDER_LIST  0
#define CHIP_CONFIG_MINMDNS_MAX_PARALLEL_RESOLVES               2

#endif  // CORE_CHIPBUILDCONFIG_H_
