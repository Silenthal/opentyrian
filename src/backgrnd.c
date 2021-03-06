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
#include "opentyr.h"
#include "backgrnd.h"

#include "config.h"
#include "varz.h"
#include "video.h"

#include <assert.h>

/*Special Background 2 and Background 3*/

/*Back Pos 3*/
uint16_t backPos, backPos2, backPos3;
uint16_t backMove, backMove2, backMove3;

/*Main Maps*/
uint16_t mapX, mapY, mapX2, mapX3, mapY2, mapY3;
uint8_t **mapYPos, **mapY2Pos, **mapY3Pos;
uint16_t mapXPos, oldMapXOfs, mapXOfs, mapX2Ofs, mapX2Pos, mapX3Pos, oldMapX3Ofs, mapX3Ofs, tempMapXOfs;
intptr_t mapXbpPos, mapX2bpPos, mapX3bpPos;
uint8_t map1YDelay, map1YDelayMax, map2YDelay, map2YDelayMax;


bool  anySmoothies;
uint8_t     smoothie_data[9]; /* [1..9] */

void JE_darkenBackground( uint16_t neat )  /* wild detail level */
{
	uint8_t *s = VGAScreen->pixels; /* screen pointer, 8-bit specific */
	int x, y;
	
	s += 24;
	
	for (y = 184; y; y--)
	{
		for (x = 264; x; x--)
		{
			*s = ((((*s & 0x0f) << 4) - (*s & 0x0f) + ((((x - neat - y) >> 2) + *(s-2) + (y == 184 ? 0 : *(s-(VGAScreen->pitch-1)))) & 0x0f)) >> 4) | (*s & 0xf0);
			s++;
		}
		s += VGAScreen->pitch - 264;
	}
}

void blit_background_row( SDL_Surface *surface, int x, int y, uint8_t **map )
{
	assert(surface->format->BitsPerPixel == 8);
	
	uint8_t *pixels = (uint8_t *)surface->pixels + (y * surface->pitch) + x,
	      *pixels_ll = (uint8_t *)surface->pixels,  // lower limit
	      *pixels_ul = (uint8_t *)surface->pixels + (surface->h * surface->pitch);  // upper limit
	
	for (int y = 0; y < 28; y++)
	{
		// not drawing on screen yet; skip y
		if ((pixels + (12 * 24)) < pixels_ll)
		{
			pixels += surface->pitch;
			continue;
		}
		
		for (int tile = 0; tile < 12; tile++)
		{
			uint8_t *data = *(map + tile);
			
			// no tile; skip tile
			if (data == NULL)
			{
				pixels += 24;
				continue;
			}
			
			data += y * 24;
			
			for (int x = 24; x; x--)
			{
				if (pixels >= pixels_ul)
					return;
				if (pixels >= pixels_ll && *data != 0)
					*pixels = *data;
				
				pixels++;
				data++;
			}
		}
		
		pixels += surface->pitch - 12 * 24;
	}
}

void blit_background_row_blend( SDL_Surface *surface, int x, int y, uint8_t **map )
{
	assert(surface->format->BitsPerPixel == 8);
	
	uint8_t *pixels = (uint8_t *)surface->pixels + (y * surface->pitch) + x,
	      *pixels_ll = (uint8_t *)surface->pixels,  // lower limit
	      *pixels_ul = (uint8_t *)surface->pixels + (surface->h * surface->pitch);  // upper limit
	
	for (int y = 0; y < 28; y++)
	{
		// not drawing on screen yet; skip y
		if ((pixels + (12 * 24)) < pixels_ll)
		{
			pixels += surface->pitch;
			continue;
		}
		
		for (int tile = 0; tile < 12; tile++)
		{
			uint8_t *data = *(map + tile);
			
			// no tile; skip tile
			if (data == NULL)
			{
				pixels += 24;
				continue;
			}
			
			data += y * 24;
			
			for (int x = 24; x; x--)
			{
				if (pixels >= pixels_ul)
					return;
				if (pixels >= pixels_ll && *data != 0)
					*pixels = (*data & 0xf0) | (((*pixels & 0x0f) + (*data & 0x0f)) / 2);
				
				pixels++;
				data++;
			}
		}
		
		pixels += surface->pitch - 12 * 24;
	}
}

void draw_background_1( SDL_Surface *surface )
{
	SDL_FillRect(surface, NULL, 0);
	
	uint8_t **map = (uint8_t **)mapYPos + mapXbpPos - 12;
	
	for (int i = -1; i < 7; i++)
	{
		blit_background_row(surface, mapXPos, (i * 28) + backPos, map);
		
		map += 14;
	}
}

void draw_background_2( SDL_Surface *surface )
{
	if (map2YDelayMax > 1 && backMove2 < 2)
		backMove2 = (map2YDelay == 1) ? 1 : 0;
	
	if (background2 != 0)
	{
		// water effect combines background 1 and 2 by syncronizing the x coordinate
		int x = smoothies[1] ? mapXPos : mapX2Pos;
		
		uint8_t **map = (uint8_t **)mapY2Pos + (smoothies[1] ? mapXbpPos : mapX2bpPos) - 12;
		
		for (int i = -1; i < 7; i++)
		{
			blit_background_row(surface, x, (i * 28) + backPos2, map);
			
			map += 14;
		}
	}
	
	/*Set Movement of background*/
	if (--map2YDelay == 0)
	{
		map2YDelay = map2YDelayMax;
		
		backPos2 += backMove2;
		
		if (backPos2 >  27)
		{
			backPos2 -= 28;
			mapY2--;
			mapY2Pos -= 14;  /*Map Width*/
		}
	}
}

void draw_background_2_blend( SDL_Surface *surface )
{
	if (map2YDelayMax > 1 && backMove2 < 2)
		backMove2 = (map2YDelay == 1) ? 1 : 0;
	
	uint8_t **map = (uint8_t **)mapY2Pos + mapX2bpPos - 12;
	
	for (int i = -1; i < 7; i++)
	{
		blit_background_row_blend(surface, mapX2Pos, (i * 28) + backPos2, map);
		
		map += 14;
	}
	
	/*Set Movement of background*/
	if (--map2YDelay == 0)
	{
		map2YDelay = map2YDelayMax;
		
		backPos2 += backMove2;
		
		if (backPos2 >  27)
		{
			backPos2 -= 28;
			mapY2--;
			mapY2Pos -= 14;  /*Map Width*/
		}
	}
}

void draw_background_3( SDL_Surface *surface )
{
	/* Movement of background */
	backPos3 += backMove3;
	
	if (backPos3 > 27)
	{
		backPos3 -= 28;
		mapY3--;
		mapY3Pos -= 15;   /*Map Width*/
	}
	
	uint8_t **map = (uint8_t **)mapY3Pos + mapX3bpPos - 12;
	
	for (int i = -1; i < 7; i++)
	{
		blit_background_row(surface, mapX3Pos, (i * 28) + backPos3, map);
		
		map += 15;
	}
}

void JE_filterScreen( int8_t col, int8_t int_)
{
	uint8_t *s = NULL; /* screen pointer, 8-bit specific */
	int x, y;
	unsigned int temp;
	
	if (filterFade)
	{
		levelBrightness += levelBrightnessChg;
		if ((filterFadeStart && levelBrightness < -14) || levelBrightness > 14)
		{
			levelBrightnessChg = -levelBrightnessChg;
			filterFadeStart = false;
			levelFilter = levelFilterNew;
		}
		if (!filterFadeStart && levelBrightness == 0)
		{
			filterFade = false;
			levelBrightness = -99;
		}
	}
	
	if (col != -99 && filtrationAvail)
	{
		s = VGAScreen->pixels;
		s += 24;
		
		col <<= 4;
		
		for (y = 184; y; y--)
		{
			for (x = 264; x; x--)
			{
				*s = col | (*s & 0x0f);
				s++;
			}
			s += VGAScreen->pitch - 264;
		}
	}
	
	if (int_ != -99 && explosionTransparent)
	{
		s = VGAScreen->pixels;
		s += 24;
		
		for (y = 184; y; y--)
		{
			for (x = 264; x; x--)
			{
				temp = (*s & 0x0f) + int_;
				*s = (*s & 0xf0) | (temp >= 0x1f ? 0 : (temp >= 0x0f ? 0x0f : temp));
				s++;
			}
			s += VGAScreen->pitch - 264;
		}
	}
}

void JE_checkSmoothies( void )
{
	anySmoothies = (processorType > 2 && (smoothies[1-1] || smoothies[2-1])) || (processorType > 1 && (smoothies[3-1] || smoothies[4-1] || smoothies[5-1]));
}

void lava_filter( SDL_Surface *dst, SDL_Surface *src )
{
	assert(src->format->BitsPerPixel == 8 && dst->format->BitsPerPixel == 8);
	
	/* we don't need to check for over-reading the pixel surfaces since we only
	 * read from the top 185+1 scanlines, and there should be 320 */
	
	const int dst_pitch = dst->pitch;
	uint8_t *dst_pixel = (uint8_t *)dst->pixels + (185 * dst_pitch);
	const uint8_t * const dst_pixel_ll = (uint8_t *)dst->pixels;  // lower limit
	
	const int src_pitch = src->pitch;
	const uint8_t *src_pixel = (uint8_t *)src->pixels + (185 * src->pitch);
	const uint8_t * const src_pixel_ll = (uint8_t *)src->pixels;  // lower limit
	
	int w = 320 * 185 - 1;
	
	for (int y = 185 - 1; y >= 0; --y)
	{
		dst_pixel -= (dst_pitch - 320);  // in case pitch is not 320
		src_pixel -= (src_pitch - 320);  // in case pitch is not 320
		
		for (int x = 320 - 1; x >= 0; x -= 8)
		{
			int waver = abs(((w >> 9) & 0x0f) - 8) - 1;
			w -= 8;
			
			for (int xi = 8 - 1; xi >= 0; --xi)
			{
				--dst_pixel;
				--src_pixel;
				
				// value is average value of source pixel (2x), destination pixel above, and destination pixel below (all with waver)
				// hue is red
				uint8_t value = 0;
				
				if (src_pixel + waver >= src_pixel_ll)
					value += (*(src_pixel + waver) & 0x0f) * 2;
				value += *(dst_pixel + waver + dst_pitch) & 0x0f;
				if (dst_pixel + waver - dst_pitch >= dst_pixel_ll)
					value += *(dst_pixel + waver - dst_pitch) & 0x0f;
				
				*dst_pixel = (value / 4) | 0x70;
			}
		}
	}
}

void water_filter( SDL_Surface *dst, SDL_Surface *src )
{
	assert(src->format->BitsPerPixel == 8 && dst->format->BitsPerPixel == 8);
	
	uint8_t hue = smoothie_data[1] << 4;
	
	/* we don't need to check for over-reading the pixel surfaces since we only
	 * read from the top 185+1 scanlines, and there should be 320 */
	
	const int dst_pitch = dst->pitch;
	uint8_t *dst_pixel = (uint8_t *)dst->pixels + (185 * dst_pitch);
	
	const uint8_t *src_pixel = (uint8_t *)src->pixels + (185 * src->pitch);
	
	int w = 320 * 185 - 1;
	
	for (int y = 185 - 1; y >= 0; --y)
	{
		dst_pixel -= (dst_pitch - 320);  // in case pitch is not 320
		src_pixel -= (src->pitch - 320);  // in case pitch is not 320
		
		for (int x = 320 - 1; x >= 0; x -= 8)
		{
			int waver = abs(((w >> 10) & 0x07) - 4) - 1;
			w -= 8;
			
			for (int xi = 8 - 1; xi >= 0; --xi)
			{
				--dst_pixel;
				--src_pixel;
				
				// pixel is copied from source if not blue
				// otherwise, value is average of value of source pixel and destination pixel below (with waver)
				if ((*src_pixel & 0x30) == 0)
				{
					*dst_pixel = *src_pixel;
				}
				else
				{
					uint8_t value = *src_pixel & 0x0f;
					value += *(dst_pixel + waver + dst_pitch) & 0x0f;
					*dst_pixel = (value / 2) | hue;
				}
			}
		}
	}
}

void iced_blur_filter( SDL_Surface *dst, SDL_Surface *src )
{
	assert(src->format->BitsPerPixel == 8 && dst->format->BitsPerPixel == 8);
	
	uint8_t *dst_pixel = dst->pixels;
	const uint8_t *src_pixel = src->pixels;
	
	for (int y = 0; y < 184; ++y)
	{
		for (int x = 0; x < 320; ++x)
		{
			// value is average value of source pixel and destination pixel
			// hue is icy blue
			
			const uint8_t value = (*src_pixel & 0x0f) + (*dst_pixel & 0x0f);
			*dst_pixel = (value / 2) | 0x80;
			
			++dst_pixel;
			++src_pixel;
		}
		
		dst_pixel += (dst->pitch - 320);  // in case pitch is not 320
		src_pixel += (src->pitch - 320);  // in case pitch is not 320
	}
}

void blur_filter( SDL_Surface *dst, SDL_Surface *src )
{
	assert(src->format->BitsPerPixel == 8 && dst->format->BitsPerPixel == 8);
	
	uint8_t *dst_pixel = dst->pixels;
	const uint8_t *src_pixel = src->pixels;
	
	for (int y = 0; y < 184; ++y)
	{
		for (int x = 0; x < 320; ++x)
		{
			// value is average value of source pixel and destination pixel
			// hue is source pixel hue
			
			const uint8_t value = (*src_pixel & 0x0f) + (*dst_pixel & 0x0f);
			*dst_pixel = (value / 2) | (*src_pixel & 0xf0);
			
			++dst_pixel;
			++src_pixel;
		}
		
		dst_pixel += (dst->pitch - 320);  // in case pitch is not 320
		src_pixel += (src->pitch - 320);  // in case pitch is not 320
	}
}

