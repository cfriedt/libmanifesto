/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MNFS_PARSER_H_
#define MNFS_PARSER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "Manifest.h"

class MnfsParser {
public:
  // strings
  static const std::string MNFS_HEADER;
  static const std::string MNFS_HEADER_VMAJ;
  static const std::string MNFS_HEADER_VMIN;
  static const std::string INTERFACE_DESC;
  static const std::string INTERFACE_DESC_VSID;
  static const std::string INTERFACE_DESC_PSID;
  static const std::string STRING_DESC;
  static const std::string STRING_DESC_STRING;
  static const std::string BUNDLE_DESC;
  static const std::string BUNDLE_DESC_CLASS;
  static const std::string CPORT_DESC;
  static const std::string CPORT_DESC_BUNDLE;
  static const std::string CPORT_DESC_PROTOCOL;

  // sizes
  static const uint8_t MNFS_HEADER_VERSION_SIZE = 1;
  static const uint8_t ID_DESC_SIZE = 1;
  static const uint8_t STRING_DESC_STRING_SIZE = 255;
  static const uint8_t BUNDLE_DESC_CLASS_SIZE = 1;
  static const uint8_t CPORT_ID_DESC_SIZE = 2;
  static const uint8_t CPORT_DESC_PROTOCOL_SIZE = 1;

  Manifest parse(const std::string &mnfs);
  Manifest parse_file(const std::string &mnfs_file);

protected:
  static const std::string identifier_pattern;
  static const std::string nat0_pattern;
  static const std::string string_pattern;

  static const std::string section_pattern;
  static const std::string string_option_pattern;
  static const std::string nat0_option_pattern;

  // original, unmodified manifest
  std::string mnfs;
  // stripped manifest
  std::string stripped_mnfs;

  // preserve the order of sections as declared in mnfs
  std::vector<std::string> sections;
  // easily lookup key-value pairs (as strings) in a manifest
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
      keyvals_by_section;

  int check_int(int int_val, uint8_t num_bytes);
  int parse_id(const std::string &section, uint8_t num_bytes);
  std::string get_option(const std::string &section,
                         const std::string &option_name);
  int get_int_option(const std::string &section, const std::string &option_name,
                     uint8_t num_bytes);
  std::string get_string_option(const std::string &section,
                                const std::string &option_name);
};

#endif
