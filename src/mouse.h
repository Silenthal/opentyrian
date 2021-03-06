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
#ifndef MOUSE_H
#define MOUSE_H

#include "opentyr.h"
 
extern bool has_mouse;
extern bool mouse_has_three_buttons;

extern uint16_t lastMouseX, lastMouseY;
extern uint8_t mouseCursor;
extern uint16_t mouseX, mouseY, mouseButton;
extern uint16_t mouseXB, mouseYB;

extern uint8_t mouseGrabShape[24 * 28];

void JE_drawShapeTypeOne( uint16_t x, uint16_t y, uint8_t *shape );
void JE_grabShapeTypeOne( uint16_t x, uint16_t y, uint8_t *shape );

void JE_mouseStart( void );
void JE_mouseReplace( void );

#endif /* MOUSE_H */

