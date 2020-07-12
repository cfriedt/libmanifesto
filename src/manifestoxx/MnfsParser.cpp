/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fstream>
#include <regex>
#include <sstream>
#include <string>

#include <ctype.h>

#include "MnfsParser.h"

using namespace std;

const string MnfsParser::MNFS_HEADER = "manifest-header";
const string MnfsParser::MNFS_HEADER_VMAJ = "version-major";
const string MnfsParser::MNFS_HEADER_VMIN = "version-minor";
const string MnfsParser::INTERFACE_DESC = "interface-descriptor";
const string MnfsParser::INTERFACE_DESC_VSID = "vendor-string-id";
const string MnfsParser::INTERFACE_DESC_PSID = "product-string-id";
const string MnfsParser::STRING_DESC = "string-descriptor";
const string MnfsParser::STRING_DESC_STRING = "string";
const string MnfsParser::BUNDLE_DESC = "bundle-descriptor";
const string MnfsParser::BUNDLE_DESC_CLASS = "class";
const string MnfsParser::CPORT_DESC = "cport-descriptor";
const string MnfsParser::CPORT_DESC_BUNDLE = "bundle";
const string MnfsParser::CPORT_DESC_PROTOCOL = "protocol";

const string MnfsParser::identifier_pattern = "[a-z]([a-z\\-]*[a-z])?";
const string MnfsParser::nat0_pattern = "([1-9][0-9]*|0x[0-9a-f]+|0)";
// TODO: this needs to support utf-8, not just printable ascii
const string MnfsParser::string_pattern = ".*";
const string MnfsParser::section_pattern = "\\["
                                           "(" +
                                           MnfsParser::identifier_pattern +
                                           ")"
                                           "([\\s]+(" +
                                           MnfsParser::nat0_pattern +
                                           "))?"
                                           "\\]";
const string MnfsParser::string_option_pattern =
    ""
    "(" +
    MnfsParser::identifier_pattern +
    ")"
    "[\\s]+=[\\s]+"
    "(" +
    MnfsParser::string_pattern + ")";
const string MnfsParser::nat0_option_pattern = "(" +
                                               MnfsParser::identifier_pattern +
                                               ")"
                                               "[\\s]+=[\\s]+"
                                               "(" +
                                               MnfsParser::nat0_pattern + ")";

static inline void trim(string &s) {
  for (; !s.empty() && ::isspace(s[0]);) {
    s = s.substr(1);
  }
  for (; !s.empty() && ::isspace(s[s.length() - 1]);) {
    s = s.substr(0, s.length() - 1);
  }
}

static vector<string> split(const string &s_, char delim = ' ') {
  vector<string> r;

  string s = s_;
  trim(s);

  if ("" == s) {
    return r;
  }

  stringstream ss;
  string tok;

  ss.str(s);

  while (getline(ss, tok, delim)) {
    if (!tok.empty()) {
      r.push_back(tok);
    }
  }

  return r;
}

static bool starts_with(const string &a, const string &b) {
  return true && a.size() >= b.size() && a.substr(0, b.size()) == b;
}

Manifest MnfsParser::parse(const string &mnfs) {

  Manifest manifest;
  istringstream iss;
  string stripped;
  ostringstream oss;

  this->mnfs = mnfs;
  sections.clear();
  keyvals_by_section.clear();

  regex section_regex(MnfsParser::section_pattern, regex_constants::ECMAScript);
  regex string_option_regex(MnfsParser::string_option_pattern,
                            regex_constants::ECMAScript);
  regex nat0_option_regex(MnfsParser::nat0_option_pattern,
                          regex_constants::ECMAScript);

  string current_section;
  iss = istringstream(mnfs);

  for (string line; getline(iss, line);) {

    trim(line);

    if (line.empty()) {
      // skip empty lines
      continue;
    }

    if (';' == line[0]) {
      // skip comments
      continue;
    }

    smatch smatch;
    // first go through line-by-line and ensure there are no syntax errors
    // while simultaneously storing sections, keys, and values
    if (false) {
    } else if (regex_match(line, section_regex)) {

      string section = line.substr(1, line.size() - 2);

      if (keyvals_by_section.end() != keyvals_by_section.find(section)) {
        throw invalid_argument("duplicate section: '" + section + "'");
      }

      keyvals_by_section[section] = unordered_map<string, string>();
      sections.push_back(section);
      current_section = section;

    } else if (regex_match(line, smatch, nat0_option_regex)) {

      if (current_section.empty()) {
        throw invalid_argument("integer options must belong to a section");
      }

      auto key = smatch[1].str();
      auto val = smatch[4].str();

      keyvals_by_section[current_section][key] = val;

    } else if (regex_match(line, smatch, string_option_regex)) {

      if (current_section.empty()) {
        throw invalid_argument("string options must belong to a section");
      }

      auto key = smatch[1].str();
      auto val = smatch[3].str();

      keyvals_by_section[current_section][key] = val;

    } else {
      throw invalid_argument("syntax error: " + line);
    }

    oss << line << endl;
  }

  // semantically process data
  /* FIXME: validate the integer bundles / interfaces references actually exist.
   * This tool, like the original manifesto, does not perform such validation
   */
  for (auto &section : sections) {

    if (false) {
    } else if (section == MnfsParser::MNFS_HEADER) {

      auto vmaj = get_int_option(section, MnfsParser::MNFS_HEADER_VMAJ,
                                 MnfsParser::MNFS_HEADER_VERSION_SIZE);
      auto vmin = get_int_option(section, MnfsParser::MNFS_HEADER_VMIN,
                                 MnfsParser::MNFS_HEADER_VERSION_SIZE);
      auto header = ManifestHeader(vmaj, vmin);
      manifest.add_header(header);

    } else if (section == MnfsParser::INTERFACE_DESC) {

      auto vsid = get_int_option(section, MnfsParser::INTERFACE_DESC_VSID,
                                 MnfsParser::ID_DESC_SIZE);
      auto psid = get_int_option(section, MnfsParser::INTERFACE_DESC_PSID,
                                 MnfsParser::ID_DESC_SIZE);

      auto interface = make_shared<InterfaceDescriptor>(vsid, psid);
      manifest.add_interface_desc(interface);

    } else if (starts_with(section, MnfsParser::STRING_DESC)) {

      auto id = parse_id(section, MnfsParser::CPORT_ID_DESC_SIZE);
      auto str = get_string_option(section, MnfsParser::STRING_DESC_STRING);

      auto string_ = make_shared<StringDescriptor>(id, str);
      manifest.add_string_desc(string_);

    } else if (starts_with(section, MnfsParser::BUNDLE_DESC)) {

      auto id = parse_id(section, MnfsParser::ID_DESC_SIZE);
      auto class_ = get_int_option(section, MnfsParser::BUNDLE_DESC_CLASS,
                                   MnfsParser::BUNDLE_DESC_CLASS_SIZE);
      auto bundle = make_shared<BundleDescriptor>(id, BundleClass(class_));
      manifest.add_bundle_desc(bundle);

    } else if (starts_with(section, MnfsParser::CPORT_DESC)) {

      auto id = parse_id(section, MnfsParser::ID_DESC_SIZE);
      auto bundle = get_int_option(section, MnfsParser::CPORT_DESC_BUNDLE,
                                   MnfsParser::ID_DESC_SIZE);
      auto protocol = get_int_option(section, MnfsParser::CPORT_DESC_PROTOCOL,
                                     MnfsParser::CPORT_DESC_PROTOCOL_SIZE);

      auto cport = make_shared<CPortDescriptor>(
          id, BundleClass(bundle), CPortDescriptor::CPortProtocol(protocol));
      manifest.add_cport_desc(cport);

    } else {

      throw invalid_argument("invalid descriptor '" + section + "'");
    }
  }

  return manifest;
}

Manifest MnfsParser::parse_file(const string &mnfs_file) {
  ifstream ifs;
  size_t size;

  ifs.open(mnfs_file, ios::in | ios::ate);
  size = ifs.tellg();
  ifs.seekg(0);

  string text = string(size, 0);
  ifs.read(&text.front(), size);
  ifs.close();

  return parse(text);
}

int MnfsParser::check_int(int int_val, uint8_t num_bytes) {
  auto min_ = 0;
  auto max_ = (1 << (8 * num_bytes)) - 1;

  if (int_val < min_ || int_val > max_) {
    throw out_of_range("invalid integer value " + to_string(int_val));
  }

  return int_val;
}

int MnfsParser::parse_id(const string &section, uint8_t num_bytes) {
  int int_value;
  int base = 10;
  vector<string> tokens = split(section);
  string string_value = tokens[1];
  trim(string_value);

  if (string_value.size() > 2 && "0x" == string_value.substr(0, 2)) {
    string_value = string_value.substr(2);
    base = 16;
  }

  int_value = stoi(string_value, nullptr, base);
  int_value = check_int(int_value, num_bytes);

  return int_value;
}

string MnfsParser::get_option(const string &section,
                              const string &option_name) {
  return keyvals_by_section.at(section).at(option_name);
}

int MnfsParser::get_int_option(const string &section, const string &option_name,
                               uint8_t num_bytes) {
  int int_value;
  int base = 10;
  string string_value = get_string_option(section, option_name);

  if (string_value.size() > 2 && "0x" == string_value.substr(0, 2)) {
    string_value = string_value.substr(2);
    base = 16;
  }

  int_value = stoi(string_value, nullptr, base);
  int_value = check_int(int_value, num_bytes);

  return int_value;
}

string MnfsParser::get_string_option(const string &section,
                                     const string &option_name) {
  return get_option(section, option_name);
}
