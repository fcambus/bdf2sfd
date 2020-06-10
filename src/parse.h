/*
 * bdf2sfd 1.1.2
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-02-08
 *
 * bdf2sfd is released under the BSD 2-Clause license
 * See LICENSE file for details
 */

#ifndef PARSE_H
#define PARSE_H

#define FAMILY_NAME	1
#define COPYRIGHT	2
#define FONTBOUNDINGBOX	3
#define FONT_ASCENT	4
#define FONT_DESCENT	5
#define FONT_VERSION	6
#define CHARS		7
#define STARTCHAR	8
#define ENCODING	9
#define BITMAP		10
#define ENDCHAR		11

int parseLine(char *);

#endif /* PARSE_H */
