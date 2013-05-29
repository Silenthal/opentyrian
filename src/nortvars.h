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
#ifndef NORTVARS_H
#define NORTVARS_H

#include "opentyr.h"

extern bool inputDetected;

bool JE_buttonPressed( void );

bool JE_anyButton( void );

void JE_dBar3( SDL_Surface *surface, int16_t x,  int16_t y,  int16_t num,  int16_t col );
void JE_barDrawShadow( SDL_Surface *surface, uint16_t x, uint16_t y, uint16_t res, uint16_t col, uint16_t amt, uint16_t xsize, uint16_t ysize );
void JE_wipeKey( void );

#endif /* NORTVARS_H */

