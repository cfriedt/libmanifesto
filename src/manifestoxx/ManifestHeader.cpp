/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <iostream>
#include <sstream>

#include "manifestoxx/ManifestHeader.h"

using namespace std;

string to_string(const ManifestHeader &header) {
  stringstream ss;
  ss << "{ "
     << "major: " << int(header.major_) << ", "
     << "minor: " << int(header.minor_) << " "
     << "}";
  return ss.str();
}

ostream &operator<<(ostream &os, const ManifestHeader &header) {
  os << to_string(header);
  return os;
}
