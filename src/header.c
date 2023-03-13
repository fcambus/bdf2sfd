/*
 * bdf2sfd 1.1.7
 * Copyright (c) 2019-2023, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-10-08
 *
 * bdf2sfd is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sys/time.h>
#include <stdio.h>

#include "header.h"

void
header(FILE *stream, struct fontinfo *font)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	fprintf(stream, "SplineFontDB: 3.0\n");
	fprintf(stream, "FontName: %s\n", font->psname);
	fprintf(stream, "FullName: %s\n", font->name);
	fprintf(stream, "FamilyName: %s\n", font->name);
	fprintf(stream, "Weight: Medium\n");

	if (font->copyright)
		fprintf(stream, "Copyright: %s\n", font->copyright);

	if (font->version)
		fprintf(stream, "Version: %s\n", font->version);

	fprintf(stream, "ItalicAngle: 0\n"
			"UnderlinePosition: -100\n"
			"UnderlineWidth: 40\n");
	fprintf(stream, "Ascent: %i\n", font->ascent);
	fprintf(stream, "Descent: %i\n", font->descent);
	fprintf(stream, "LayerCount: 2\n"
			"Layer: 0 0 \"Back\"  1\n"
			"Layer: 1 0 \"Fore\"  0\n"
			"FSType: 0\n"
			"OS2Version: 0\n"
			"OS2_WeightWidthSlopeOnly: 0\n"
			"OS2_UseTypoMetrics: 0\n");
	fprintf(stream, "CreationTime: %ld\n", tv.tv_sec);
	fprintf(stream, "PfmFamily: 33\n"
			"TTFWeight: 500\n"
			"TTFWidth: 5\n"
			"LineGap: 72\n"
			"VLineGap: 0\n"
			"Panose: 2 0 6 4 0 0 0 0 0 0\n"
			"OS2TypoAscent: 0\n"
			"OS2TypoAOffset: 1\n"
			"OS2TypoDescent: 0\n"
			"OS2TypoDOffset: 1\n"
			"OS2TypoLinegap: 0\n"
			"OS2WinAscent: 0\n"
			"OS2WinAOffset: 1\n"
			"OS2WinDescent: 0\n"
			"OS2WinDOffset: 1\n"
			"HheadAscent: 0\n"
			"HheadAOffset: 1\n"
			"HheadDescent: 0\n"
			"HheadDOffset: 1\n"
			"OS2Vendor: 'PfEd'\n"
			"Encoding: UnicodeBmp\n"
			"UnicodeInterp: none\n"
			"DisplaySize: -24\n"
			"AntiAlias: 0\n"
			"FitToEm: 1\n"
			"WinInfo: 64 16 4\n"
			"TeXData: 1 0 0 346030 173015 115343 0 1048576 115343 "
			"783286 444596 497025 792723 393216 433062 380633 "
			"303038 157286 324010 404750 52429 2506097 1059062 "
			"262144\n");
	fprintf(stream, "BeginChars: 65536 %s\n\n", font->chars);
}
