/*
 * bdf2sfd
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-02-07
 *
 * bdf2sfd is released under the BSD 2-Clause license
 * See LICENSE file for details
 */

#include <inttypes.h>
#include <stdio.h>

void
polygon(uint32_t row, uint32_t mask, uint32_t width, float x, float y, float xlength, float ylength) {
	for (size_t column = 0; column < width; column++) {
		if ((row & (mask >> column)) != 0) {
			x = column * xlength;
			fprintf(stdout, "%f %f m 1\n"
					" %f %f l 1\n"
					" %f %f l 1\n"
					" %f %f l 1\n"
					" %f %f l 1\n",
					x, y,
					x, y - ylength,
					x + xlength, y - ylength,
					x + xlength, y,
					x, y);
		}
	}
}
