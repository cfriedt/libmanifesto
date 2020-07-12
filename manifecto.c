/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manifecto/parse-args.h"
#include "manifecto/manifest.h"

int main(int argc, char *argv[]) {

	int r;
	struct ctx ctx;
	manifest_t manifest = NULL;
	FILE *ofp;
	size_t expected_size;
	size_t actual_size;
	uint8_t *mnfb_data;

	ctx_init(&ctx);
	r = parse_args(argc, argv, &ctx);
	if (0 != r) {
		r = EXIT_FAILURE;
		goto cleanup_ctx;
	}

	if (ctx.help) {
		r = EXIT_SUCCESS;
		goto cleanup_ctx;
	}

	manifest = manifest_new();
	r = manifest_mnfs_parse_file(manifest, ctx.infile);
	if (r < 0) {
		fprintf(stderr, "manifest_mnfs_parse_file() failed: %d (%s)\n", r, strerror(-r));
		goto cleanup_manifest;
	}

	r = manifest_mnfb_gen(manifest);
	if (r < 0) {
		fprintf(stderr, "manifest_mnfb_gen() failed: %d (%s)\n", r, strerror(-r));
		goto cleanup_manifest;
	}

	ofp = fopen(ctx.outfile, "w");
	if (NULL == ofp) {
		r = EXIT_FAILURE;
		goto cleanup_manifest;
	}

	expected_size = manifest_mnfb_size(manifest);
	mnfb_data = manifest_mnfb_data(manifest);
	
	if (0 == expected_size || NULL == mnfb_data) {
		r = EXIT_FAILURE;
		goto cleanup_output;
	}
	
	actual_size = fwrite(mnfb_data, 1, expected_size, ofp);
	if (actual_size != expected_size) {
		r = EXIT_FAILURE;
		goto cleanup_output;
	}

	r = EXIT_SUCCESS;

cleanup_output:
	fclose(ofp);
	if (EXIT_SUCCESS != r) {
		remove(ctx.outfile);
	}

cleanup_manifest:
	manifest_fini(&manifest);

cleanup_ctx:
	ctx_fini(&ctx);

	return r;
}
