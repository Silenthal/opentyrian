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
#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"
#include "opentyr.h"

enum
{
	FRONT_WEAPON = 0,
	REAR_WEAPON = 1
};

enum
{
	LEFT_SIDEKICK = 0,
	RIGHT_SIDEKICK = 1
};

typedef struct
{
	uint32_t ship;
	uint32_t generator;
	uint32_t shield;
	struct { uint32_t id; uint32_t power; } weapon[2];
	uint32_t sidekick[2];
	uint32_t special;
	
	// Dragonwing only:
	// repeatedly collecting the same powerup gives a series of sidekick upgrades
	uint32_t sidekick_series;
	uint32_t sidekick_level;
	
	// Single-player only
	uint32_t super_arcade_mode;  // stored as an item for compatibility :(
}
PlayerItems;

typedef struct
{
	uint32_t cash;
	
	PlayerItems items, last_items;
	
	bool is_dragonwing;  // i.e., is player 2
	uint32_t *lives;
	
	// calculatable
	uint32_t shield_max;
	uint32_t initial_armor;
	uint32_t shot_hit_area_x, shot_hit_area_y;
	
	// state
	bool is_alive;
	uint32_t invulnerable_ticks;  // ticks until ship can be damaged
	uint32_t exploding_ticks;     // ticks until ship done exploding
	uint32_t shield;
	uint32_t armor;
	uint32_t weapon_mode;
	uint32_t superbombs;
	uint32_t purple_balls_needed;
	
	int x, y;
	int old_x[20], old_y[20];
	
	int x_velocity, y_velocity;
	uint32_t x_friction_ticks, y_friction_ticks;  // ticks until friction is applied
	
	int delta_x_shot_move, delta_y_shot_move;
	
	int last_x_shot_move, last_y_shot_move;
	int last_x_explosion_follow, last_y_explosion_follow;
	
	struct
	{
		// calculatable
		int ammo_max;
		uint32_t ammo_refill_ticks_max;
		uint32_t style;  // affects movement and size
		
		// state
		int x, y;
		int ammo;
		uint32_t ammo_refill_ticks;
		
		bool animation_enabled;
		uint32_t animation_frame;
		
		uint32_t charge;
		uint32_t charge_ticks;
	}
	sidekick[2];
}
Player;

extern Player player[2];

static inline bool all_players_dead( void )
{
	return (!player[0].is_alive && (!twoPlayerMode || !player[1].is_alive));
}

static inline bool all_players_alive( void )
{
	return (player[0].is_alive && (!twoPlayerMode || player[1].is_alive));
}

void calc_purple_balls_needed( Player * );
bool power_up_weapon( Player *, uint32_t port );
void handle_got_purple_ball( Player * );

#endif // PLAYER_H
