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
#ifndef MAININT_H
#define MAININT_H

#include "config.h"
#include "opentyr.h"
#include "palette.h"
#include "player.h"
#include "sprite.h"

extern bool button[4]; // fire, left fire, right fire, mode swap

extern int8_t constantLastX;
extern uint16_t textErase;
extern uint16_t upgradeCost;
extern uint16_t downgradeCost;
extern bool performSave;
extern bool jumpSection;
extern bool useLastBank;

extern bool pause_pressed, ingamemenu_pressed;

/*void JE_textMenuWait ( uint16_t waittime, bool dogamma );*/

void JE_drawTextWindow( const char *text );
void JE_initPlayerData( void );
void JE_highScoreScreen( void );
void JE_gammaCorrect_func( uint8_t *col, float r );
void JE_gammaCorrect( Palette *colorBuffer, uint8_t gamma );
bool JE_gammaCheck( void );
/* void JE_textMenuWait( uint16_t *waitTime, bool doGamma ); /!\ In setup.h */
void JE_loadOrderingInfo( void );
void JE_nextEpisode( void );
void JE_helpSystem( uint8_t startTopic );
void JE_doInGameSetup( void );
bool JE_inGameSetup( void );
void JE_inGameHelp( void );
void JE_sortHighScores( void );
void JE_highScoreCheck( void );
void adjust_difficulty( void );

bool load_next_demo( void );
bool replay_demo_keys( void );
bool read_demo_keys( void );

void JE_SFCodes( uint8_t playerNum_, int16_t PX_, int16_t PY_, int16_t mouseX_, int16_t mouseY_ );
void JE_sort( void );

long weapon_upgrade_cost( long base_cost, unsigned int power );
uint32_t JE_getCost( uint8_t itemType, uint16_t itemNum );
int32_t JE_getValue( uint8_t itemType, uint16_t itemNum );
uint32_t JE_totalScore( const Player * );

void JE_drawPortConfigButtons( void );
void JE_outCharGlow( uint16_t x, uint16_t y, const char *s );

void JE_playCredits( void );
void JE_endLevelAni( void );
void JE_drawCube( SDL_Surface * screen, uint16_t x, uint16_t y, uint8_t filter, uint8_t brightness );
void JE_handleChat( void );
bool str_pop_int( char *str, int *val );
void JE_loadScreen( void );
void JE_operation( uint8_t slot );
void JE_inGameDisplays( void );
void JE_mainKeyboardInput( void );
void JE_pauseGame( void );

void JE_playerMovement( Player *this_player, uint8_t inputDevice, uint8_t playerNum, uint16_t shipGr, Sprite2_array *shapes9ptr_, uint16_t *mouseX, uint16_t *mouseY );
void JE_mainGamePlayerFunctions( void );
const char *JE_getName( uint8_t pnum );

void JE_playerCollide( Player *this_player, uint8_t playerNum );


#endif /* MAININT_H */

