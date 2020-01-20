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

#ifndef HEADER_H
#define HEADER_H

struct fontinfo {
	char *name;
	char *chars;
	char *copyright;
};

void header(FILE *, struct fontinfo *);

#endif /* HEADER_H */
