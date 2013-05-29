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
#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "helptext.h"
#include "opentyr.h"

typedef uint8_t JE_MenuChoiceType[MENU_MAX];

int32_t JE_cashLeft( void );
void JE_itemScreen( void );

void load_cubes( void );
bool load_cube( int cube_slot, int cube_index );

void JE_drawItem( uint8_t itemType, uint16_t itemNum, uint16_t x, uint16_t y );
void JE_drawMenuHeader( void );
void JE_drawMenuChoices( void );
void JE_updateNavScreen( void );
void JE_drawNavLines( bool dark );
void JE_drawLines( SDL_Surface *surface, bool dark );
void JE_drawDots( void );
void JE_drawPlanet( uint8_t planetNum );
void draw_ship_illustration( void );
void JE_scaleBitmap( SDL_Surface *dst, const SDL_Surface *src, int x1, int y1, int x2, int y2 );
void JE_initWeaponView( void );
void JE_computeDots( void );
int16_t JE_partWay( int16_t start, int16_t finish, uint8_t dots, uint8_t dist );
void JE_doShipSpecs( void );
void JE_drawMainMenuHelpText( void );
bool JE_quitRequest( void );
void JE_genItemMenu( uint8_t itemnum );
void JE_scaleInPicture( SDL_Surface *dst, const SDL_Surface *src );
void JE_drawScore( void );
void JE_menuFunction( uint8_t select );
void JE_drawShipSpecs( SDL_Surface *, SDL_Surface * );
void JE_weaponSimUpdate( void );
void JE_weaponViewFrame( void );

#endif // GAME_MENU_H

