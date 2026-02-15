/*
 * bdf2sfd 1.2.0
 * Copyright (c) 2019-2026, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-02-12
 *
 * bdf2sfd is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>
#include <stdio.h>

struct fontinfo {
	char *name;
	char *psname;
	char *chars;
	char *copyright;
	char *version;
	int32_t ascent;
	int32_t descent;
};

void header(FILE *, struct fontinfo *);

#endif /* HEADER_H */
