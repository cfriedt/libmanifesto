/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BUNDLE_DESCRIPTOR_H_
#define BUNDLE_DESCRIPTOR_H_

#include <iostream>
#include <string>
#include <vector>

#include "BundleClass.h"
#include "CPortDescriptor.h"
#include "Descriptor.h"

// fwd declaration
class CPortDescriptor;

class BundleDescriptor : public Descriptor {
public:
  BundleDescriptor(uint8_t id, BundleClass class_) : id(id), class_(class_) {}
  virtual ~BundleDescriptor() {}

  virtual DescriptorType type() const override final {
    return DescriptorType::Bundle;
  }

  uint8_t id;
  BundleClass class_;

  BundleClass class_num() const;
  const std::string &class_name() const;
  void add_cport(const CPortDescriptor &desc);

protected:
  std::vector<CPortDescriptor> cports;
};

std::string to_string(const BundleDescriptor &desc);
std::ostream &operator<<(std::ostream &os, const BundleDescriptor &desc);

#endif
