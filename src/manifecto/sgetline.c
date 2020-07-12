/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sgetline.h"

ssize_t sgetdelim(char **lineptr, size_t *n, const int delim,
                  const char *const stream, const size_t stream_size) {

  // return value
  ssize_t r;
  // length of the current line
  size_t m;
  void *tmp;

  if (NULL == lineptr || NULL == n || NULL == stream) {
    errno = -EINVAL;
    r = -1;
    goto out;
  }

  if (0 == stream_size) {
    // end-of-stream
    return -1;
  }

  for (m = 0; m < stream_size; ++m) {
    if (delim == stream[m]) {
      m++;
      break;
    }
  }

  if (NULL == *lineptr || *n < m + 1) {
    tmp = realloc(*lineptr, m + 1);
    if (NULL == tmp) {
      r = -ENOMEM;
      goto out;
    }
    *lineptr = tmp;
    *n = m + 1;
  }
  memmove(*lineptr, stream, m);
  (*lineptr)[m] = '\0';

  r = m;

out:
  return r;
}
