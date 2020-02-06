/*
 * bdftosfd
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdftosfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-02-06
 *
 * bdftosfd is released under the BSD 2-Clause license
 * See LICENSE file for details
 */

#include <string.h>
#include "parse.h"

int
parseLine(char *lineBuffer) {
	if (*lineBuffer) {
		if (!strncmp(lineBuffer, "FAMILY_NAME ", 12))
			return FAMILY_NAME;

		if (!strncmp(lineBuffer, "COPYRIGHT ", 10))
			return COPYRIGHT;

		if (!strncmp(lineBuffer, "FONTBOUNDINGBOX ", 16))
			return FONTBOUNDINGBOX;

		if (!strncmp(lineBuffer, "FONT_ASCENT ", 12))
			return FONT_ASCENT;

		if (!strncmp(lineBuffer, "FONT_DESCENT ", 12))
			return FONT_DESCENT;

		if (!strncmp(lineBuffer, "FONT_VERSION ", 13))
			return FONT_VERSION;

		if (!strncmp(lineBuffer, "CHARS ", 6))
			return CHARS;

		if (!strncmp(lineBuffer, "STARTCHAR", 9))
			return STARTCHAR;

		if (!strncmp(lineBuffer, "ENCODING", 8))
			return ENCODING;

		if (!strncmp(lineBuffer, "BITMAP", 6))
			return BITMAP;

		if (!strncmp(lineBuffer, "ENDCHAR", 7))
			return ENDCHAR;
	}

	return 0;
}
