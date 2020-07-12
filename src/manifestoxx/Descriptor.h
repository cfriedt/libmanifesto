/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

#include <cstdint>
#include <iostream>
#include <string>

/*
 * static constexpr uint8_t INTERFACE_DESC_TYPE = 0x01;
 * static constexpr uint8_t STRING_DESC_TYPE = 0x02;
 * static constexpr uint8_t BUNDLE_DESC_TYPE = 0x03;
 * static constexpr uint8_t CPORT_DESC_TYPE = 0x04;
 */
enum DescriptorType {
  Interface = 0x01,
  String = 0x02,
  Bundle = 0x03,
  CPort = 0x04,
};

std::string to_string(const DescriptorType &desc);
std::ostream &operator<<(std::ostream &os, const DescriptorType &desc);

class Descriptor {
public:
  Descriptor(bool used = false) : used(used) {}
  virtual ~Descriptor() = 0;

  virtual DescriptorType type() const = 0;
  bool used;
};

#endif
