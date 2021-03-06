/*
 * OpenTyrian: A modern cross-platform port of Tyrian
 * Copyright (C) 2007-2009  The OpenTyrian Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "picload.h"
#include "file.h"
#include "palette.h"
#include "pcxmast.h"
#include "video.h"

#include <string.h>

void JE_loadPic(SDL_Surface *screen, uint8_t PCXnumber, bool storepal )
{
	PCXnumber--;

	FILE *f = dir_fopen_die(data_dir(), "tyrian.pic", "rb");

	static bool first = true;
	if (first)
	{
		first = false;

		uint16_t temp;
		efread(&temp, sizeof(uint16_t), 1, f);
		for (int i = 0; i < PCX_NUM; i++)
		{
			efread(&pcxpos[i], sizeof(int32_t), 1, f);
		}

		pcxpos[PCX_NUM] = ftell_eof(f);
	}

	uint32_t size = pcxpos[PCXnumber + 1] - pcxpos[PCXnumber];
	uint8_t *buffer = malloc(size);

	fseek(f, pcxpos[PCXnumber], SEEK_SET);
	efread(buffer, sizeof(uint8_t), size, f);
	fclose(f);

	uint8_t *p = buffer;
	uint8_t *s; /* screen pointer, 8-bit specific */

	s = (uint8_t *)screen->pixels;

	for (int i = 0; i < 320 * 200; )
	{
		if ((*p & 0xc0) == 0xc0)
		{
			i += (*p & 0x3f);
			memset(s, *(p + 1), (*p & 0x3f));
			s += (*p & 0x3f); p += 2;
		} else {
			i++;
			*s = *p;
			s++; p++;
		}
		if (i && (i % 320 == 0))
		{
			s += screen->pitch - 320;
		}
	}

	free(buffer);

	memcpy(colors, palettes[pcxpal[PCXnumber]], sizeof(colors));

	if (storepal)
		set_palette(colors, 0, 255);
}

