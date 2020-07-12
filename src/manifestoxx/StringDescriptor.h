/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef STRING_DESCRIPTOR_H_
#define STRING_DESCRIPTOR_H_

#include <iostream>
#include <memory>
#include <string>

#include "Descriptor.h"

class StringDescriptor : public Descriptor {
public:
  StringDescriptor(uint8_t id, const std::string &string_)
      : id(id), string_(string_) {}
  ~StringDescriptor() {}

  virtual DescriptorType type() const override final {
    return DescriptorType::String;
  }

  std::shared_ptr<Descriptor> parent() const;
  void parent(std::shared_ptr<Descriptor> parent);

  std::shared_ptr<Descriptor> parent_;
  uint8_t id;
  const std::string string_;
};

std::string to_string(const StringDescriptor &desc);
std::ostream &operator<<(std::ostream &os, const StringDescriptor &desc);

#endif
