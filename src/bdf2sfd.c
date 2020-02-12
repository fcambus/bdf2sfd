/*
 * bdf2sfd 1.1.0
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-02-12
 *
 * bdf2sfd is released under the BSD 2-Clause license
 * See LICENSE file for details
 */

#define _XOPEN_SOURCE 600
#define _POSIX_C_SOURCE 199309L
#define _POSIX_SOURCE

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

#include "compat.h"
#include "config.h"
#include "header.h"
#include "parse.h"
#include "polygon.h"

struct timespec begin, end, elapsed;

char lineBuffer[LINE_LENGTH_MAX];

FILE *bdfFile;
struct stat bdfFileStat;

const char *errstr;

int8_t getoptFlag;

char *intputFile;

uint64_t glyphes;
double runtime;

void
displayUsage()
{
	printf("bdf2sfd [-hv] [-f name] [-p name] file \n\n"
	    "The options are as follows:\n\n"
	    "     -f name     Specify font name.\n"
	    "     -p name     Specify PostScript font name.\n"
	    "     -h          Display usage.\n"
	    "     -v          Display version.\n");
}

int
main(int argc, char *argv[])
{
	bool readglyph = false;

	char *value = NULL;

	uint32_t height = 0, width = 0;
	uint32_t ascent = 0, descent = 0;

	int key;
	float x = 0.0, y = 0.0;
	uint32_t mask = 0;
	float xlength = 64.0, ylength = 64.0; /* Default values for 8x16 fonts */

	struct fontinfo font;
	memset(&font, 0, sizeof(struct fontinfo));

	if (pledge("stdio rpath", NULL) == -1) {
		err(EXIT_FAILURE, "pledge");
	}

	while ((getoptFlag = getopt(argc, argv, "f:p:hv")) != -1) {
		switch (getoptFlag) {
		case 'f':
			font.name = optarg;
			break;

		case 'p':
			font.psname = optarg;
			break;

		case 'h':
			displayUsage();
			return EXIT_SUCCESS;

		case 'v':
			printf("%s\n", VERSION);
			return EXIT_SUCCESS;
		}
	}

	if (optind < argc) {
		intputFile = argv[optind];
	} else {
		displayUsage();
		return EXIT_SUCCESS;
	}

	argc -= optind;
	argv += optind;

	/* Starting timer */
	clock_gettime(CLOCK_MONOTONIC, &begin);

	/* Open BDF file */
	if (!strcmp(intputFile, "-")) {
		/* Read from standard input */
		bdfFile = stdin;
	} else {
		/* Attempt to read from file */
		if (!(bdfFile = fopen(intputFile, "r"))) {
			perror("Can't open BDF file");
			return EXIT_FAILURE;
		}
	}

	/* Get BDF file size */
	if (fstat(fileno(bdfFile), &bdfFileStat)) {
		perror("Can't stat BDF file");
		return EXIT_FAILURE;
	}

	while (fgets(lineBuffer, LINE_LENGTH_MAX, bdfFile)) {
		if (!*lineBuffer)
			continue;

		key = parseLine(lineBuffer);

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
			if (!font.name)
				font.name = strdup(value);

			if (!font.psname)
				font.psname = strdup(value);

			continue;

		case COPYRIGHT:
			font.copyright = strdup(value);

			continue;

		case FONTBOUNDINGBOX:
			value = strtok(NULL, " \t");

			if (value)
				width = strtonum(value, 0, 32, &errstr);

			if (errstr)
				errx(EXIT_FAILURE, "Invalid value for FONTBOUNDINGBOX.");

			value = strtok(NULL, " \t");

			if (value)
				height = strtonum(value, 0, 64, &errstr);

			if (errstr)
				errx(EXIT_FAILURE, "Invalid value for FONTBOUNDINGBOX.");

			if (!width || !height)
				errx(EXIT_FAILURE, "Invalid value for FONTBOUNDINGBOX.");

			xlength = 512.0 / width;
			ylength = 1024.0 / height;

			mask = 1 << (stride[width] * 8 - 1);

			continue;

		case FONT_ASCENT:
			ascent = strtonum(value, 0, 64, &errstr);

			if (!errstr)
				font.ascent = ascent * ylength;
			else
				errx(EXIT_FAILURE, "Invalid value for FONT_ASCENT.");

			continue;

		case FONT_DESCENT:
			descent = strtonum(value, 0, 64, &errstr);

			if (!errstr)
				font.descent = descent * ylength;
			else
				errx(EXIT_FAILURE, "Invalid value for FONT_DESCENT.");

			continue;

		case FONT_VERSION:
			font.version = strdup(value);

			continue;

		case CHARS:
			font.chars = value;

			if (font.chars)
				header(stdout, &font);
			else
				errx(EXIT_FAILURE, "Invalid value for CHARS.");

			continue;

		case STARTCHAR:
			fprintf(stdout, "StartChar: %s", value);

			continue;

		case ENCODING:
			fprintf(stdout, "\nEncoding: %s %s %s\n", value, value, value);

			continue;

		case BITMAP:
			fprintf(stdout, "Width: 512\n"
					"Flags: HW\n"
					"LayerCount: 2\n"
					"Fore\n"
					"SplineSet\n");

			y = font.ascent;
			readglyph = true;
			glyphes++;

			continue;

		case ENDCHAR:
			fprintf(stdout, "EndSplineSet\n"
					"EndChar\n\n");

			readglyph = false;
			continue;
		}

		if (readglyph) {
			uint32_t row = strtoul(lineBuffer, NULL, 16);

			polygon(row, mask, width, x, y, xlength, ylength);
			
			y -= ylength;
		}
	}

	fprintf(stdout, "EndChars\n"
			"EndSplineFont\n");

	/* Stopping timer */
	clock_gettime(CLOCK_MONOTONIC, &end);

	timespecsub(&end, &begin, &elapsed);
	runtime = elapsed.tv_sec + elapsed.tv_nsec / 1E9;

	/* Printing results */
	fprintf(stderr, "Processed %" PRIu64 " glyphes in %f seconds.\n", glyphes, runtime);

	/* Clean up */
	fclose(bdfFile);

	return EXIT_SUCCESS;
}
