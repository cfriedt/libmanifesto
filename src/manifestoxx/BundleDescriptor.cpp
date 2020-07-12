/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sstream>

#include "BundleDescriptor.h"

using namespace std;

BundleClass BundleDescriptor::class_num() const { return class_; }

string to_string(const BundleDescriptor &desc) {
  stringstream ss;
  ss << "{ "
     << "id: " << int(desc.id) << ", "
     << "class: " << desc.class_ << " "
     << "}";
  return ss.str();
}

ostream &operator<<(ostream &os, const BundleDescriptor &desc) {
  os << to_string(desc);
  return os;
}

string to_string(const BundleClass &_class) {
  switch (_class) {
  case BundleClass::Control:
    return "Control";
  case BundleClass::AP:
    return "AP";
  case BundleClass::HID:
    return "HID";
  case BundleClass::PowerSupply:
    return "Power Supply";
  case BundleClass::BridgedPhy:
    return "Bridged Phy";
  case BundleClass::Display:
    return "Display";
  case BundleClass::Camera:
    return "Camera";
  case BundleClass::Sensor:
    return "Sensor";
  case BundleClass::Lights:
    return "Lights";
  case BundleClass::Vibrator:
    return "Vibrator";
  case BundleClass::Loopback:
    return "Loopback";
  case BundleClass::Audio:
    return "Audio";
  case BundleClass::SVC:
    return "SVC";
  case BundleClass::Firmware:
    return "Firmware";
  case BundleClass::Raw:
    return "Raw";
  case BundleClass::VendorSpecific:
    return "Vendor Specific";
  default:
    return "(unknown)";
  }
}

ostream &operator<<(ostream &os, const BundleClass &_class) {
  os << to_string(_class);
  return os;
}
