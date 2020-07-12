/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "parse-args.h"

#define MNFS_EXT ".mnfs"
#define MNFS_EXT_SIZE 4

#define MNFB_EXT ".mnfb"
#define MNFB_EXT_SIZE 4

// ugh... strdup(3) is not standard c. Doesn't get declared with -std=c11
extern char *strdup(const char *s);

void usage(const char *progname) {
  fprintf(stderr,
          "usage: %s "
          " [-h]"
          " [-I {mnfs,mnfb}]"
          " [-o OUT]"
          " [-O {mnfs,mnfb}] "
          " [-s]"
          " infile"
          "\n",
          progname);
}

void ctx_init(struct ctx *ctx) {
  struct ctx tmp = CTX_INITIALIZER;
  memcpy(ctx, &tmp, sizeof(tmp));
}

void ctx_fini(struct ctx *ctx) { free(ctx->outfile); }

extern char *optarg;
extern int optind, opterr, optopt;

int parse_args(int argc, char *const argv[], struct ctx *const ctx) {

  const char *basename_s = basename(argv[0]);

  int opt;
  int r;

  struct stat statbuf = {};

  for (;;) {
    opt = getopt(argc, argv, "hI:o:O:s");

    if (false) {
    } else if (-1 == opt) {
      ctx->num_positional = argc - optind;
      break;
    } else if ('?' == opt) {
      fprintf(stderr, "error processing options\n");
      return -EINVAL;
    } else if ('h' == opt) {
      ctx->help = true;
    } else if ('I' == opt) {
      if (false) {
      } else if (0 == strcmp("mnfs", optarg)) {
        ctx->output_binary = false;
      } else if (0 == strcmp("mnfb", optarg)) {
        ctx->output_binary = true;
      } else {
        fprintf(stderr, "invalid input format '%s'\n", optarg);
        return -EINVAL;
      }
    } else if ('o' == opt) {
      ctx->outfile = strdup(optarg);
    } else if ('O' == opt) {
      if (false) {
      } else if (0 == strcmp("mnfs", optarg)) {
        ctx->output_binary = false;
      } else if (0 == strcmp("mnfb", optarg)) {
        ctx->output_binary = true;
      } else {
        fprintf(stderr, "invalid output format '%s'\n", optarg);
        return -EINVAL;
      }
    } else if ('s' == opt) {
      ctx->silent = true;
    } else {
      fprintf(stderr, "unrecognized option '%s'\n", argv[optind - 1]);
      continue;
    }
  }

  if (ctx->help) {
    usage(basename_s);
    return 0;
  }

  //
  // Validate Input File
  //

  if (false) {
  } else if (ctx->num_positional <= 0) {
    usage(basename_s);
    fprintf(stderr, "please specify an argument for 'infile'\n");
    return -EINVAL;
  } else if (ctx->num_positional >= 2) {
    usage(basename_s);
    fprintf(stderr, "only one positional argument for 'infile'\n");
    return -EINVAL;
  } else {
    ctx->infile = argv[argc - 1];
    if (NULL == ctx->outfile) {
      ctx->outfile = strdup(ctx->infile);
      ctx->outfile[strlen(ctx->outfile) - 1] = 'b';
    }
  }

  if (ctx->input_binary) {
    fprintf(stderr, "binary input is not yet supported\n");
    return -EINVAL;
  } else {

    if (strlen(ctx->infile) < MNFS_EXT_SIZE) {
      fprintf(stderr, "input file name '%s' has invalid length\n", ctx->infile);
      return -EINVAL;
    }

    if (0 != strcmp(MNFS_EXT,
                    &ctx->infile[strlen(ctx->infile) - MNFS_EXT_SIZE - 1])) {
      fprintf(stderr, "input file '%s' has incorrect extension\n", ctx->infile);
      return -EINVAL;
    }
  }
  r = stat(ctx->infile, &statbuf);
  if (-1 == r) {
    perror("stat");
    return -errno;
  }

  //
  // Validate Output File
  //

  if (ctx->output_binary) {
    if (strlen(ctx->outfile) < MNFB_EXT_SIZE) {
      fprintf(stderr, "input file name '%s' has invalid length\n", ctx->infile);
      return -EINVAL;
    }

    if (0 != strcmp(MNFB_EXT,
                    &ctx->outfile[strlen(ctx->outfile) - MNFB_EXT_SIZE - 1])) {
      fprintf(stderr, "input file '%s' has incorrect extension", ctx->infile);
      return -EINVAL;
    }
  } else {
    fprintf(stderr, "mnfs output is not yet supported\n");
    return -EINVAL;
  }
  r = stat(ctx->outfile, &statbuf);
  if (0 == r) {
    fprintf(stderr, "output file '%s' already exists\n", ctx->outfile);
    return -EALREADY;
  }

  return 0;
}
