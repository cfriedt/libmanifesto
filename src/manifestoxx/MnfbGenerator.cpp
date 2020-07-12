/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <endian.h>

#include "manifestoxx/MnfbGenerator.h"

using namespace std;

void MnfbGenerator::generate(const Manifest &manifest) {

  // reserve enough space for the header
  // it will be filled-in later
  mnfb = vector<uint8_t>(MNFS_HEADER_SIZE, 0);

  for (auto &desc : manifest.descriptors) {

    auto desc_type = desc->type();
    switch (desc_type) {

    case DescriptorType::Bundle: {
      BundleDescriptor *bundle =
          reinterpret_cast<BundleDescriptor *>(desc.get());
      // cout << "packing bundle desc " << *bundle << endl;
      pack_bundle_desc(*bundle);
    } break;

    case DescriptorType::CPort: {
      CPortDescriptor *cport = reinterpret_cast<CPortDescriptor *>(desc.get());
      // cout << "packing cport desc " << *cport << endl;
      pack_cport_desc(*cport);
    } break;

    case DescriptorType::Interface: {
      InterfaceDescriptor *interface =
          reinterpret_cast<InterfaceDescriptor *>(desc.get());
      // cout << "packing interface " << *interface << endl;
      pack_interface_desc(*interface);
    } break;

    case DescriptorType::String: {
      StringDescriptor *string_ =
          reinterpret_cast<StringDescriptor *>(desc.get());
      // cout << "packing string desc " << *string_ << endl;
      pack_string_desc(*string_);
    } break;

    default:
      throw logic_error("unknown descriptor type " + to_string(desc_type));
    }
  }

  // cout << "packing header " << manifest.header << endl;
  pack_header(manifest.header);
}

void MnfbGenerator::pack_header(const ManifestHeader &header) {
  // MNFS_HEADER_FMT = '<HBB' (le u16, u8, u8)
  uint16_t mnfb_size = mnfb.size();
  mnfb[0] = (mnfb_size & 0x00ff) >> 0;
  mnfb[1] = (mnfb_size & 0xff00) >> 1;
  mnfb[2] = header.major_;
  mnfb[3] = header.minor_;
}

void MnfbGenerator::pack_desc(uint16_t desc_size, DescriptorType desc_type,
                              const void *data, uint8_t len) {
  // cout << "size: " << desc_size << endl;
  // BASE_DESC_FMT = '<HBx' (le u16, u8, pad byte)
  if (desc_size % 4) {
    throw invalid_argument("invalid size " + to_string(desc_size));
  }
  mnfb.push_back((desc_size & 0x00ff) >> 0);
  mnfb.push_back((desc_size & 0xff00) >> 8);
  mnfb.push_back(uint8_t(desc_type));
  mnfb.push_back(0);
  if (len > 0) {
    mnfb.insert(mnfb.end(), (uint8_t *)data, (uint8_t *)data + len);
  }
  for (len += 4; len < desc_size; ++len) {
    mnfb.push_back(0);
  }
}

void MnfbGenerator::pack_bundle_desc(const BundleDescriptor &desc) {
  array<uint8_t, 2> data{desc.id, uint8_t(desc.class_num())};
  pack_desc(BUNDLE_DESC_SIZE, DescriptorType::Bundle, &data.front(),
            data.size());
}

void MnfbGenerator::pack_cport_desc(const CPortDescriptor &desc) {
  array<uint8_t, 4> data{uint8_t(desc.id >> 0), uint8_t(desc.id >> 8),
                         desc.bundle, (uint8_t)desc.protocol_num()};
  pack_desc(CPORT_DESC_SIZE, DescriptorType::CPort, &data.front(), data.size());
}

void MnfbGenerator::pack_interface_desc(const InterfaceDescriptor &desc) {
  array<uint8_t, 2> data{desc.vendor_string_id, desc.product_string_id};
  pack_desc(INTERFACE_DESC_SIZE, DescriptorType::Interface, &data.front(),
            data.size());
}

static uint16_t string_desc_size(const string &string_) {
  auto base_size = MnfbGenerator::STRING_DESC_BASE_SIZE + string_.size();
  auto mod = base_size % 4;
  auto pad_bytes = mod ? (4 - mod) : 0;
  // cout << "base: " << int(MnfbGenerator::STRING_DESC_BASE_SIZE) << " len: "
  // << string_.size() << " mod: " << int(mod) << " pad: " << int(pad_bytes) <<
  // endl;
  return base_size + pad_bytes;
}

void MnfbGenerator::pack_string_desc(const StringDescriptor &desc) {
  vector<uint8_t> data;
  auto sz = string_desc_size(desc.string_);
  if (sz > MnfbGenerator::STRING_DESC_MAX_SIZE) {
    throw logic_error("string descriptor too long with length " +
                      to_string(sz));
  }
  data.push_back(uint8_t(desc.string_.size()));
  data.push_back(desc.id);
  data.insert(data.end(), desc.string_.c_str(),
              desc.string_.c_str() + desc.string_.size());
  pack_desc(sz, DescriptorType::String, &data.front(), data.size());
}
