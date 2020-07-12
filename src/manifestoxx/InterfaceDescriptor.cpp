/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sstream>

#include "manifestoxx/InterfaceDescriptor.h"

using namespace std;

string to_string(const InterfaceDescriptor &desc) {
  stringstream ss;
  ss << "{ "
     << "vendor: " << int(desc.vendor_string_id) << ", "
     << "product: " << int(desc.product_string_id) << " "
     << "}";
  return ss.str();
}

ostream &operator<<(ostream &os, const InterfaceDescriptor &desc) {
  os << to_string(desc);
  return os;
}
