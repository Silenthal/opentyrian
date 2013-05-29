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
#ifndef LOUDNESS_H
#define LOUDNESS_H

#include "opentyr.h"
#include "opl.h"

#define SFX_CHANNELS 8

#if defined(TARGET_GP2X) || defined(TARGET_DINGUX)
#define OUTPUT_QUALITY 2  // 22 kHz
#else
#define OUTPUT_QUALITY 4  // 44 kHz
#endif

#define SAMPLE_SCALING OUTPUT_QUALITY
#define SAMPLE_TYPE Bit16s
#define BYTES_PER_SAMPLE 2

extern float music_volume, sample_volume;

extern uint32_t song_playing;

extern bool audio_disabled, music_disabled, samples_disabled;

bool init_audio( void );
void deinit_audio( void );

void load_music( void );
void play_song( uint32_t song_num );
void restart_song( void );
void stop_song( void );
void fade_song( void );

void set_volume( uint32_t music, uint32_t sample );

void JE_multiSamplePlay(uint8_t *buffer, uint16_t size, uint8_t chan, uint8_t vol);

#endif /* LOUDNESS_H */

