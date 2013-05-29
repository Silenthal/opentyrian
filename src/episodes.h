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
#ifndef EPISODES_H
#define EPISODES_H

#include "opentyr.h"

#include "lvlmast.h"


/* Episodes and general data */

#define FIRST_LEVEL 1
#define EPISODE_MAX 5
#ifdef TYRIAN2000
#define EPISODE_AVAILABLE 5
#else
#define EPISODE_AVAILABLE 4
#endif

typedef struct
{
	uint16_t     drain;
	uint8_t     shotrepeat;
	uint8_t     multi;
	uint16_t     weapani;
	uint8_t     max;
	uint8_t     tx, ty, aim;
	uint8_t     attack[8], del[8]; /* [1..8] */
	int8_t sx[8], sy[8]; /* [1..8] */
	int8_t bx[8], by[8]; /* [1..8] */
	uint16_t     sg[8]; /* [1..8] */
	int8_t acceleration, accelerationx;
	uint8_t     circlesize;
	uint8_t     sound;
	uint8_t     trail;
	uint8_t     shipblastfilter;
} JE_WeaponType[WEAP_NUM + 1]; /* [0..weapnum] */

typedef struct
{
	char    name[31]; /* string [30] */
	uint8_t opnum;
	uint16_t op[2][11]; /* [1..2, 1..11] */
	uint16_t cost;
	uint16_t itemgraphic;
	uint16_t poweruse;
} JE_WeaponPortType[PORT_NUM + 1]; /* [0..portnum] */

typedef struct
{
	char        name[31]; /* string [30] */
	uint16_t     itemgraphic;
	uint8_t     power;
	int8_t speed;
	uint16_t     cost;
} JE_PowerType[POWER_NUM + 1]; /* [0..powernum] */

typedef struct
{
	char    name[31]; /* string [30] */
	uint16_t itemgraphic;
	uint8_t pwr;
	uint8_t stype;
	uint16_t wpn;
} JE_SpecialType[SPECIAL_NUM + 1]; /* [0..specialnum] */

typedef struct
{
	char        name[31]; /* string [30] */
	uint8_t     pwr;
	uint16_t     itemgraphic;
	uint16_t     cost;
	uint8_t     tr, option;
	int8_t opspd;
	uint8_t     ani;
	uint16_t     gr[20]; /* [1..20] */
	uint8_t     wport;
	uint16_t     wpnum;
	uint8_t     ammo;
	bool  stop;
	uint8_t     icongr;
} JE_OptionType;

typedef struct
{
	char    name[31]; /* string [30] */
	uint8_t tpwr;
	uint8_t mpwr;
	uint16_t itemgraphic;
	uint16_t cost;
} JE_ShieldType[SHIELD_NUM + 1]; /* [0..shieldnum] */

typedef struct
{
	char        name[31]; /* string [30] */
	uint16_t     shipgraphic;
	uint16_t     itemgraphic;
	uint8_t     ani;
	int8_t spd;
	uint8_t     dmg;
	uint16_t     cost;
	uint8_t     bigshipgraphic;
} JE_ShipType[SHIP_NUM + 1]; /* [0..shipnum] */

/* EnemyData */
typedef struct
{
	uint8_t     ani;
	uint8_t     tur[3]; /* [1..3] */
	uint8_t     freq[3]; /* [1..3] */
	int8_t xmove;
	int8_t ymove;
	int8_t xaccel;
	int8_t yaccel;
	int8_t xcaccel;
	int8_t ycaccel;
	int16_t  startx;
	int16_t  starty;
	int8_t startxc;
	int8_t startyc;
	uint8_t     armor;
	uint8_t     esize;
	uint16_t     egraphic[20];  /* [1..20] */
	uint8_t     explosiontype;
	uint8_t     animate;       /* 0:Not Yet   1:Always   2:When Firing Only */
	uint8_t     shapebank;     /* See LEVELMAK.DOC */
	int8_t xrev, yrev;
	uint16_t     dgr;
	int8_t dlevel;
	int8_t dani;
	uint8_t     elaunchfreq;
	uint16_t     elaunchtype;
	int16_t  value;
	uint16_t     eenemydie;
} JE_EnemyDatType[ENEMY_NUM + 1]; /* [0..enemynum] */

extern JE_WeaponPortType weaponPort;
extern JE_WeaponType weapons;
extern JE_PowerType powerSys;
extern JE_ShipType ships;
extern JE_OptionType options[OPTION_NUM + 1]; /* [0..optionnum] */
extern JE_ShieldType shields;
extern JE_SpecialType special;
extern JE_EnemyDatType enemyDat;
extern uint8_t initial_episode_num, episodeNum;
extern bool episodeAvail[EPISODE_MAX];

extern char episode_file[13], cube_file[13];

extern int32_t episode1DataLoc;
extern bool bonusLevel;
extern bool jumpBackToEpisode1;

void JE_loadItemDat( void );
void JE_initEpisode( uint8_t newEpisode );
unsigned int JE_findNextEpisode( void );
void JE_scanForEpisodes( void );

#endif /* EPISODES_H */

