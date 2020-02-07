/*
 * bdf2sfd
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-01-21
 *
 * bdf2sfd is released under the BSD 2-Clause license
 * See LICENSE file for details
 */

#ifndef HEADER_H
#define HEADER_H

#include <inttypes.h>

struct fontinfo {
	char *name;
	char *chars;
	char *copyright;
	char *version;
	int32_t ascent;
	int32_t descent;
};

void header(FILE *, struct fontinfo *);

#endif /* HEADER_H */
