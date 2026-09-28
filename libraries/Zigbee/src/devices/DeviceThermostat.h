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

#ifndef ZIGBEE_DEVICE_THERMOSTAT_H
#define ZIGBEE_DEVICE_THERMOSTAT_H

#include "ZigbeeDevice.h"

class DeviceThermostat : public ZigbeeDevice {
public:
  DeviceThermostat(const char* device_name,
                   uint8_t endpoint_id,
                   int16_t local_temperature,
                   int16_t heating_setpoint);
  ~DeviceThermostat();

  void InitializeAttributes();

  int16_t GetLocalTemperatureValue();
  void SetLocalTemperatureValue(int16_t local_temp);
  int16_t GetHeatingSetpointValue();
  void SetHeatingSetpointValue(int16_t heating_setpoint);

  void SetAbsMinHeatingSetpoint(int16_t abs_min_heating_setpoint);
  int16_t GetAbsMinHeatingSetpoint();
  void SetMinHeatingSetpoint(int16_t min_heating_setpoint);
  int16_t GetMinHeatingSetpoint();
  void SetAbsMaxHeatingSetpoint(int16_t abs_max_heating_setpoint);
  int16_t GetAbsMaxHeatingSetpoint();
  void SetMaxHeatingSetpoint(int16_t max_heating_setpoint);
  int16_t GetMaxHeatingSetpoint();

  uint8_t GetSystemMode();
  void SetSystemMode(uint8_t system_mode);
  uint8_t GetHvacAction();
  void SetHvacAction(uint8_t hvac_action);
  bool SendAttributeReport();
  void SendDeferredAttributeReports(uint16_t report_mask);

  uint32_t HandleAttributePreChange(uint16_t cluster_id,
                                    uint16_t attribute_id,
                                    uint8_t size,
                                    uint8_t* value) override;
  bool HandleCommand(uint16_t cluster_id,
                     bool cluster_specific,
                     uint8_t direction,
                     uint8_t command_id,
                     const uint8_t* payload,
                     uint16_t payload_length,
                     uint8_t& status) override;

  void HandleAttributeChange(uint16_t cluster_id,
                             uint16_t attribute_id,
                             uint8_t size,
                             uint8_t* value) override;

private:
  static const uint8_t thermostat_control_sequence_of_operation = 2;
  static const uint8_t thermostat_hvac_action_idle = 0;
  static const uint8_t thermostat_hvac_action_heating = 1;
  static const uint16_t thermostat_report_system_mode = 0x0001;
  static const uint16_t thermostat_report_hvac_action = 0x0002;
  static const uint16_t thermostat_report_heating_setpoint = 0x0004;

  bool HandleSetpointRaiseLower(uint8_t mode, int8_t amount);
  bool UpdateHeatingSetpointValue(int16_t heating_setpoint, bool send_report);
  bool UpdateHvacAction(uint8_t hvac_action, bool send_report);
  bool ScheduleAttributeReport(uint16_t report_mask);
  uint8_t NormalizeSystemMode(uint8_t system_mode);
  uint8_t NormalizeHvacAction(uint8_t hvac_action);
  uint8_t HvacActionToRunningMode(uint8_t hvac_action);
  uint16_t HvacActionToRunningState(uint8_t hvac_action);
  void WriteInt16Attribute(uint16_t attribute_id, int16_t value);
  void WriteEnum8Attribute(uint16_t attribute_id, uint8_t value);
  void WriteBitmap16Attribute(uint16_t attribute_id, uint16_t value);
  bool SendAttributeReport(uint8_t* report_data, uint8_t report_data_length);
  bool SendInt16AttributeReport(uint16_t attribute_id, int16_t value);
  bool SendEnum8AttributeReport(uint16_t attribute_id, uint8_t value);
  bool SendBitmap16AttributeReport(uint16_t attribute_id, uint16_t value);
  bool SendHvacActionReport();
  void WriteLocalTemperature();
  void WriteHeatingSetpoint();
  void WriteSystemMode();
  void WriteHvacAction();
  void WriteAbsMinHeatingSetpoint();
  void WriteMinHeatingSetpoint();
  void WriteAbsMaxHeatingSetpoint();
  void WriteMaxHeatingSetpoint();

  int16_t local_temperature;
  int16_t heating_setpoint;
  uint8_t system_mode;
  uint8_t hvac_action;
  int16_t abs_min_heating_setpoint;
  int16_t min_heating_setpoint;
  int16_t abs_max_heating_setpoint;
  int16_t max_heating_setpoint;
  uint8_t attribute_write_depth;
};

#endif // ZIGBEE_DEVICE_THERMOSTAT_H
