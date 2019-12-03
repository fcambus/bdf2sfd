/*
 * bdftosfd
 * Copyright (c) 2019, Frederic Cambus
 * https://github.com/fcambus/bdftosfd
 *
 * Created:      2019-11-21
 * Last Updated: 2019-12-03
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
displayUsage() {
	printf("USAGE: bdftosfd [options] inputfile\n\n" \
	    "Options are:\n\n" \
	    "	-h Display usage\n" \
	    "	-v Display version\n");
}

int
main(int argc, char *argv[]) {
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
	char *encoding;

	while (fgets(lineBuffer, LINE_LENGTH_MAX, bdfFile)) {
		if (*lineBuffer) {
			if (!strncmp(lineBuffer, "ENCODING", 8)) {
				token = strtok(lineBuffer, " \t");

				if (token)
					encoding = strtok(NULL, " \n");

				if (encoding)
					fprintf(stdout, "Encoding: %s %s %s\n", encoding, encoding, encoding);

				continue;
			}

			if (!strncmp(lineBuffer, "BITMAP", 6)) {
				readglyph = true;
				glyphes++;
				continue;
			}
			if (!strncmp(lineBuffer, "ENDCHAR", 7)) {
				readglyph = false;
				continue;
			}
		}
	}

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
