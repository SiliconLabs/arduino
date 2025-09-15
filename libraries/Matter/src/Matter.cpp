/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2025 Silicon Laboratories Inc. www.silabs.com
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

#include "Matter.h"
#include <util/endpoint-config-api.h>
#include <QRCodeSetupPayloadParser.h>
#include <ManualSetupPayloadGenerator.h>
#include <ProvisionManager.h>

using namespace chip;
using namespace chip::app;
using namespace ::chip::DeviceLayer;
using namespace ::chip::app::Clusters;
using namespace ::chip::DeviceLayer::Silabs;

// Current ZCL implementation of struct uses a max size array of 254 bytes
const uint16_t kDescriptorAttributeArraySize = 254u;

// Descriptor cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(descriptorAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(Descriptor::Attributes::DeviceTypeList::Id, ARRAY, kDescriptorAttributeArraySize, 0), /* DeviceTypeList */
DECLARE_DYNAMIC_ATTRIBUTE(Descriptor::Attributes::ServerList::Id, ARRAY, kDescriptorAttributeArraySize, 0),     /* ServerList */
DECLARE_DYNAMIC_ATTRIBUTE(Descriptor::Attributes::ClientList::Id, ARRAY, kDescriptorAttributeArraySize, 0),     /* ClientList */
DECLARE_DYNAMIC_ATTRIBUTE(Descriptor::Attributes::PartsList::Id, ARRAY, kDescriptorAttributeArraySize, 0),      /* PartsList */
DECLARE_DYNAMIC_ATTRIBUTE(Descriptor::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                              /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                           /* ClusterRevision auto added by LIST_END */

// BridgedDeviceBasicInformation cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(bridgedDeviceBasicAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(BridgedDeviceBasicInformation::Attributes::NodeLabel::Id, CHAR_STRING, Device::DeviceDescStrSize, 0),    /* NodeLabel */
DECLARE_DYNAMIC_ATTRIBUTE(BridgedDeviceBasicInformation::Attributes::VendorName::Id, CHAR_STRING, Device::DeviceDescStrSize, 0),   /* VendorName */
DECLARE_DYNAMIC_ATTRIBUTE(BridgedDeviceBasicInformation::Attributes::ProductName::Id, CHAR_STRING, Device::DeviceDescStrSize, 0),  /* ProductName */
DECLARE_DYNAMIC_ATTRIBUTE(BridgedDeviceBasicInformation::Attributes::SerialNumber::Id, CHAR_STRING, Device::DeviceDescStrSize, 0), /* SerialNumber */
DECLARE_DYNAMIC_ATTRIBUTE(BridgedDeviceBasicInformation::Attributes::Reachable::Id, BOOLEAN, 1, 0),                                /* Reachable */
DECLARE_DYNAMIC_ATTRIBUTE(BridgedDeviceBasicInformation::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                              /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                                              /* ClusterRevision auto added by LIST_END */

// Identify cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(identifyAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(Identify::Attributes::IdentifyTime::Id, INT16U, 2, ATTRIBUTE_MASK_WRITABLE),  /* IdentifyTime */
DECLARE_DYNAMIC_ATTRIBUTE(Identify::Attributes::IdentifyType::Id, INT8U, 1, 0),                         /* IdentifyType */
DECLARE_DYNAMIC_ATTRIBUTE(Identify::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                        /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                   /* ClusterRevision auto added by LIST_END */

CommandId identifyIncomingCommands[] = {
  app::Clusters::Identify::Commands::Identify::Id,
  kInvalidCommandId,
};

// Groups cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(groupsAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(Groups::Attributes::NameSupport::Id, INT8U, 1, 0),        /* NameSupport */
DECLARE_DYNAMIC_ATTRIBUTE(Groups::Attributes::FeatureMap::Id, BITMAP32, 4, 0),      /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                               /* ClusterRevision auto added by LIST_END */

CommandId groupsIncomingCommands[] = {
  app::Clusters::Groups::Commands::AddGroup::Id,
  app::Clusters::Groups::Commands::ViewGroup::Id,
  app::Clusters::Groups::Commands::GetGroupMembership::Id,
  app::Clusters::Groups::Commands::RemoveGroup::Id,
  app::Clusters::Groups::Commands::RemoveAllGroups::Id,
  app::Clusters::Groups::Commands::AddGroupIfIdentifying::Id,
  kInvalidCommandId,
};

CommandId groupsOutgoingCommands[] = {
  app::Clusters::Groups::Commands::AddGroupResponse::Id,
  app::Clusters::Groups::Commands::ViewGroupResponse::Id,
  app::Clusters::Groups::Commands::GetGroupMembershipResponse::Id,
  app::Clusters::Groups::Commands::RemoveGroupResponse::Id,
  kInvalidCommandId,
};

void IdentifyStartHandler(::Identify* identify)
{
  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(identify->mEndpoint);
  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev) {
    return;
  }
  dev->HandleIdentifyStart();
}

void IdentifyStopHandler(::Identify* identify)
{
  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(identify->mEndpoint);
  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev) {
    return;
  }
  dev->HandleIdentifyStop();
}

void TriggerIdentifyEffectHandler(::Identify* identify)
{
  ;
}

ArduinoMatterAppliance::ArduinoMatterAppliance() :
  base_matter_device(nullptr)
{
  ;
}

ArduinoMatterAppliance::~ArduinoMatterAppliance()
{
  ;
}

bool ArduinoMatterAppliance::get_identify_in_progress()
{
  if (this->base_matter_device) {
    return this->base_matter_device->GetIdentifyInProgress();
  }
  return false;
}

void ArduinoMatterAppliance::set_device_name(const char* device_name)
{
  if (this->base_matter_device && device_name) {
    base_matter_device->SetName(device_name);
  }
}

void ArduinoMatterAppliance::set_vendor_name(const char* vendor_name)
{
  if (this->base_matter_device && vendor_name) {
    base_matter_device->SetVendorName(vendor_name);
  }
}

void ArduinoMatterAppliance::set_product_name(const char* product_name)
{
  if (this->base_matter_device && product_name) {
    base_matter_device->SetProductName(product_name);
  }
}

void ArduinoMatterAppliance::set_serial_number(const char* serial_number)
{
  if (this->base_matter_device && serial_number) {
    base_matter_device->SetSerialNumber(serial_number);
  }
}

bool ArduinoMatterAppliance::is_online()
{
  if (this->base_matter_device) {
    return base_matter_device->IsOnline();
  }
  return false;
}

void ArduinoMatterAppliance::set_device_change_callback(void (*matter_device_changed_cb)(void))
{
  if (this->base_matter_device) {
    this->base_matter_device->SetDeviceChangeCallback(matter_device_changed_cb);
  }
}

void MatterClass::begin()
{
  InitDynamicEndpointHandler();
}

void MatterClass::disableBridgeEndpoint()
{
  // The bridge endpoint is on index 1
  emberAfEndpointEnableDisable(emberAfEndpointFromIndex(1), false);
}

String MatterClass::getManualPairingCode()
{
  std::string inputPayload = getOnboardingQRCodePayload().c_str();
  SetupPayload parsedSetupPayload;
  QRCodeSetupPayloadParser parser(inputPayload);
  CHIP_ERROR err = parser.populatePayload(parsedSetupPayload);
  if (err != CHIP_NO_ERROR) {
    return "N/A";
  }
  std::string manualPairingCode;
  err = ManualSetupPayloadGenerator(parsedSetupPayload).payloadDecimalStringRepresentation(manualPairingCode);
  if (err == CHIP_NO_ERROR) {
    return String(manualPairingCode.c_str());
  }
  return "N/A";
}

String MatterClass::getOnboardingQRCodeUrl()
{
  constexpr char kQrCodeBaseUrl[] = "https://project-chip.github.io/connectedhomeip/qrcode.html";
  constexpr char kUrlDataAssignmentPhrase[] = "?data=";

  char setupPayloadBuffer[chip::QRCodeBasicSetupPayloadGenerator::kMaxQRCodeBase38RepresentationLength + 1];
  chip::MutableCharSpan setupPayload(setupPayloadBuffer);
  CHIP_ERROR err = Provision::Manager::GetInstance().GetStorage().GetSetupPayload(setupPayload);
  if (CHIP_NO_ERROR == err) {
    chip::Platform::ScopedMemoryBuffer<char> qrCodeBuffer;
    const size_t qrCodeBufferMaxSize = strlen(kQrCodeBaseUrl) + strlen(kUrlDataAssignmentPhrase) + 3 * setupPayload.size() + 1;
    qrCodeBuffer.Alloc(qrCodeBufferMaxSize);

    if (GetQRCodeUrl(qrCodeBuffer.Get(), qrCodeBufferMaxSize, setupPayload) == CHIP_NO_ERROR) {
      return qrCodeBuffer.Get();
    }
  }
  return "N/A";
}

String MatterClass::getOnboardingQRCodePayload()
{
  char setupPayloadBuffer[chip::QRCodeBasicSetupPayloadGenerator::kMaxQRCodeBase38RepresentationLength + 1];
  chip::MutableCharSpan setupPayload(setupPayloadBuffer);
  CHIP_ERROR err = Provision::Manager::GetInstance().GetStorage().GetSetupPayload(setupPayload);
  if (CHIP_NO_ERROR == err) {
    return setupPayload.data();
  }
  return "N/A";
}

bool MatterClass::isDeviceCommissioned()
{
  return ConnectivityMgr().IsThreadProvisioned();
}

bool MatterClass::isDeviceThreadConnected()
{
  return ConnectivityMgr().IsThreadAttached();
}

void MatterClass::decommission()
{
  PlatformMgr().ScheduleWork([](intptr_t) {
    PlatformMgr().HandleServerShuttingDown(); // HandleServerShuttingDown calls OnShutdown() which is only implemented for the
                                              // basic information cluster it seems. And triggers and Event flush, which is not
                                              // relevant when there are no fabrics left
    ConfigurationMgr().InitiateFactoryReset();
  });

  while (1) {
    // Wait for the factory reset to complete
    // The device will reboot after the decommissioning has completed
    yield();
  }
}

MatterClass Matter;

void CallMatterReportingCallback(intptr_t closure)
{
  auto path = reinterpret_cast<app::ConcreteAttributePath*>(closure);
  MatterReportingAttributeChangeCallback(*path);
  Platform::Delete(path);
}

void ScheduleMatterReportingCallback(EndpointId endpointId, ClusterId cluster, AttributeId attribute)
{
  auto* path = Platform::New<app::ConcreteAttributePath>(endpointId, cluster, attribute);
  chip::DeviceLayer::PlatformMgr().ScheduleWork(CallMatterReportingCallback, reinterpret_cast<intptr_t>(path));
}
