/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MANIFEST_HEADER_H_
#define MANIFEST_HEADER_H_

#include <cstdint>
#include <iostream>
#include <string>

struct ManifestHeader {

  static constexpr uint8_t GB_VERSION_MAJOR = 0;
  static constexpr uint8_t GB_VERSION_MINOR = 1;

  ManifestHeader() : ManifestHeader(GB_VERSION_MAJOR, GB_VERSION_MINOR) {}
  ManifestHeader(uint8_t major_, uint8_t minor_)
      : major_(major_), minor_(minor_) {}

  uint8_t major_;
  uint8_t minor_;
};

std::string to_string(const ManifestHeader &header);
std::ostream &operator<<(std::ostream &os, const ManifestHeader &header);

#endif
