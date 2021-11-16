/*
 * bdf2sfd 1.1.6
 * Copyright (c) 2019-2021, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2021-11-16
 *
 * bdf2sfd is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <err.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef HAVE_SECCOMP
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include "seccomp.h"
#endif

#include "compat.h"
#include "config.h"
#include "header.h"
#include "parse.h"
#include "polygon.h"

static void
usage()
{
	printf("bdf2sfd [-hv] [-f name] [-p name] font.bdf\n\n"
	    "The options are as follows:\n\n"
	    "	-f name	Specify font name.\n"
	    "	-p name	Specify PostScript font name.\n"
	    "	-h	Display usage.\n"
	    "	-v	Display version.\n");
}

static void
error(const char *str)
{
	errx(EXIT_FAILURE, "%s", str);
}

int
main(int argc, char *argv[])
{
	struct timespec begin, current, elapsed;
	struct stat bdf_stat;

	float x = 0.0, y = 0.0;
	float xlength = 64.0, ylength = 64.0; /* Default for 8x16 fonts */

	uint64_t glyphs = 0;
	uint32_t height = 0, width = 0;
	uint32_t ascent = 0, descent = 0;
	uint32_t mask = 0;

	int opt, key, stride;

	const char *errstr = NULL;
	char *input;
	char *value = NULL;
	char *linebuffer = NULL;
	size_t linesize = 0;

	bool readglyph = false;
	bool name_allocated = false, psname_allocated = false;

	FILE *bdf;

	struct fontinfo font;
	memset(&font, 0, sizeof(struct fontinfo));

	if (pledge("stdio rpath", NULL) == -1) {
		err(EXIT_FAILURE, "pledge");
	}

#ifdef HAVE_SECCOMP
	if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
		perror("Can't initialize seccomp");
		return EXIT_FAILURE;
	}

	if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &bdf2sfd)) {
		perror("Can't load seccomp filter");
		return EXIT_FAILURE;
	}
#endif

	while ((opt = getopt(argc, argv, "f:p:hv")) != -1) {
		switch (opt) {
		case 'f':
			font.name = optarg;
			break;

		case 'p':
			font.psname = optarg;
			break;

		case 'h':
			usage();
			return EXIT_SUCCESS;

		case 'v':
			printf("%s\n", VERSION);
			return EXIT_SUCCESS;
		}
	}

	if (optind < argc) {
		input = argv[optind];
	} else {
		usage();
		return EXIT_SUCCESS;
	}

	/* Starting timer */
	clock_gettime(CLOCK_MONOTONIC, &begin);

	/* Open BDF file */
	if (!strcmp(input, "-")) {
		/* Read from standard input */
		bdf = stdin;
	} else {
		/* Attempt to read from file */
		if (!(bdf = fopen(input, "r"))) {
			perror("Can't open BDF file");
			return EXIT_FAILURE;
		}
	}

	/* Get BDF file size */
	if (fstat(fileno(bdf), &bdf_stat)) {
		perror("Can't stat BDF file");
		return EXIT_FAILURE;
	}

	while (getline(&linebuffer, &linesize, bdf) != -1) {
		if (!*linebuffer)
			continue;

		key = parseLine(linebuffer);

		switch(key) {
		case FAMILY_NAME:
		case COPYRIGHT:
		case FONT_ASCENT:
		case FONT_DESCENT:
		case FONT_VERSION:
		case CHARS:
		case STARTCHAR:
		case ENCODING:
			value = strtok(NULL, "\n");

			if (!value)
				continue;
		}

		switch(key) {
		case FAMILY_NAME:
			if (!font.name) {
				if ((font.name = strdup(value)) == NULL)
					error("Memory allocation error.");

				name_allocated = true;
			}

			if (!font.psname) {
				if ((font.psname = strdup(value)) == NULL)
					error("Memory allocation error.");

				psname_allocated = true;
			}

			continue;

		case COPYRIGHT:
			if ((font.copyright = strdup(value)) == NULL)
				error("Memory allocation error.");

			continue;

		case FONTBOUNDINGBOX:
			value = strtok(NULL, " \t");

			if (value)
				width = strtonum(value, 0, 32, &errstr);

			if (errstr)
				error("Invalid value for FONTBOUNDINGBOX.");

			value = strtok(NULL, " \t");

			if (value)
				height = strtonum(value, 0, 64, &errstr);

			if (errstr)
				error("Invalid value for FONTBOUNDINGBOX.");

			if (!width || !height)
				error("Invalid value for FONTBOUNDINGBOX.");

			xlength = 512.0 / width;
			ylength = 1024.0 / height;

			stride = (width + 7) / 8;
			mask = 1 << (stride * 8 - 1);

			continue;

		case FONT_ASCENT:
			ascent = strtonum(value, 0, 64, &errstr);

			if (!errstr)
				font.ascent = ascent * ylength;
			else
				error("Invalid value for FONT_ASCENT.");

			continue;

		case FONT_DESCENT:
			descent = strtonum(value, 0, 64, &errstr);

			if (!errstr)
				font.descent = descent * ylength;
			else
				error("Invalid value for FONT_DESCENT.");

			continue;

		case FONT_VERSION:
			if ((font.version = strdup(value)) == NULL)
				error("Memory allocation error.");

			continue;

		case CHARS:
			font.chars = value;

			if (font.chars)
				header(stdout, &font);
			else
				error("Invalid value for CHARS.");

			continue;

		case STARTCHAR:
			fprintf(stdout, "StartChar: %s", value);

			continue;

		case ENCODING:
			fprintf(stdout, "\nEncoding: %s %s %s\n",
			    value, value, value);

			continue;

		case BITMAP:
			fprintf(stdout, "Width: 512\n"
					"Flags: HW\n"
					"LayerCount: 2\n"
					"Fore\n"
					"SplineSet\n");

			y = font.ascent;
			readglyph = true;
			glyphs++;

			continue;

		case ENDCHAR:
			fprintf(stdout, "EndSplineSet\n"
					"EndChar\n\n");

			readglyph = false;
			continue;
		}

		if (readglyph) {
			uint32_t row = strtoul(linebuffer, NULL, 16);

			polygon(row, mask, width, x, y, xlength, ylength);
			
			y -= ylength;
		}
	}

	fprintf(stdout, "EndChars\n"
			"EndSplineFont\n");

	/* Stopping timer */
	clock_gettime(CLOCK_MONOTONIC, &current);

	timespecsub(&current, &begin, &elapsed);

	/* Printing results */
	fprintf(stderr, "Processed %" PRIu64 " glyphs in %f seconds.\n",
	    glyphs, elapsed.tv_sec + elapsed.tv_nsec / 1E9);

	/* Clean up */
	free(linebuffer);
	fclose(bdf);

	if (name_allocated)
		free(font.name);

	if (psname_allocated)
		free(font.psname);

	free(font.copyright);
	free(font.version);

	return EXIT_SUCCESS;
}
