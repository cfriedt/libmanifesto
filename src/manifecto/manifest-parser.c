/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manifest_.h"
#include "sgetline.h"

#define MNFS_HEADER "manifest-header"
#define MNFS_HEADER_VMAJ "version-major"
#define MNFS_HEADER_VMIN "version-minor"
#define INTERFACE_DESC "interface-descriptor"
#define INTERFACE_DESC_VSID "vendor-string-id"
#define INTERFACE_DESC_PSID "product-string-id"
#define STRING_DESC "string-descriptor"
#define STRING_DESC_STRING "string"
#define BUNDLE_DESC "bundle-descriptor"
#define BUNDLE_DESC_CLASS "class"
#define CPORT_DESC "cport-descriptor"
#define CPORT_DESC_BUNDLE "bundle"
#define CPORT_DESC_PROTOCOL "protocol"

#ifdef DEBUG
#define D(fmt, args...)                                                        \
  printf("%s(): %d: " fmt "\n", __func__, __LINE__, ##args)
#define E(fmt, args...)                                                        \
  if (errno) {                                                                 \
    fprintf(stderr, "%s(): %d: %d: %s: " fmt "\n", __func__, __LINE__, errno,  \
            strerror(errno), ##args);                                          \
  } else {                                                                     \
    fprintf(stderr, "%s(): %d: " fmt "\n", __func__, __LINE__, ##args);        \
  }
#else
#define E(fmt, args...)
#define D(fmt, args...)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

// ugh... strdup(3) and strtok_r(3) are not standard c.
// Doesn't get declared with -std=c11
extern char *strdup(const char *s);
extern char *strtok_r(char *str, const char *delim, char **saveptr);

// intentionally not using regex here, as it is possibly not available in some
// embedded devices, which is where the C library is expected to be used

static int read_manifest_file(const char *file, size_t *file_size,
                              uint8_t **file_data);
static size_t trim(char *s);
static bool is_section(char *line, char **const section_name);
static bool is_option(char *line, char **const option_name,
                      char **const string_value);
static bool is_identifier(const char *identifier);
static bool is_nat0(const char *s);
static int manifest_parse_id(manifest_t manifest, const char *section_name,
                             uint8_t num_bytes);

static int manifest_syntax_check(manifest_t manifest, const char *mnfs,
                                 size_t mnfs_size) {

  int r;
  char *line;
  ssize_t line_size;
  size_t trimmed_line_size;
  char *stream;
  size_t stream_size;
  size_t line_buffer_size;
  char *section_name;
  char *option_name;
  char *option_value;
  char *current_section = NULL;

  for (
      // loop initialization
      line = NULL, line_buffer_size = 0, stream = (char *)mnfs,
      stream_size = mnfs_size,
      line_size = sgetline(&line, &line_buffer_size, stream, stream_size);
      // stop when sgetline() returns -1
      - 1 != line_size;
      // loop update
      stream_size -= line_size,
      stream += line_size,
      line_size = sgetline(&line, &line_buffer_size, stream, stream_size)) {

    trimmed_line_size = trim(line);

    section_name = NULL;
    option_name = NULL;
    option_value = NULL;

    // D("line: '%s'", line);

    if (false) {
    } else if (0 == trimmed_line_size) {
      // skip empty lines
      continue;
    } else if (';' == line[0]) {
      // skip comments
      continue;
    } else if (is_section(line, &section_name)) {
      r = manifest_add_section(manifest, section_name);
      if (0 != r) {
        E("manifest_add_section() failed: %d", r);
        free(section_name);
        section_name = NULL;
        goto out;
      }
      current_section = section_name;
      continue;
    } else if (is_option(line, &option_name, &option_value)) {
      if (NULL == current_section) {
        E("current_selection is NULL");
        if (NULL != option_name) {
          free(option_name);
          option_name = NULL;
        }
        if (NULL != option_value) {
          free(option_value);
          option_value = NULL;
        }
        r = -EINVAL;
        goto out;
      }

      r = manifest_add_option(manifest, current_section, option_name,
                              option_value);
      if (0 != r) {
        E("manifest_add_option() failed: %d", r);
        free(option_name);
        option_name = NULL;
        free(option_value);
        option_value = NULL;
        goto out;
      }
      continue;
    } else {
      E("invalid line '%s'", line);
      r = -EINVAL;
      goto out;
    }
  }

out:
  if (NULL != line) {
    free(line);
  }

  return r;
}

static int manifest_semantic_check(manifest_t manifest) {

  /* FIXME: validate the integer bundles / interfaces references actually exist.
   * This tool, like the original manifesto, does not perform such validation
   */

  int r;
  int version_major;
  int version_minor;
  int vendor_string_id;
  int product_string_id;
  int id;
  char *string_;
  int class_;
  int bundle;
  int protocol;

  struct manifest_section *section;
  struct manifest_ *const man = (struct manifest_ *)manifest;

  for (size_t i = 0; i < man->num_sections; ++i) {
    section = &man->sections[i];
    if (false) {
    } else if (0 == strcmp(section->name, MNFS_HEADER)) {
      r = manifest_get_int_option(manifest, section->name, MNFS_HEADER_VMAJ,
                                  MNFS_HEADER_VERSION_SIZE);
      if (r < 0) {
        goto out;
      }
      version_major = r;

      r = manifest_get_int_option(manifest, section->name, MNFS_HEADER_VMIN,
                                  MNFS_HEADER_VERSION_SIZE);
      if (r < 0) {
        goto out;
      }
      version_minor = r;

      r = manifest_add_header(manifest, version_major, version_minor);
      if (0 != r) {
        goto out;
      }
    } else if (0 == strcmp(section->name, INTERFACE_DESC)) {

      r = manifest_get_int_option(manifest, section->name, INTERFACE_DESC_VSID,
                                  ID_DESC_SIZE);
      if (r < 0) {
        goto out;
      }
      vendor_string_id = r;

      r = manifest_get_int_option(manifest, section->name, INTERFACE_DESC_PSID,
                                  ID_DESC_SIZE);
      if (r < 0) {
        goto out;
      }
      product_string_id = r;

      r = manifest_add_interface_desc(manifest, vendor_string_id,
                                      product_string_id);
      if (0 != r) {
        goto out;
      }

    } else if (0 == strncmp(section->name, STRING_DESC, strlen(STRING_DESC))) {

      r = manifest_parse_id(manifest, section->name, ID_DESC_SIZE);
      if (r < 0) {
        goto out;
      }
      id = r;

      string_ = manifest_get_string_option(manifest, section->name,
                                           STRING_DESC_STRING);
      if (NULL == string_) {
        goto out;
      }

      r = manifest_add_string_desc(manifest, id, string_);
      if (0 != r) {
        goto out;
      }

    } else if (0 == strncmp(section->name, BUNDLE_DESC, strlen(BUNDLE_DESC))) {

      r = manifest_parse_id(manifest, section->name, ID_DESC_SIZE);
      if (r < 0) {
        goto out;
      }
      id = r;

      r = manifest_get_int_option(manifest, section->name, BUNDLE_DESC_CLASS,
                                  BUNDLE_DESC_CLASS_SIZE);
      if (r < 0) {
        goto out;
      }
      class_ = r;

      r = manifest_add_bundle_desc(manifest, id, class_);
      if (0 != r) {
        goto out;
      }

    } else if (0 == strncmp(section->name, CPORT_DESC, strlen(CPORT_DESC))) {

      r = manifest_parse_id(manifest, section->name, ID_DESC_SIZE);
      if (r < 0) {
        goto out;
      }
      id = r;

      r = manifest_get_int_option(manifest, section->name, CPORT_DESC_BUNDLE,
                                  ID_DESC_SIZE);
      if (r < 0) {
        goto out;
      }
      bundle = r;

      r = manifest_get_int_option(manifest, section->name, CPORT_DESC_PROTOCOL,
                                  CPORT_DESC_PROTOCOL_SIZE);
      if (r < 0) {
        goto out;
      }
      protocol = r;

      r = manifest_add_cport_desc(manifest, id, bundle, protocol);
      if (0 != r) {
        goto out;
      }

    } else {
      r = -EINVAL;
      goto out;
    }
  }

  r = 0;

out:
  return r;
}

int manifest_mnfs_parse(manifest_t manifest, const char *mnfs,
                        size_t mnfs_size) {

  int r;

  // first go through line-by-line and ensure there are no syntax errors
  // while simultaneously storing sections, keys, and values
  r = manifest_syntax_check(manifest, mnfs, mnfs_size);
  if (0 != r) {
    E("manifest_syntax_check() failed: %d", r);
    return r;
  }

  // then semantically process data
  r = manifest_semantic_check(manifest);
  if (0 != r) {
    E("manifest_semantic_check() failed: %d", r);
  }

  return r;
}

int manifest_mnfs_parse_file(manifest_t manifest, const char *file) {

  int r;
  size_t file_size = 0;
  uint8_t *file_data = NULL;

  if (NULL == manifest || NULL == file) {
    r = -EINVAL;
    goto out;
  }

  r = read_manifest_file(file, &file_size, &file_data);
  if (0 != r) {
    goto out;
  }

  r = manifest_mnfs_parse(manifest, (const char *)file_data, file_size);
  free(file_data);

out:
  return r;
}

static int read_manifest_file(const char *file, size_t *file_size,
                              uint8_t **file_data) {
  int r;
  FILE *fp;
  long len;

  if (NULL == file || NULL == file_size || NULL == file_data) {
    r = -EINVAL;
    goto out;
  }

  fp = fopen(file, "r");
  if (NULL == fp) {
    r = -errno;
    goto out;
  }

  r = fseek(fp, 0, SEEK_END);
  if (-1 == r) {
    r = -errno;
    goto close_fp;
  }

  len = ftell(fp);
  if (-1 == r) {
    r = -errno;
    goto close_fp;
  }

  r = fseek(fp, 0, SEEK_SET);
  if (-1 == r) {
    r = -errno;
    goto close_fp;
  }

  *file_data = (uint8_t *)malloc((size_t)len);
  if (NULL == *file_data) {
    E("malloc: failed to allocate %u bytes", (unsigned)len);
    r = -ENOMEM;
    goto close_fp;
  }
  *file_size = (size_t)len;

  r = fread(*file_data, 1, *file_size, fp);
  if ((size_t)r != *file_size) {
    E("fread: expected: %d actual: %d", (int)*file_size, r);
    r = -EIO;
    goto free_file_data;
  }

  r = 0;
  goto close_fp;

free_file_data:
  free(*file_data);
  *file_data = NULL;
  *file_size = 0;

close_fp:
  fclose(fp);

out:
  return r;
}

static inline size_t trim(char *const s) {

  size_t size;
  char *s_ = s;

  if (NULL == s) {
    return 0;
  }

  for (size = strlen(s_); 0 != size;) {
    if (isspace(s_[size - 1])) {
      s_[size - 1] = '\0';
      size--;
      continue;
    }

    if (isspace(*s_)) {
      s_++;
      size--;
      continue;
    }

    break;
  }

  memmove(s, s_, size);
  s[size] = '\0';

  return size;
}

static bool is_section(char *line, char **const section_name) {

  bool r;
  size_t line_size = strlen(line);
  char *token;
  char *saveptr;

  if (!('[' == line[0] && ']' == line[line_size - 1])) {
    r = false;
    goto out;
  }

  line[line_size - 1] = '\0';
  line++;
  line_size -= 2;

  *section_name = strdup(line);
  if (NULL == *section_name) {
    // technically, this is an out-of-memory error condition
    r = false;
    goto out;
  }

  token = strtok_r(line, " ", &saveptr);
  if (NULL == token || !is_identifier(token)) {
    D("not an identifier: '%s'", token);
    r = false;
    goto free_section_name;
  }

  token = strtok_r(NULL, " ", &saveptr);
  if (NULL == token) {
    r = true;
    goto out;
  }

  if (!is_nat0(token)) {
    r = false;
    goto free_section_name;
  }

  r = true;
  goto out;

free_section_name:
  free(*section_name);
  *section_name = NULL;

out:
  return r;
}

static bool is_option(char *line, char **const option_name,
                      char **const string_value) {

  bool r;
  char *token;
  char *saveptr;
  const size_t line_size = strlen(line);

  token = strtok_r(line, " ", &saveptr);
  if (NULL == token || 0 == strlen(token)) {
    r = false;
    goto out;
  }

  if (!is_identifier(token)) {
    r = false;
    goto out;
  }
  *option_name = strdup(token);
  if (NULL == *option_name) {
    // technically this is an out-of-memory error condition
    r = false;
    goto out;
  }

  token = strtok_r(NULL, " ", &saveptr);
  if (NULL == token) {
    r = false;
    goto free_option_name;
  }
  if (0 != strcmp(token, "=")) {
    r = false;
    goto free_option_name;
  }

  token = strtok_r(NULL, " ", &saveptr);
  if (NULL == token) {
    r = false;
    goto free_option_name;
  }

  if (0 == strlen(token)) {
    r = false;
    goto free_option_name;
  }

  // correct the fact that strtok_r() has inserted '\0'
  // in the range [token, line + line_size] to tokenize
  for (char *z = token; z < line + line_size; ++z) {
    if ('\0' == *z) {
      *z = ' ';
    }
  }

  // FIXME: do some kind of UTF-8 validation
  *string_value = strdup(token);
  if (NULL == *string_value) {
    r = false;
    goto free_option_name;
  }

  r = true;
  goto out;

free_option_name:
  free(*option_name);

out:
  return r;
}

static bool is_identifier(const char *identifier) {
  (void)identifier;
  return true;
}

int manifest_check_int(int int_val, uint8_t num_bytes) {
  const int min_ = 0;
  const int max_ = (1 << (8 * num_bytes)) - 1;

  if (int_val < min_ || int_val > max_) {
    return -ERANGE;
  }

  return int_val;
}

int manifest_parse_int(const char *s) {

  long x;
  char *endptr;

  const size_t size = strlen(s);
  const int base[] = {
      10,
      16,
  };

  for (size_t i = 0; i < ARRAY_SIZE(base); ++i) {
    errno = 0;
    x = strtol(s, &endptr, base[i]);
    if ((ERANGE == errno && (LONG_MAX == x || LONG_MIN == x)) ||
        (0 != errno && 0 == x)) {
      continue;
    }
    if ((size_t)(endptr - s) == size && 0 <= x && x <= INT_MAX) {
      return x;
    }
  }

  return -1;
}

static bool is_nat0(const char *s) { return manifest_parse_int(s) >= 0; }

static int manifest_parse_id(manifest_t manifest, const char *section_name,
                             uint8_t num_bytes) {

  int r;
  char *space_pos;
  struct manifest_section *section;
  const struct manifest_ *man = (const struct manifest_ *)manifest;

  for (size_t i = 0; i < man->num_sections; ++i) {
    section = &man->sections[i];
    if (0 == strcmp(section_name, section->name)) {
      space_pos = strrchr(section_name, ' ');
      if (NULL == space_pos) {
        return -EINVAL;
      }
      space_pos++;
      r = manifest_parse_int(space_pos);
      if (-1 == r) {
        return -EINVAL;
      }

      return manifest_check_int(r, num_bytes);
    }
  }

  return -ENOENT;
}
