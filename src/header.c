/*
 * bdftosfd
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdftosfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-01-20
 *
 * bdftosfd is released under the BSD 2-Clause license
 * See LICENSE file for details
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
	fprintf(stream, "FontName: %s\n", font->name);
	fprintf(stream, "FullName: %s\n", font->name);
	fprintf(stream, "FamilyName: %s\n", font->name);
	fprintf(stream, "Weight: Medium\n");
	fprintf(stream, "Copyright: %s\n", font->copyright);
	fprintf(stream, "Version: %s\n", font->version);
	fprintf(stream, "ItalicAngle: 0\n");
	fprintf(stream, "UnderlinePosition: -100\n");
	fprintf(stream, "UnderlineWidth: 40\n");
	fprintf(stream, "Ascent: 768\n");
	fprintf(stream, "Descent: 256\n");
	fprintf(stream, "LayerCount: 2\n");
	fprintf(stream, "Layer: 0 0 \"Back\"  1\n");
	fprintf(stream, "Layer: 1 0 \"Fore\"  0\n");
	fprintf(stream, "FSType: 0\n");
	fprintf(stream, "OS2Version: 0\n");
	fprintf(stream, "OS2_WeightWidthSlopeOnly: 0\n");
	fprintf(stream, "OS2_UseTypoMetrics: 0\n");
	fprintf(stream, "CreationTime: %ld\n", tv.tv_sec);
	fprintf(stream, "PfmFamily: 33\n");
	fprintf(stream, "TTFWeight: 500\n");
	fprintf(stream, "TTFWidth: 5\n");
	fprintf(stream, "LineGap: 72\n");
	fprintf(stream, "VLineGap: 0\n");
	fprintf(stream, "Panose: 2 0 6 4 0 0 0 0 0 0\n");
	fprintf(stream, "OS2TypoAscent: 0\n");
	fprintf(stream, "OS2TypoAOffset: 1\n");
	fprintf(stream, "OS2TypoDescent: 0\n");
	fprintf(stream, "OS2TypoDOffset: 1\n");
	fprintf(stream, "OS2TypoLinegap: 0\n");
	fprintf(stream, "OS2WinAscent: 0\n");
	fprintf(stream, "OS2WinAOffset: 1\n");
	fprintf(stream, "OS2WinDescent: 0\n");
	fprintf(stream, "OS2WinDOffset: 1\n");
	fprintf(stream, "HheadAscent: 0\n");
	fprintf(stream, "HheadAOffset: 1\n");
	fprintf(stream, "HheadDescent: 0\n");
	fprintf(stream, "HheadDOffset: 1\n");
	fprintf(stream, "OS2Vendor: 'PfEd'\n");
	fprintf(stream, "Encoding: UnicodeBmp\n");
	fprintf(stream, "UnicodeInterp: none\n");
	fprintf(stream, "DisplaySize: -24\n");
	fprintf(stream, "AntiAlias: 0\n");
	fprintf(stream, "FitToEm: 1\n");
	fprintf(stream, "WinInfo: 64 16 4\n");
	fprintf(stream, "TeXData: 1 0 0 346030 173015 115343 0 1048576 115343 783286 444596 497025 792723 393216 433062 380633 303038 157286 324010 404750 52429 2506097 1059062 262144\n");
	fprintf(stream, "BeginChars: 65536 %s\n\n", font->chars);
}
