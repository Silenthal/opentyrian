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
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "opentyr.h"

#define SDL_POLL_INTERVAL 5

extern bool ESCPressed;
extern bool newkey, newmouse, keydown, mousedown;
extern SDLKey lastkey_sym;
extern SDLMod lastkey_mod;
extern unsigned char lastkey_char;
extern uint8_t lastmouse_but;
extern uint16_t lastmouse_x, lastmouse_y;
extern bool mouse_pressed[3];
extern uint16_t mouse_x, mouse_y;
extern int numkeys;
extern uint8_t *keysactive;

extern bool input_grab_enabled, input_grabbed;

void flush_events_buffer( void );
void wait_input( bool keyboard, bool mouse, bool joystick );
void wait_noinput( bool keyboard, bool mouse, bool joystick );
void init_keyboard( void );
void input_grab( void );
uint16_t JE_mousePosition( uint16_t *mouseX, uint16_t *mouseY );
void set_mouse_position( int x, int y );

void service_SDL_events( bool clear_new );

void sleep_game( void );

void JE_clearKeyboard( void );

#endif /* KEYBOARD_H */

