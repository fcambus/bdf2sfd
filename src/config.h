/*
 * bdf2sfd 1.0.0
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-02-08
 *
 * bdf2sfd is released under the BSD 2-Clause license
 * See LICENSE file for details
 */

#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "bdf2sfd 1.0.0"

enum {
	LINE_LENGTH_MAX = 65536
};

int stride[] = {
	0,
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4
};

#endif /* CONFIG_H */
