/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MNFB_GENERATOR_H_
#define MNFB_GENERATOR_H_

#include <vector>

#include "Manifest.h"

class MnfbGenerator {
public:
  // types
  /*
   * static constexpr uint8_t INTERFACE_DESC_TYPE = 0x01;
   * static constexpr uint8_t STRING_DESC_TYPE = 0x02;
   * static constexpr uint8_t BUNDLE_DESC_TYPE = 0x03;
   * static constexpr uint8_t CPORT_DESC_TYPE = 0x04;
   */

  // sizes
  static constexpr uint16_t MNFS_MAX_SIZE = 0xffff;
  static constexpr uint8_t MNFS_HEADER_SIZE = 0x4;
  static constexpr uint8_t BASE_DESC_SIZE = 0x4;
  static constexpr uint8_t INTERFACE_DESC_SIZE = BASE_DESC_SIZE + 0x4;
  static constexpr uint8_t STRING_DESC_BASE_SIZE = BASE_DESC_SIZE + 0x2;
  static constexpr uint8_t BUNDLE_DESC_SIZE = BASE_DESC_SIZE + 0x4;
  static constexpr uint8_t CPORT_DESC_SIZE = BASE_DESC_SIZE + 0x4;

  static constexpr uint8_t STRING_MAX_SIZE = 0xff;
  static constexpr uint16_t STRING_DESC_MAX_SIZE =
      STRING_DESC_BASE_SIZE + STRING_MAX_SIZE;

  // formatting
  /*
   * MNFS_HEADER_FMT         = '<HBB'
   * BASE_DESC_FMT           = '<HBx'
   * INTERFACE_DESC_FMT      = BASE_DESC_FMT + 'BB'
   * STRING_DESC_BASE_FMT    = BASE_DESC_FMT + 'BB'
   * BUNDLE_DESC_FMT         = BASE_DESC_FMT + 'BB'
   * CPORT_DESC_FMT          = BASE_DESC_FMT + 'HBB'
   */

  MnfbGenerator() = default;
  ~MnfbGenerator() {}

  std::vector<uint8_t> mnfb;

  void generate(const Manifest &manifest);

protected:
  void pack_header(const ManifestHeader &header);
  void pack_desc(uint16_t desc_size, DescriptorType desc_type, const void *data,
                 uint8_t len);
  void pack_bundle_desc(const BundleDescriptor &desc);
  void pack_cport_desc(const CPortDescriptor &desc);
  void pack_interface_desc(const InterfaceDescriptor &desc);
  void pack_string_desc(const StringDescriptor &desc);
};

#endif
