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
#ifndef TYRIAN2_H
#define TYRIAN2_H

#include "opentyr.h"

#include "varz.h"
#include "helptext.h"

void intro_logos( void );

typedef struct
{
	uint8_t link_num;
	uint8_t armor;
	uint8_t color;
}
boss_bar_t;

extern boss_bar_t boss_bar[2];

extern char tempStr[31];
extern uint8_t itemAvail[9][10], itemAvailMax[9];

void JE_createNewEventEnemy( uint8_t enemytypeofs, uint16_t enemyoffset, int16_t uniqueShapeTableI );

void JE_doNetwork( void );

uint32_t JE_makeEnemy( struct JE_SingleEnemyType *enemy, uint16_t eDatI, int16_t uniqueShapeTableI );

void JE_eventJump( uint16_t jump );

void JE_whoa( void );

void JE_barX ( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t col );

int16_t JE_newEnemy( int enemyOffset, uint16_t eDatI, int16_t uniqueShapeTableI );
void JE_drawEnemy( int enemyOffset );
void JE_starShowVGA( void );

void JE_main( void );
void JE_loadMap( void );
bool JE_titleScreen( bool animate );
void JE_readTextSync( void );
void JE_displayText( void );

bool JE_searchFor( uint8_t PLType );
void JE_eventSystem( void );

void draw_boss_bar( void );

#endif /* TYRIAN2_H */

