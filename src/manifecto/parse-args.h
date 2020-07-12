/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MANIFECTO_PARSE_ARGS_H_
#define MANIFECTO_PARSE_ARGS_H_

#include <stdbool.h>
#include <stddef.h>

#include <sys/cdefs.h>

__BEGIN_DECLS

struct ctx {
  char *infile;
  char *outfile;
  int num_positional;
  int help;
  int input_binary;
  int output_binary;
  int silent;
};

#define CTX_INITIALIZER                                                        \
  {                                                                            \
    .infile = NULL, .outfile = NULL, .num_positional = 0, .help = false,       \
    .input_binary = false, .output_binary = true, .silent = false,             \
  }

void ctx_init(struct ctx *ctx);
void ctx_fini(struct ctx *ctx);

void usage(const char *progname);
int parse_args(int argc, char *const argv[], struct ctx *const ctx);

__END_DECLS

#endif /* MANIFECTO_PARSE_ARGS_H_ */
