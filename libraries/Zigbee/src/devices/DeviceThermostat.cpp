/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2026 Silicon Laboratories Inc. www.silabs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "DeviceThermostat.h"
#include "../ZigbeeAfLock.h"

extern "C" {
#include "af.h"
}

static int16_t ReadInt16(const uint8_t* value)
{
  uint16_t raw = static_cast<uint16_t>(value[0]) | (static_cast<uint16_t>(value[1]) << 8);
  return static_cast<int16_t>(raw);
}

namespace {
static constexpr uint8_t kMaxDeferredThermostatReportSlots = 3;
static constexpr uint32_t kDeferredThermostatReportDelayMs = 100;

struct DeferredThermostatReportSlot {
  DeviceThermostat* device;
  sl_zigbee_af_event_t event;
  bool initialized;
  uint16_t report_mask;
};

static DeferredThermostatReportSlot deferred_thermostat_report_slots[kMaxDeferredThermostatReportSlots] = {};

DeferredThermostatReportSlot* FindDeferredThermostatReportSlot(DeviceThermostat* device, bool allocate)
{
  for (uint8_t i = 0; i < kMaxDeferredThermostatReportSlots; i++) {
    if (deferred_thermostat_report_slots[i].device == device) {
      return &deferred_thermostat_report_slots[i];
    }
  }

  if (!allocate) {
    return nullptr;
  }

  for (uint8_t i = 0; i < kMaxDeferredThermostatReportSlots; i++) {
    if (deferred_thermostat_report_slots[i].device == nullptr) {
      deferred_thermostat_report_slots[i].device = device;
      return &deferred_thermostat_report_slots[i];
    }
  }

  return nullptr;
}

DeferredThermostatReportSlot* FindDeferredThermostatReportSlot(sl_zigbee_af_event_t* event)
{
  for (uint8_t i = 0; i < kMaxDeferredThermostatReportSlots; i++) {
    if (&deferred_thermostat_report_slots[i].event == event) {
      return &deferred_thermostat_report_slots[i];
    }
  }

  return nullptr;
}

void DeferredThermostatReportEventHandler(sl_zigbee_af_event_t* event)
{
  DeferredThermostatReportSlot* slot = FindDeferredThermostatReportSlot(event);
  if (slot == nullptr || slot->device == nullptr) {
    return;
  }

  uint16_t report_mask = slot->report_mask;
  slot->report_mask = 0;
  sl_zigbee_af_event_set_inactive(&slot->event);
  slot->device->SendDeferredAttributeReports(report_mask);
}
} // namespace

DeviceThermostat::DeviceThermostat(const char* device_name,
                                   uint8_t endpoint_id,
                                   int16_t local_temperature,
                                   int16_t heating_setpoint) :
  ZigbeeDevice(device_name, endpoint_id),
  local_temperature(local_temperature),
  heating_setpoint(heating_setpoint),
  system_mode(SL_ZIGBEE_ZCL_THERMOSTAT_SYSTEM_MODE_OFF),
  hvac_action(thermostat_hvac_action_idle),
  abs_min_heating_setpoint(700),
  min_heating_setpoint(1600),
  abs_max_heating_setpoint(3200),
  max_heating_setpoint(3000),
  attribute_write_depth(0)
{
}

DeviceThermostat::~DeviceThermostat()
{
  DeferredThermostatReportSlot* slot = FindDeferredThermostatReportSlot(this, false);
  if (slot == nullptr) {
    return;
  }

  if (slot->initialized) {
    ZigbeeAfLock lock;
    sl_zigbee_af_event_set_inactive(&slot->event);
  }
  slot->device = nullptr;
  slot->initialized = false;
  slot->report_mask = 0;
}

void DeviceThermostat::InitializeAttributes()
{
  this->WriteLocalTemperature();
  this->WriteHeatingSetpoint();
  this->WriteSystemMode();
  this->WriteHvacAction();
  this->WriteAbsMinHeatingSetpoint();
  this->WriteMinHeatingSetpoint();
  this->WriteAbsMaxHeatingSetpoint();
  this->WriteMaxHeatingSetpoint();
  this->WriteEnum8Attribute(ZCL_CONTROL_SEQUENCE_OF_OPERATION_ATTRIBUTE_ID,
                            this->thermostat_control_sequence_of_operation);
}

int16_t DeviceThermostat::GetLocalTemperatureValue()
{
  return this->local_temperature;
}

void DeviceThermostat::SetLocalTemperatureValue(int16_t local_temp)
{
  bool changed = this->local_temperature != local_temp;
  this->local_temperature = local_temp;
  this->WriteLocalTemperature();

  if (changed) {
    this->SendInt16AttributeReport(ZCL_LOCAL_TEMPERATURE_ATTRIBUTE_ID, this->local_temperature);
    CallDeviceChangeCallback();
  }
}

int16_t DeviceThermostat::GetHeatingSetpointValue()
{
  return this->heating_setpoint;
}

void DeviceThermostat::SetHeatingSetpointValue(int16_t heating_setpoint)
{
  if (this->UpdateHeatingSetpointValue(heating_setpoint, true)) {
    CallDeviceChangeCallback();
  }
}

void DeviceThermostat::SetAbsMinHeatingSetpoint(int16_t abs_min_heating_setpoint)
{
  if (abs_min_heating_setpoint > this->min_heating_setpoint) {
    return;
  }

  this->abs_min_heating_setpoint = abs_min_heating_setpoint;
  this->WriteAbsMinHeatingSetpoint();
}

int16_t DeviceThermostat::GetAbsMinHeatingSetpoint()
{
  return this->abs_min_heating_setpoint;
}

void DeviceThermostat::SetMinHeatingSetpoint(int16_t min_heating_setpoint)
{
  if (min_heating_setpoint < this->abs_min_heating_setpoint || min_heating_setpoint > this->max_heating_setpoint) {
    return;
  }

  bool changed = this->min_heating_setpoint != min_heating_setpoint;
  this->min_heating_setpoint = min_heating_setpoint;
  this->WriteMinHeatingSetpoint();
  if (changed) {
    this->SendInt16AttributeReport(ZCL_MIN_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID, this->min_heating_setpoint);
  }
  bool heating_setpoint_changed = false;
  if (this->heating_setpoint < this->min_heating_setpoint) {
    heating_setpoint_changed = this->UpdateHeatingSetpointValue(this->heating_setpoint, true);
  }
  if (changed || heating_setpoint_changed) {
    CallDeviceChangeCallback();
  }
}

int16_t DeviceThermostat::GetMinHeatingSetpoint()
{
  return this->min_heating_setpoint;
}

void DeviceThermostat::SetAbsMaxHeatingSetpoint(int16_t abs_max_heating_setpoint)
{
  if (this->max_heating_setpoint > abs_max_heating_setpoint) {
    return;
  }

  this->abs_max_heating_setpoint = abs_max_heating_setpoint;
  this->WriteAbsMaxHeatingSetpoint();
}

int16_t DeviceThermostat::GetAbsMaxHeatingSetpoint()
{
  return this->abs_max_heating_setpoint;
}

void DeviceThermostat::SetMaxHeatingSetpoint(int16_t max_heating_setpoint)
{
  if (max_heating_setpoint > this->abs_max_heating_setpoint || max_heating_setpoint < this->min_heating_setpoint) {
    return;
  }

  bool changed = this->max_heating_setpoint != max_heating_setpoint;
  this->max_heating_setpoint = max_heating_setpoint;
  this->WriteMaxHeatingSetpoint();
  if (changed) {
    this->SendInt16AttributeReport(ZCL_MAX_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID, this->max_heating_setpoint);
  }
  bool heating_setpoint_changed = false;
  if (this->heating_setpoint > this->max_heating_setpoint) {
    heating_setpoint_changed = this->UpdateHeatingSetpointValue(this->heating_setpoint, true);
  }
  if (changed || heating_setpoint_changed) {
    CallDeviceChangeCallback();
  }
}

int16_t DeviceThermostat::GetMaxHeatingSetpoint()
{
  return this->max_heating_setpoint;
}

uint8_t DeviceThermostat::GetSystemMode()
{
  return this->system_mode;
}

void DeviceThermostat::SetSystemMode(uint8_t system_mode)
{
  system_mode = this->NormalizeSystemMode(system_mode);

  bool changed = this->system_mode != system_mode;
  this->system_mode = system_mode;
  this->WriteSystemMode();

  if (changed) {
    this->SendEnum8AttributeReport(ZCL_SYSTEM_MODE_ATTRIBUTE_ID, this->system_mode);
  }
  bool hvac_changed = false;
  if (this->system_mode == SL_ZIGBEE_ZCL_THERMOSTAT_SYSTEM_MODE_OFF) {
    hvac_changed = this->UpdateHvacAction(thermostat_hvac_action_idle, true);
  }
  if (changed || hvac_changed) {
    CallDeviceChangeCallback();
  }
}

uint8_t DeviceThermostat::GetHvacAction()
{
  return this->hvac_action;
}

void DeviceThermostat::SetHvacAction(uint8_t hvac_action)
{
  if (this->UpdateHvacAction(hvac_action, true)) {
    CallDeviceChangeCallback();
  }
}

bool DeviceThermostat::SendAttributeReport()
{
  bool sent = this->SendInt16AttributeReport(ZCL_LOCAL_TEMPERATURE_ATTRIBUTE_ID,
                                             this->local_temperature);
  sent = this->SendInt16AttributeReport(ZCL_OCCUPIED_HEATING_SETPOINT_ATTRIBUTE_ID,
                                        this->heating_setpoint) && sent;
  sent = this->SendInt16AttributeReport(ZCL_MIN_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID,
                                        this->min_heating_setpoint) && sent;
  sent = this->SendInt16AttributeReport(ZCL_MAX_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID,
                                        this->max_heating_setpoint) && sent;
  sent = this->SendEnum8AttributeReport(ZCL_SYSTEM_MODE_ATTRIBUTE_ID,
                                        this->system_mode) && sent;
  sent = this->SendHvacActionReport() && sent;
  return sent;
}

uint32_t DeviceThermostat::HandleAttributePreChange(uint16_t cluster_id,
                                                    uint16_t attribute_id,
                                                    uint8_t size,
                                                    uint8_t* value)
{
  if (cluster_id != ZCL_THERMOSTAT_CLUSTER_ID) {
    return SL_ZIGBEE_ZCL_STATUS_SUCCESS;
  }

  // Local writes from this device increment attribute_write_depth first
  if (this->attribute_write_depth > 0) {
    return SL_ZIGBEE_ZCL_STATUS_SUCCESS;
  }

  // Heat-only thermostat: reject remote changes to the control sequence and the cooling setpoint
  if (attribute_id == ZCL_CONTROL_SEQUENCE_OF_OPERATION_ATTRIBUTE_ID || attribute_id == ZCL_OCCUPIED_COOLING_SETPOINT_ATTRIBUTE_ID) {
    return SL_ZIGBEE_ZCL_STATUS_READ_ONLY;
  }

  if (attribute_id == ZCL_OCCUPIED_HEATING_SETPOINT_ATTRIBUTE_ID && size == sizeof(int16_t)) {
    int16_t new_value = ReadInt16(value);
    if (new_value < this->min_heating_setpoint || new_value > this->max_heating_setpoint) {
      return SL_ZIGBEE_ZCL_STATUS_INVALID_VALUE;
    }
  } else if (attribute_id == ZCL_SYSTEM_MODE_ATTRIBUTE_ID && size == sizeof(uint8_t)) {
    uint8_t new_value = *value;
    if (new_value != SL_ZIGBEE_ZCL_THERMOSTAT_SYSTEM_MODE_OFF && new_value != SL_ZIGBEE_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT) {
      return SL_ZIGBEE_ZCL_STATUS_INVALID_VALUE;
    }
  } else if (attribute_id == ZCL_MIN_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID && size == sizeof(int16_t)) {
    int16_t new_value = ReadInt16(value);
    if (new_value < this->abs_min_heating_setpoint || new_value > this->max_heating_setpoint) {
      return SL_ZIGBEE_ZCL_STATUS_INVALID_VALUE;
    }
  } else if (attribute_id == ZCL_MAX_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID && size == sizeof(int16_t)) {
    int16_t new_value = ReadInt16(value);
    if (new_value > this->abs_max_heating_setpoint || new_value < this->min_heating_setpoint) {
      return SL_ZIGBEE_ZCL_STATUS_INVALID_VALUE;
    }
  }

  return SL_ZIGBEE_ZCL_STATUS_SUCCESS;
}

bool DeviceThermostat::HandleCommand(uint16_t cluster_id,
                                     bool cluster_specific,
                                     uint8_t direction,
                                     uint8_t command_id,
                                     const uint8_t* payload,
                                     uint16_t payload_length,
                                     uint8_t& status)
{
  if (cluster_id != ZCL_THERMOSTAT_CLUSTER_ID
      || !cluster_specific
      || direction != ZCL_DIRECTION_CLIENT_TO_SERVER
      || command_id != ZCL_SETPOINT_RAISE_LOWER_COMMAND_ID
      || payload == nullptr
      || payload_length < 2u) {
    return false;
  }

  uint8_t mode = payload[0];
  int8_t amount = static_cast<int8_t>(payload[1]);
  status = this->HandleSetpointRaiseLower(mode, amount)
           ? SL_ZIGBEE_ZCL_STATUS_SUCCESS
           : SL_ZIGBEE_ZCL_STATUS_INVALID_VALUE;
  return true;
}

bool DeviceThermostat::HandleSetpointRaiseLower(uint8_t mode, int8_t amount)
{
  if (mode != SL_ZIGBEE_ZCL_SETPOINT_ADJUST_MODE_HEAT_SETPOINT && mode != SL_ZIGBEE_ZCL_SETPOINT_ADJUST_MODE_HEAT_AND_COOL_SETPOINTS) {
    return false;
  }

  int32_t new_setpoint = static_cast<int32_t>(this->heating_setpoint) + (static_cast<int32_t>(amount) * 10);
  if (new_setpoint < INT16_MIN) {
    new_setpoint = INT16_MIN;
  }
  if (new_setpoint > INT16_MAX) {
    new_setpoint = INT16_MAX;
  }

  if (this->UpdateHeatingSetpointValue(static_cast<int16_t>(new_setpoint), false)) {
    this->ScheduleAttributeReport(thermostat_report_heating_setpoint);
    CallDeviceChangeCallback();
  }
  return true;
}

bool DeviceThermostat::UpdateHeatingSetpointValue(int16_t heating_setpoint, bool send_report)
{
  if (heating_setpoint < this->min_heating_setpoint) {
    heating_setpoint = this->min_heating_setpoint;
  }
  if (heating_setpoint > this->max_heating_setpoint) {
    heating_setpoint = this->max_heating_setpoint;
  }

  bool changed = this->heating_setpoint != heating_setpoint;
  this->heating_setpoint = heating_setpoint;
  this->WriteHeatingSetpoint();

  if (changed && send_report) {
    this->SendInt16AttributeReport(ZCL_OCCUPIED_HEATING_SETPOINT_ATTRIBUTE_ID, this->heating_setpoint);
  }
  return changed;
}

bool DeviceThermostat::UpdateHvacAction(uint8_t hvac_action, bool send_report)
{
  hvac_action = this->NormalizeHvacAction(hvac_action);
  if (this->system_mode == SL_ZIGBEE_ZCL_THERMOSTAT_SYSTEM_MODE_OFF) {
    hvac_action = thermostat_hvac_action_idle;
  }

  bool changed = this->hvac_action != hvac_action;
  this->hvac_action = hvac_action;
  this->WriteHvacAction();

  if (changed && send_report) {
    this->SendHvacActionReport();
  }
  return changed;
}

bool DeviceThermostat::ScheduleAttributeReport(uint16_t report_mask)
{
  DeferredThermostatReportSlot* slot = FindDeferredThermostatReportSlot(this, true);
  if (slot == nullptr) {
    return false;
  }

  ZigbeeAfLock lock;
  if (!slot->initialized) {
    sl_zigbee_af_event_init(&slot->event, DeferredThermostatReportEventHandler);
    slot->initialized = true;
  }
  slot->report_mask |= report_mask;
  sl_zigbee_af_event_set_delay_ms(&slot->event, kDeferredThermostatReportDelayMs);
  return true;
}

void DeviceThermostat::SendDeferredAttributeReports(uint16_t report_mask)
{
  if (report_mask & thermostat_report_system_mode) {
    this->SendEnum8AttributeReport(ZCL_SYSTEM_MODE_ATTRIBUTE_ID, this->system_mode);
  }
  if (report_mask & thermostat_report_hvac_action) {
    this->SendHvacActionReport();
  }
  if (report_mask & thermostat_report_heating_setpoint) {
    this->SendInt16AttributeReport(ZCL_OCCUPIED_HEATING_SETPOINT_ATTRIBUTE_ID, this->heating_setpoint);
  }
}

void DeviceThermostat::HandleAttributeChange(uint16_t cluster_id,
                                             uint16_t attribute_id,
                                             uint8_t size,
                                             uint8_t* value)
{
  if (cluster_id != ZCL_THERMOSTAT_CLUSTER_ID) {
    return;
  }

  if (this->attribute_write_depth > 0) {
    return;
  }

  if (attribute_id == ZCL_LOCAL_TEMPERATURE_ATTRIBUTE_ID && size == sizeof(int16_t)) {
    int16_t new_value = ReadInt16(value);
    if (this->local_temperature != new_value) {
      this->local_temperature = new_value;
      CallDeviceChangeCallback();
    }
  } else if (attribute_id == ZCL_OCCUPIED_HEATING_SETPOINT_ATTRIBUTE_ID && size == sizeof(int16_t)) {
    int16_t raw_value = ReadInt16(value);
    int16_t new_value = raw_value;
    if (new_value < this->min_heating_setpoint) {
      new_value = this->min_heating_setpoint;
    }
    if (new_value > this->max_heating_setpoint) {
      new_value = this->max_heating_setpoint;
    }

    bool corrected = new_value != raw_value;
    bool changed = this->heating_setpoint != new_value;
    this->heating_setpoint = new_value;

    if (corrected) {
      this->WriteHeatingSetpoint();
    }
    if (changed) {
      CallDeviceChangeCallback();
    }
  } else if (attribute_id == ZCL_SYSTEM_MODE_ATTRIBUTE_ID && size == sizeof(uint8_t)) {
    uint8_t new_value = this->NormalizeSystemMode(*value);
    bool corrected = new_value != *value;
    bool changed = this->system_mode != new_value;
    this->system_mode = new_value;

    if (corrected) {
      this->WriteSystemMode();
    }
    if (changed || corrected) {
      this->ScheduleAttributeReport(thermostat_report_system_mode);
    }
    bool hvac_changed = false;
    if (this->system_mode == SL_ZIGBEE_ZCL_THERMOSTAT_SYSTEM_MODE_OFF) {
      hvac_changed = this->UpdateHvacAction(thermostat_hvac_action_idle, false);
      if (hvac_changed) {
        this->ScheduleAttributeReport(thermostat_report_hvac_action);
      }
    }
    if (changed || hvac_changed) {
      CallDeviceChangeCallback();
    }
  } else if (attribute_id == ZCL_MIN_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID && size == sizeof(int16_t)) {
    int16_t new_value = ReadInt16(value);
    if (new_value < this->abs_min_heating_setpoint || new_value > this->max_heating_setpoint) {
      this->WriteMinHeatingSetpoint();
      return;
    }

    bool changed = this->min_heating_setpoint != new_value;
    this->min_heating_setpoint = new_value;
    bool heating_setpoint_changed = false;
    if (this->heating_setpoint < this->min_heating_setpoint) {
      heating_setpoint_changed = this->UpdateHeatingSetpointValue(this->heating_setpoint, false);
      if (heating_setpoint_changed) {
        this->ScheduleAttributeReport(thermostat_report_heating_setpoint);
      }
    }
    if (changed || heating_setpoint_changed) {
      CallDeviceChangeCallback();
    }
  } else if (attribute_id == ZCL_MAX_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID && size == sizeof(int16_t)) {
    int16_t new_value = ReadInt16(value);
    if (new_value > this->abs_max_heating_setpoint || new_value < this->min_heating_setpoint) {
      this->WriteMaxHeatingSetpoint();
      return;
    }

    bool changed = this->max_heating_setpoint != new_value;
    this->max_heating_setpoint = new_value;
    bool heating_setpoint_changed = false;
    if (this->heating_setpoint > this->max_heating_setpoint) {
      heating_setpoint_changed = this->UpdateHeatingSetpointValue(this->heating_setpoint, false);
      if (heating_setpoint_changed) {
        this->ScheduleAttributeReport(thermostat_report_heating_setpoint);
      }
    }
    if (changed || heating_setpoint_changed) {
      CallDeviceChangeCallback();
    }
  } else if (attribute_id == ZCL_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_ID && size == sizeof(uint8_t)) {
    uint8_t new_value = (*value == SL_ZIGBEE_ZCL_THERMOSTAT_RUNNING_MODE_HEAT)
                        ? thermostat_hvac_action_heating
                        : thermostat_hvac_action_idle;
    if (this->UpdateHvacAction(new_value, false)) {
      CallDeviceChangeCallback();
    }
  } else if (attribute_id == ZCL_THERMOSTAT_RUNNING_STATE_ATTRIBUTE_ID && size == sizeof(uint16_t)) {
    uint16_t new_value = static_cast<uint16_t>(ReadInt16(value));
    uint8_t new_hvac_action = (new_value & SL_ZIGBEE_AF_THERMOSTAT_RUNNING_STATE_HEAT_STATE_ON)
                              ? thermostat_hvac_action_heating
                              : thermostat_hvac_action_idle;
    if (this->UpdateHvacAction(new_hvac_action, false)) {
      CallDeviceChangeCallback();
    }
  }
}

uint8_t DeviceThermostat::NormalizeSystemMode(uint8_t system_mode)
{
  if (system_mode == SL_ZIGBEE_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT) {
    return SL_ZIGBEE_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT;
  }
  return SL_ZIGBEE_ZCL_THERMOSTAT_SYSTEM_MODE_OFF;
}

uint8_t DeviceThermostat::NormalizeHvacAction(uint8_t hvac_action)
{
  if (hvac_action == thermostat_hvac_action_heating) {
    return thermostat_hvac_action_heating;
  }
  return thermostat_hvac_action_idle;
}

uint8_t DeviceThermostat::HvacActionToRunningMode(uint8_t hvac_action)
{
  if (this->NormalizeHvacAction(hvac_action) == thermostat_hvac_action_heating) {
    return SL_ZIGBEE_ZCL_THERMOSTAT_RUNNING_MODE_HEAT;
  }
  return SL_ZIGBEE_ZCL_THERMOSTAT_RUNNING_MODE_OFF;
}

uint16_t DeviceThermostat::HvacActionToRunningState(uint8_t hvac_action)
{
  if (this->NormalizeHvacAction(hvac_action) == thermostat_hvac_action_heating) {
    return SL_ZIGBEE_AF_THERMOSTAT_RUNNING_STATE_HEAT_STATE_ON;
  }
  return 0;
}

void DeviceThermostat::WriteInt16Attribute(uint16_t attribute_id, int16_t value)
{
  ZigbeeAfLock lock;
  this->attribute_write_depth++;
  sl_zigbee_af_write_server_attribute(this->endpoint_id,
                                      ZCL_THERMOSTAT_CLUSTER_ID,
                                      attribute_id,
                                      (uint8_t*)&value,
                                      ZCL_INT16S_ATTRIBUTE_TYPE);
  this->attribute_write_depth--;
}

void DeviceThermostat::WriteEnum8Attribute(uint16_t attribute_id, uint8_t value)
{
  ZigbeeAfLock lock;
  this->attribute_write_depth++;
  sl_zigbee_af_write_server_attribute(this->endpoint_id,
                                      ZCL_THERMOSTAT_CLUSTER_ID,
                                      attribute_id,
                                      &value,
                                      ZCL_ENUM8_ATTRIBUTE_TYPE);
  this->attribute_write_depth--;
}

void DeviceThermostat::WriteBitmap16Attribute(uint16_t attribute_id, uint16_t value)
{
  ZigbeeAfLock lock;
  this->attribute_write_depth++;
  sl_zigbee_af_write_server_attribute(this->endpoint_id,
                                      ZCL_THERMOSTAT_CLUSTER_ID,
                                      attribute_id,
                                      (uint8_t*)&value,
                                      ZCL_BITMAP16_ATTRIBUTE_TYPE);
  this->attribute_write_depth--;
}

bool DeviceThermostat::SendAttributeReport(uint8_t* report_data, uint8_t report_data_length)
{
  ZigbeeAfLock lock;
  bool sent = false;

  sl_zigbee_af_set_command_endpoints(this->endpoint_id, 1);
  sl_zigbee_af_fill_command_global_server_to_client_report_attributes(ZCL_THERMOSTAT_CLUSTER_ID,
                                                                      report_data,
                                                                      report_data_length);
  sent = (sl_zigbee_af_send_command_unicast(SL_ZIGBEE_OUTGOING_DIRECT, 0x0000) == SL_STATUS_OK);

  sl_zigbee_af_set_command_endpoints(this->endpoint_id, 1);
  sl_zigbee_af_fill_command_global_server_to_client_report_attributes(ZCL_THERMOSTAT_CLUSTER_ID,
                                                                      report_data,
                                                                      report_data_length);
  sent = (sl_zigbee_af_send_command_unicast_to_bindings() == SL_STATUS_OK) || sent;

  return sent;
}

bool DeviceThermostat::SendInt16AttributeReport(uint16_t attribute_id, int16_t value)
{
  uint8_t report_data[5];
  report_data[0] = static_cast<uint8_t>(attribute_id & 0xFF);
  report_data[1] = static_cast<uint8_t>((attribute_id >> 8) & 0xFF);
  report_data[2] = ZCL_INT16S_ATTRIBUTE_TYPE;
  report_data[3] = static_cast<uint8_t>(value & 0xFF);
  report_data[4] = static_cast<uint8_t>((value >> 8) & 0xFF);

  return this->SendAttributeReport(report_data, sizeof(report_data));
}

bool DeviceThermostat::SendEnum8AttributeReport(uint16_t attribute_id, uint8_t value)
{
  uint8_t report_data[4];
  report_data[0] = static_cast<uint8_t>(attribute_id & 0xFF);
  report_data[1] = static_cast<uint8_t>((attribute_id >> 8) & 0xFF);
  report_data[2] = ZCL_ENUM8_ATTRIBUTE_TYPE;
  report_data[3] = value;

  return this->SendAttributeReport(report_data, sizeof(report_data));
}

bool DeviceThermostat::SendBitmap16AttributeReport(uint16_t attribute_id, uint16_t value)
{
  uint8_t report_data[5];
  report_data[0] = static_cast<uint8_t>(attribute_id & 0xFF);
  report_data[1] = static_cast<uint8_t>((attribute_id >> 8) & 0xFF);
  report_data[2] = ZCL_BITMAP16_ATTRIBUTE_TYPE;
  report_data[3] = static_cast<uint8_t>(value & 0xFF);
  report_data[4] = static_cast<uint8_t>((value >> 8) & 0xFF);

  return this->SendAttributeReport(report_data, sizeof(report_data));
}

bool DeviceThermostat::SendHvacActionReport()
{
  bool sent = this->SendEnum8AttributeReport(ZCL_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_ID,
                                             this->HvacActionToRunningMode(this->hvac_action));
  sent = this->SendBitmap16AttributeReport(ZCL_THERMOSTAT_RUNNING_STATE_ATTRIBUTE_ID,
                                           this->HvacActionToRunningState(this->hvac_action)) && sent;
  return sent;
}

void DeviceThermostat::WriteLocalTemperature()
{
  this->WriteInt16Attribute(ZCL_LOCAL_TEMPERATURE_ATTRIBUTE_ID, this->local_temperature);
}

void DeviceThermostat::WriteHeatingSetpoint()
{
  this->WriteInt16Attribute(ZCL_OCCUPIED_HEATING_SETPOINT_ATTRIBUTE_ID, this->heating_setpoint);
}

void DeviceThermostat::WriteSystemMode()
{
  this->WriteEnum8Attribute(ZCL_SYSTEM_MODE_ATTRIBUTE_ID, this->system_mode);
}

void DeviceThermostat::WriteHvacAction()
{
  this->WriteEnum8Attribute(ZCL_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_ID,
                            this->HvacActionToRunningMode(this->hvac_action));
  this->WriteBitmap16Attribute(ZCL_THERMOSTAT_RUNNING_STATE_ATTRIBUTE_ID,
                               this->HvacActionToRunningState(this->hvac_action));
}

void DeviceThermostat::WriteAbsMinHeatingSetpoint()
{
  this->WriteInt16Attribute(ZCL_ABS_MIN_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID,
                            this->abs_min_heating_setpoint);
}

void DeviceThermostat::WriteMinHeatingSetpoint()
{
  this->WriteInt16Attribute(ZCL_MIN_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID,
                            this->min_heating_setpoint);
}

void DeviceThermostat::WriteAbsMaxHeatingSetpoint()
{
  this->WriteInt16Attribute(ZCL_ABS_MAX_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID,
                            this->abs_max_heating_setpoint);
}

void DeviceThermostat::WriteMaxHeatingSetpoint()
{
  this->WriteInt16Attribute(ZCL_MAX_HEAT_SETPOINT_LIMIT_ATTRIBUTE_ID,
                            this->max_heating_setpoint);
}
