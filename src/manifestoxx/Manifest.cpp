/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdexcept>

#include "manifestoxx/Manifest.h"

using namespace std;

template <class T>
void __add_desc_dict(std::map<uint8_t, std::shared_ptr<T>> &dict_,
                     std::shared_ptr<T> desc) {
  if (dict_.end() != dict_.find(desc->id)) {
    throw logic_error("duplicate id for descriptors " +
                      to_string(*(dict_[desc->id])) + " and " +
                      to_string(*desc));
  }
  dict_[desc->id] = desc;
}

void Manifest::add_header(const ManifestHeader &hdr) { header = hdr; }

void Manifest::add_interface_desc(std::shared_ptr<InterfaceDescriptor> desc) {

  if (nullptr != interface_desc) {
    throw logic_error("multiple instances of " + to_string(*desc));
  }

  interface_desc = desc;
  __add_desc(desc);
}

void Manifest::add_string_desc(std::shared_ptr<StringDescriptor> desc) {
  __add_desc_dict(string_descs, desc);
  __add_desc(desc);
}

void Manifest::add_bundle_desc(std::shared_ptr<BundleDescriptor> desc) {
  __add_desc_dict(bundle_descs, desc);
  __add_desc(desc);
}

void Manifest::add_cport_desc(std::shared_ptr<CPortDescriptor> desc) {
  __add_desc_dict(cport_descs, desc);
  __add_desc(desc);
}

void Manifest::__add_desc(std::shared_ptr<Descriptor> desc) {
  descriptors.push_back(desc);
}

void Manifest::auto_check() {
  /*
      # check interface
      for id_ in [self.interface_desc.vsid, self.interface_desc.psid]:
          # check that it refers to existing strings
          try:
              str_desc = self.string_descs[id_]
              str_desc.parent = self.interface_desc
          except KeyError as e:
              raise Error("invalid reference to id '{}' by '[{}]'"
                      .format(id_, self.interface_desc.section))

      # check cports
      cport_control = False
      for k in self.cport_descs:
          # check that it refers to an existing bundle
          try:
              cport_desc = self.cport_descs[k]
              bundle_desc = self.bundle_descs[cport_desc.bundle]
              bundle_desc.add_cport(cport_desc)
          except KeyError as e:
              raise Error("invalid reference to bundle '{}' by '[{}]'"
                      .format(cport_desc.bundle, cport_desc.section))

          # check that the protocol is not reserved
          if cport_desc.protocol_name == 'Reserved':
              warnings.warn("reserved protocol for '[{}]'"
                      .format(cport_desc.section))

          # check that the protocol corresponds to the bundle's class
          if cport_desc.protocol_class != bundle_desc.class_num:
              warnings.warn("incompatible cport protocol '{}' of '[{}]' "
                      "with class '{}' of '[{}]'"
                      .format(cport_desc.protocol_name, cport_desc.section,
                          bundle_desc.class_name, bundle_desc.section))

          # check that a control cport exists
          if cport_desc.id_ == 0 and cport_desc.protocol_name == 'Control':
              cport_control = True
      if cport_control is False:
          warnings.warn("missing 'Control' cport")

      # check bundles
      bundle_control = False
      for k in self.bundle_descs:
          # check that the class is not reserved
          bundle_desc = self.bundle_descs[k]
          if bundle_desc.class_name ==  'Reserved':
              warnings.warn("reserved class for '[{}]'"
                      .format(bundle_desc.section))
          # check that a control bundle exists
          if bundle_desc.id_ == 0 and bundle_desc.class_name == 'Control':
              bundle_control = True
      if bundle_control is False:
          warnings.warn("missing 'Control' bundle")

      # check that all descriptors are being used
      for desc in self.descriptors:
          if not desc.used:
              warnings.warn("unused descriptor '[{}]'"
                      .format(desc.section))
  */
}

string to_string(const Manifest &desc) {
  (void)desc;
  return "";
}

ostream &operator<<(ostream &os, const Manifest &m) {
  os << to_string(m);
  return os;
}
