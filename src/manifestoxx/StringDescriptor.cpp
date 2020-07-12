/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sstream>

#include "manifestoxx/StringDescriptor.h"

using namespace std;

string to_string(const StringDescriptor &desc) {
  stringstream ss;
  ss << "{ "
     << "id: " << int(desc.id) << ", "
     << "string: '" << desc.string_ << "' "
     << "}";
  return ss.str();
}

ostream &operator<<(std::ostream &os, const StringDescriptor &desc) {
  os << to_string(desc);
  return os;
}
