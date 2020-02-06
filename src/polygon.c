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

#include <inttypes.h>
#include <stdio.h>

void
polygon(uint32_t row, uint32_t mask, uint32_t width, int32_t x, int32_t y, uint32_t xlength, uint32_t ylength) {
	for (size_t column = 0; column < width; column++) {
		if ((row & (mask >> column)) != 0) {
			x = column * xlength;
			fprintf(stdout, "%d %d m 1\n"
					" %d %d l 1\n"
					" %d %d l 1\n"
					" %d %d l 1\n"
					" %d %d l 1\n",
					x, y,
					x, y - ylength,
					x + xlength, y - ylength,
					x + xlength, y,
					x, y);
		}
	}
}
