/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <iostream>
#include <sstream>
#include <string>

#include "Descriptor.h"

using namespace std;

Descriptor::~Descriptor() {}

string to_string(const DescriptorType &desc) {
  switch (desc) {
  case DescriptorType::Interface:
    return "Interface";
  case DescriptorType::String:
    return "String";
  case DescriptorType::Bundle:
    return "Bundle";
  case DescriptorType::CPort:
    return "CPort";
  default:
    return "(unknown)";
  }
}

ostream &operator<<(ostream &os, const DescriptorType &desc) {
  os << to_string(desc);
  return os;
}
