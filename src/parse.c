/*
 * bdf2sfd 1.1.4
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-02-08
 *
 * bdf2sfd is released under the BSD 2-Clause license
 * See LICENSE file for details
 */

#include <string.h>
#include "parse.h"

int
parseLine(char *lineBuffer) {
	char *key = NULL;

	if (*lineBuffer) {
		key = strtok(lineBuffer, " \t\n");

		if (key) {
			if (!strcmp(key, "FAMILY_NAME"))
				return FAMILY_NAME;

			if (!strcmp(key, "COPYRIGHT"))
				return COPYRIGHT;

			if (!strcmp(key, "FONTBOUNDINGBOX"))
				return FONTBOUNDINGBOX;

			if (!strcmp(key, "FONT_ASCENT"))
				return FONT_ASCENT;

			if (!strcmp(key, "FONT_DESCENT"))
				return FONT_DESCENT;

			if (!strcmp(key, "FONT_VERSION"))
				return FONT_VERSION;

			if (!strcmp(key, "CHARS"))
				return CHARS;

			if (!strcmp(key, "STARTCHAR"))
				return STARTCHAR;

			if (!strcmp(key, "ENCODING"))
				return ENCODING;

			if (!strcmp(key, "BITMAP"))
				return BITMAP;

			if (!strcmp(key, "ENDCHAR"))
				return ENDCHAR;
		}
	}

	return 0;
}
