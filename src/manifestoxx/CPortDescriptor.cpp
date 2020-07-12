/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sstream>
#include <unordered_map>

#include "manifestoxx/CPortDescriptor.h"

using namespace std;

static const unordered_map<CPortDescriptor::CPortProtocol,
                           pair<string, BundleClass>>
    cport_protocol = {
        {CPortDescriptor::Control, {"Control", BundleClass::Control}},
        {CPortDescriptor::AP, {"AP", BundleClass::AP}},
        {CPortDescriptor::GPIO, {"GPIO", BundleClass::BridgedPhy}},
        {CPortDescriptor::I2C, {"I2C", BundleClass::BridgedPhy}},
        {CPortDescriptor::UART, {"UART", BundleClass::BridgedPhy}},
        {CPortDescriptor::HID, {"HID", BundleClass::BridgedPhy}},
        {CPortDescriptor::USB, {"USB", BundleClass::BridgedPhy}},
        {CPortDescriptor::SDIO, {"SDIO", BundleClass::BridgedPhy}},
        {CPortDescriptor::PowerSupply,
         {"Power Supply", BundleClass::PowerSupply}},
        {CPortDescriptor::PWM, {"PWM", BundleClass::BridgedPhy}},
        {CPortDescriptor::SPI, {"SPI", BundleClass::BridgedPhy}},
        {CPortDescriptor::Display, {"Display", BundleClass::Display}},
        {CPortDescriptor::CameraManagement,
         {"Camera Management", BundleClass::Camera}},
        {CPortDescriptor::Sensor, {"Sensor", BundleClass::Sensor}},
        {CPortDescriptor::Lights, {"Lights", BundleClass::Lights}},
        {CPortDescriptor::Vibrator, {"Vibrator", BundleClass::Vibrator}},
        {CPortDescriptor::Loopback, {"Loopback", BundleClass::Loopback}},
        {CPortDescriptor::AudioManagement,
         {"Audio Management", BundleClass::Audio}},
        {CPortDescriptor::AudioData, {"Audio Data", BundleClass::Audio}},
        {CPortDescriptor::SVC, {"SVC", BundleClass::Audio}},
        {CPortDescriptor::Firmware, {"Firmware", BundleClass::Firmware}},
        {CPortDescriptor::CameraData, {"Camera Data", BundleClass::Camera}},
        {CPortDescriptor::Raw, {"Raw", BundleClass::Raw}},
        {CPortDescriptor::VendorSpecific,
         {"Vendor Specific", BundleClass::VendorSpecific}},
};
const std::pair<std::string, BundleClass> &
CPortDescriptor::protocol_data() const {
  return cport_protocol.at(protocol);
}

CPortDescriptor::CPortProtocol CPortDescriptor::protocol_num() const {
  return protocol;
}

const std::string &CPortDescriptor::protocol_name() const {
  return cport_protocol.at(protocol).first;
}

BundleClass CPortDescriptor::protocol_class() const {
  return cport_protocol.at(protocol).second;
}

string to_string(const CPortDescriptor &desc) {
  stringstream ss;
  ss << "{ "
     << "id: " << int(desc.id) << ", "
     << "bundle: " << int(desc.bundle) << ", "
     << "protocol: " << desc.protocol_name() << " "
     << "}";
  return ss.str();
}

ostream &operator<<(std::ostream &os, const CPortDescriptor &desc) {
  os << to_string(desc);
  return os;
}
