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
#ifndef NORTSONG_H
#define NORTSONG_H

#include "opentyr.h"

#include "musmast.h"
#include "sndmast.h"

extern uint32_t target, target2;

extern uint16_t frameCount, frameCount2, frameCountMax;

extern uint8_t *digiFx[SAMPLE_COUNT];
extern uint16_t fxSize[SAMPLE_COUNT];

extern uint16_t tyrMusicVolume, fxVolume;
extern uint16_t fxPlayVol;
extern uint16_t tempVolume;

extern uint16_t speed;

extern float jasondelay;

void setdelay( uint8_t delay );
void setjasondelay( int delay );
void setjasondelay2( int delay );
int delaycount( void );
int delaycount2( void );

void wait_delay( void );
void service_wait_delay( void );
void wait_delayorinput( bool keyboard, bool mouse, bool joystick );

void JE_resetTimerInt( void );
void JE_setTimerInt( void );

void JE_calcFXVol( void );
void JE_changeVolume( uint16_t *music, int music_delta, uint16_t *sample, int sample_delta );

void JE_loadSndFile( const char *effects_sndfile, const char *voices_sndfile );
void JE_playSampleNum( uint8_t samplenum );

#endif /* NORTSONG_H */

