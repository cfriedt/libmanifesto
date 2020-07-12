/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BUNDLE_CLASS_H_
#define BUNDLE_CLASS_H_

#include <iostream>

enum BundleClass {
  Control = 0x00,
  AP = 0x01,
  HID = 0x05,
  PowerSupply = 0x08,
  BridgedPhy = 0x0a,
  Display = 0x0c,
  Camera = 0x0d,
  Sensor = 0x0e,
  Lights = 0x0f,
  Vibrator = 0x10,
  Loopback = 0x11,
  Audio = 0x12,
  SVC = 0x14,
  Firmware = 0x15,
  Raw = 0xfe,
  VendorSpecific = 0xff,
};

std::string to_string(const BundleClass &_class);
std::ostream &operator<<(std::ostream &os, const BundleClass &_class);

#endif
