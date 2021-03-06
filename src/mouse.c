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
#include "keyboard.h"
#include "nortvars.h"
#include "sprite.h"
#include "video.h"
#include "vga256d.h"

#if defined(TARGET_GP2X) || defined(TARGET_DINGUX)
bool has_mouse = false;
#else
bool has_mouse = true;
#endif
bool mouse_has_three_buttons = true;

uint16_t lastMouseX, lastMouseY;
uint8_t mouseCursor;
uint16_t mouseX, mouseY, mouseButton;
uint16_t mouseXB, mouseYB;

uint8_t mouseGrabShape[24 * 28];                 /* [1..24*28] */

void JE_drawShapeTypeOne( uint16_t x, uint16_t y, uint8_t *shape )
{
	uint16_t xloop = 0, yloop = 0;
	uint8_t *p = shape; /* shape pointer */
	uint8_t *s;   /* screen pointer, 8-bit specific */
	uint8_t *s_limit; /* buffer boundary */

	s = (uint8_t *)VGAScreen->pixels;
	s += y * VGAScreen->pitch + x;

	s_limit = (uint8_t *)VGAScreen->pixels;
	s_limit += VGAScreen->h * VGAScreen->pitch;

	for (yloop = 0; yloop < 28; yloop++)
	{
		for (xloop = 0; xloop < 24; xloop++)
		{
			if (s >= s_limit) return;
			*s = *p;
			s++; p++;
		}
		s -= 24;
		s += VGAScreen->pitch;
	}
}

void JE_grabShapeTypeOne( uint16_t x, uint16_t y, uint8_t *shape )
{
	uint16_t xloop = 0, yloop = 0;
	uint8_t *p = shape; /* shape pointer */
	uint8_t *s;   /* screen pointer, 8-bit specific */
	uint8_t *s_limit; /* buffer boundary */

	s = (uint8_t *)VGAScreen->pixels;
	s += y * VGAScreen->pitch + x;

	s_limit = (uint8_t *)VGAScreen->pixels;
	s_limit += VGAScreen->h * VGAScreen->pitch;

	for (yloop = 0; yloop < 28; yloop++)
	{
		for (xloop = 0; xloop < 24; xloop++)
		{
			if (s >= s_limit) return;
			*p = *s;
			s++; p++;
		}
		s -= 24;
		s += VGAScreen->pitch;
	}
}

void JE_mouseStart( void )
{
	const uint16_t mouseCursorGr[3] /* [1..3] */ = {273, 275, 277};
	
	if (has_mouse)
	{
		service_SDL_events(false);
		mouseButton = mousedown ? lastmouse_but : 0; /* incorrect, possibly unimportant */
		lastMouseX = MIN(mouse_x, 320 - 13);
		lastMouseY = MIN(mouse_y, 200 - 16);
		
		JE_grabShapeTypeOne(lastMouseX, lastMouseY, mouseGrabShape);
		
		blit_sprite2x2(VGAScreen, lastMouseX, lastMouseY, shapes6, mouseCursorGr[mouseCursor]);
	 }
}

void JE_mouseReplace( void )
{
	if (has_mouse)
		JE_drawShapeTypeOne(lastMouseX, lastMouseY, mouseGrabShape);
}

