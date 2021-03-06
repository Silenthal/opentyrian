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
#ifndef FONTHAND_H
#define FONTHAND_H

#include "opentyr.h"

#define PART_SHADE 0
#define FULL_SHADE 1
#define DARKEN     2
#define TRICK      3
#define NO_SHADE 255

extern const int font_ascii[256];

extern int16_t defaultBrightness;
extern uint8_t textGlowFont, textGlowBrightness;
extern bool levelWarningDisplay;
extern uint8_t levelWarningLines;
extern char levelWarningText[10][61];
extern bool warningRed;
extern uint8_t warningSoundDelay;
extern uint16_t armorShipDelay;
extern uint8_t warningCol;
extern int8_t warningColChange;

void JE_dString( SDL_Surface * screen, int x, int y, const char *s, unsigned int font );

int JE_fontCenter( const char *s, unsigned int font );
int JE_textWidth( const char *s, unsigned int font );
void JE_textShade( SDL_Surface * screen, int x, int y, const char *s, unsigned int colorbank, int brightness, unsigned int shadetype );
void JE_outText( SDL_Surface * screen, int x, int y, const char *s, unsigned int colorbank, int brightness );
void JE_outTextModify( SDL_Surface * screen, int x, int y, const char *s, unsigned int filter, unsigned int brightness, unsigned int font );
void JE_outTextAdjust( SDL_Surface * screen, int x, int y, const char *s, unsigned int filter, int brightness, unsigned int font, bool shadow );
void JE_outTextAndDarken( SDL_Surface * screen, int x, int y, const char *s, unsigned int colorbank, unsigned int brightness, unsigned int font );

void JE_updateWarning( SDL_Surface * screen );
void JE_outTextGlow( SDL_Surface * screen, int x, int y, const char *s );

#endif /* FONTHAND_H */

