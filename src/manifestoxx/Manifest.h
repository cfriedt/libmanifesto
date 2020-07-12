/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MANIFEST_H_
#define MANIFEST_H_

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "BundleDescriptor.h"
#include "CPortDescriptor.h"
#include "Descriptor.h"
#include "InterfaceDescriptor.h"
#include "ManifestHeader.h"
#include "StringDescriptor.h"

class Manifest {
public:
  Manifest() {}
  ~Manifest() {}

  void add_header(const ManifestHeader &hdr);
  void add_interface_desc(std::shared_ptr<InterfaceDescriptor> desc);
  void add_string_desc(std::shared_ptr<StringDescriptor> desc);
  void add_bundle_desc(std::shared_ptr<BundleDescriptor> desc);
  void add_cport_desc(std::shared_ptr<CPortDescriptor> desc);

  void auto_check();

  ManifestHeader header;
  std::vector<std::shared_ptr<Descriptor>> descriptors;

  std::shared_ptr<InterfaceDescriptor> interface_desc;
  std::map<uint8_t, std::shared_ptr<StringDescriptor>> string_descs;
  std::map<uint8_t, std::shared_ptr<BundleDescriptor>> bundle_descs;
  std::map<uint8_t, std::shared_ptr<CPortDescriptor>> cport_descs;

  void __add_desc(std::shared_ptr<Descriptor> desc);
  // void __add_desc_dict(std::map<uint8_t,std::shared_ptr<Descriptor>>& dict_,
  // std::shared_ptr<Descriptor> desc);
};

std::string to_string(const Manifest &desc);
std::ostream &operator<<(std::ostream &os, const Manifest &m);

#endif
