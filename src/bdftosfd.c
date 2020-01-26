/*
 * bdftosfd
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdftosfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-01-26
 *
 * bdftosfd is released under the BSD 2-Clause license
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
	printf("USAGE: bdftosfd [options] inputfile\n\n" \
	    "Options are:\n\n" \
	    "	-h Display usage\n" \
	    "	-v Display version\n");
}

int
main(int argc, char *argv[])
{
	if (pledge("stdio rpath", NULL) == -1) {
		err(EXIT_FAILURE, "pledge");
	}

	while ((getoptFlag = getopt(argc, argv, "d:ghv")) != -1) {
		switch (getoptFlag) {
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

	bool readglyph = false;
	char *token = NULL;
	char *charname = NULL, *copyright = NULL, *name = NULL, *encoding = NULL, *version = NULL;

	char *value = NULL;
	int32_t height = 0, weight = 0;
	int32_t ascent = 0, descent = 0;

	int32_t x = 0, y = 0;

	struct fontinfo font;
	memset(&font, 0, sizeof(struct fontinfo));

	while (fgets(lineBuffer, LINE_LENGTH_MAX, bdfFile)) {
		if (*lineBuffer) {
			if (!strncmp(lineBuffer, "FAMILY_NAME ", 12)) {
				token = strtok(lineBuffer, " \t");

				if (token)
					name = strtok(NULL, "\n");

				if (name)
					font.name = strdup(name);

				continue;
			}

			if (!strncmp(lineBuffer, "COPYRIGHT ", 10)) {
				token = strtok(lineBuffer, " \t");

				if (token)
					copyright = strtok(NULL, "\n");

				if (copyright)
					font.copyright = strdup(copyright);

				continue;
			}

			if (!strncmp(lineBuffer, "FONTBOUNDINGBOX ", 16)) {
				token = strtok(lineBuffer, " \t");

				if (token)
					value = strtok(NULL, " \t");

				if (value)
					weight = strtonum(value, 0, 8, &errstr);

				if (errstr)
					errx(EXIT_FAILURE, "Invalid value for FONTBOUNDINGBOX.");

				value = strtok(NULL, " \t");

				if (value)
					height = strtonum(value, 0, 16, &errstr);

				if (errstr)
					errx(EXIT_FAILURE, "Invalid value for FONTBOUNDINGBOX.");

				continue;
			}

			if (!strncmp(lineBuffer, "FONT_ASCENT ", 12)) {
				token = strtok(lineBuffer, " \t");

				if (token)
					value = strtok(NULL, "\n");

				if (value)
					ascent = strtonum(value, 0, 16, &errstr);

				if (!errstr)
					font.ascent = ascent * 64;
				else
					errx(EXIT_FAILURE, "Invalid value for FONT_ASCENT.");

				continue;
			}

			if (!strncmp(lineBuffer, "FONT_DESCENT ", 12)) {
				token = strtok(lineBuffer, " \t");

				if (token)
					value = strtok(NULL, "\n");

				if (value)
					descent = strtonum(value, 0, 16, &errstr);

				if (!errstr)
					font.descent = descent * 64;
				else
					errx(EXIT_FAILURE, "Invalid value for FONT_DESCENT.");

				continue;
			}

			if (!strncmp(lineBuffer, "FONT_VERSION ", 13)) {
				token = strtok(lineBuffer, " \t");

				if (token)
					version = strtok(NULL, "\n");

				if (version)
					font.version = strdup(version);

				continue;
			}

			if (!strncmp(lineBuffer, "CHARS ", 6)) {
				token = strtok(lineBuffer, " \t");

				if (token)
					font.chars = strtok(NULL, " \n");

				if (font.chars)
					header(stdout, &font);
				else
					errx(EXIT_FAILURE, "Invalid value for CHARS.");

				continue;
			}

			if (!strncmp(lineBuffer, "STARTCHAR", 9)) {
				fprintf(stdout, "StartChar:");
				token = strtok(lineBuffer, " \t");

				if (token) {
					charname = strtok(NULL, " \n");

					while (charname) {
						fprintf(stdout, " %s", charname);
						charname = strtok(NULL, " \n");
					}
				}

				continue;
			}

			if (!strncmp(lineBuffer, "ENCODING", 8)) {
				token = strtok(lineBuffer, " \t");

				if (token)
					encoding = strtok(NULL, " \n");

				if (encoding)
					fprintf(stdout, "\nEncoding: %s %s %s\n", encoding, encoding, encoding);

				continue;
			}

			if (!strncmp(lineBuffer, "BITMAP", 6)) {
				fprintf(stdout, "Width: 512\n"
						"Flags: HW\n"
						"LayerCount: 2\n"
						"Fore\n"
						"SplineSet\n");

				y = 704;
				readglyph = true;
				glyphes++;

				continue;
			}

			if (!strncmp(lineBuffer, "ENDCHAR", 7)) {
				fprintf(stdout, "EndSplineSet\n"
						"EndChar\n\n");

				readglyph = false;

				continue;
			}

			if (readglyph) {
				int32_t row = strtol(lineBuffer, NULL, 16);

				for (size_t column = 0; column < 8; column++) {
					if ((row & (0x80 >> column)) != 0) {
						x = column * 64;
						fprintf(stdout, "%d %d m 25\n", x, y);
						fprintf(stdout, " %d %d l 25\n", x, y + 64);
						fprintf(stdout, " %d %d l 25\n", x + 64, y + 64);
						fprintf(stdout, " %d %d l 25\n", x + 64, y);
						fprintf(stdout, " %d %d l 25\n", x, y);
					}
				}

				y -= 64;
			}
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
