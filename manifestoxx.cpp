/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <system_error>
#include <string>

#include "manifecto/parse-args.h"
#include "manifestoxx/Manifest.h"
#include "manifestoxx/MnfbGenerator.h"
#include "manifestoxx/MnfsParser.h"

using namespace std;

int main(int argc, char *argv[]) {

	::ctx ctx = CTX_INITIALIZER;
	Manifest manifest;
	MnfbGenerator gen;
	MnfsParser parser;
	ofstream ofs;

	try {

		int r = parse_args(argc, argv, &ctx);
		if (0 != r) {
			ctx_fini(&ctx);
			return EXIT_FAILURE;
		}

		if (ctx.help) {
			ctx_fini(&ctx);
			return EXIT_SUCCESS;
		}

		manifest = parser.parse_file(ctx.infile);
		gen.generate(manifest);

		ofs.open(ctx.outfile, ios::out | ios::binary);
		ofs.write((const char *)&gen.mnfb.front(), gen.mnfb.size());
		ofs.close();

	} catch(const exception & e) {

		cerr << "Caught Exception '" << e.what() << "'" << endl << endl;
		return EXIT_FAILURE;
	}
}
