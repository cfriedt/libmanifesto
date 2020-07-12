/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CPORT_DESCRIPTOR_H_
#define CPORT_DESCRIPTOR_H_

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "BundleClass.h"
#include "BundleDescriptor.h"
#include "Descriptor.h"

class CPortDescriptor : public Descriptor {
public:
  enum CPortProtocol {
    Control = 0x00,
    AP = 0x01,
    GPIO = 0x02,
    I2C = 0x03,
    UART = 0x04,
    HID = 0x05,
    USB = 0x06,
    SDIO = 0x07,
    PowerSupply = 0x08,
    PWM = 0x09,
    SPI = 0x0b,
    Display = 0x0c,
    CameraManagement = 0x0d,
    Sensor = 0x0e,
    Lights = 0x0f,
    Vibrator = 0x10,
    Loopback = 0x11,
    AudioManagement = 0x12,
    AudioData = 0x13,
    SVC = 0x14,
    Firmware = 0x15,
    CameraData = 0x16,
    Raw = 0xfe,
    VendorSpecific = 0xff,
  };

  CPortDescriptor(uint8_t id, uint8_t bundle, CPortProtocol protocol)
      : id(id), bundle(bundle), protocol(protocol) {}
  virtual ~CPortDescriptor() {}

  uint8_t id;
  uint8_t bundle;
  CPortProtocol protocol;

  virtual DescriptorType type() const override final {
    return DescriptorType::CPort;
  }

  const std::pair<std::string, BundleClass> &protocol_data() const;
  CPortProtocol protocol_num() const;
  const std::string &protocol_name() const;
  BundleClass protocol_class() const;
};

std::string to_string(const CPortDescriptor &desc);
std::ostream &operator<<(std::ostream &os, const CPortDescriptor &desc);

#endif
