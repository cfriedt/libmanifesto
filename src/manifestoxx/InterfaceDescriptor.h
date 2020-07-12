/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef INTERFACE_DESCRIPTOR_H_
#define INTERFACE_DESCRIPTOR_H_

#include <iostream>
#include <string>

#include "Descriptor.h"

class InterfaceDescriptor : public Descriptor {
public:
  InterfaceDescriptor() : InterfaceDescriptor(-1, -1) {}
  InterfaceDescriptor(uint8_t vendor_string_id, uint8_t product_string_id)
      : vendor_string_id(vendor_string_id),
        product_string_id(product_string_id) {}
  virtual ~InterfaceDescriptor() {}

  virtual DescriptorType type() const override final {
    return DescriptorType::Interface;
  }

  uint8_t vendor_string_id;
  uint8_t product_string_id;
};

std::string to_string(const InterfaceDescriptor &desc);
std::ostream &operator<<(std::ostream &os, const InterfaceDescriptor &desc);

#endif
