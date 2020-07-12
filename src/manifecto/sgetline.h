/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SGETLINE_H_
#define SGETLINE_H_

#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

ssize_t sgetdelim(char **lineptr, size_t *n, const int delim,
                  const char *const stream, const size_t stream_size);

static inline ssize_t sgetline(char **lineptr, size_t *n,
                               const char *const stream,
                               const size_t stream_size) {
  return sgetdelim(lineptr, n, '\n', stream, stream_size);
}

__END_DECLS

#endif /* SGETLINE_H_ */
