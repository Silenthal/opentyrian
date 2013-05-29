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
#include "config.h"
#include "episodes.h"
#include "file.h"
#include "lvllib.h"
#include "lvlmast.h"
#include "opentyr.h"


/* MAIN Weapons Data */
JE_WeaponPortType weaponPort;
JE_WeaponType     weapons;

/* Items */
JE_PowerType   powerSys;
JE_ShipType    ships;
JE_OptionType  options[OPTION_NUM + 1]; /* [0..optionnum] */
JE_ShieldType  shields;
JE_SpecialType special;

/* Enemy data */
JE_EnemyDatType enemyDat;

/* EPISODE variables */
uint8_t    initial_episode_num, episodeNum = 0;
bool episodeAvail[EPISODE_MAX]; /* [1..episodemax] */
char       episode_file[13], cube_file[13];

int32_t episode1DataLoc;

/* Tells the game whether the level currently loaded is a bonus level. */
bool bonusLevel;

/* Tells if the game jumped back to Episode 1 */
bool jumpBackToEpisode1;

void JE_loadItemDat( void )
{
	FILE *f = NULL;
	
	if (episodeNum <= 3)
	{
		f = dir_fopen_die(data_dir(), "tyrian.hdt", "rb");
		efread(&episode1DataLoc, sizeof(int32_t), 1, f);
		fseek(f, episode1DataLoc, SEEK_SET);
	}
	else
	{
		// episode 4 stores item data in the level file
		f = dir_fopen_die(data_dir(), levelFile, "rb");
		fseek(f, lvlPos[lvlNum-1], SEEK_SET);
	}

	uint16_t itemNum[7]; /* [1..7] */
	efread(&itemNum, sizeof(uint16_t), 7, f);

	for (int i = 0; i < WEAP_NUM + 1; ++i)
	{
		efread(&weapons[i].drain,           sizeof(uint16_t), 1, f);
		efread(&weapons[i].shotrepeat,      sizeof(uint8_t), 1, f);
		efread(&weapons[i].multi,           sizeof(uint8_t), 1, f);
		efread(&weapons[i].weapani,         sizeof(uint16_t), 1, f);
		efread(&weapons[i].max,             sizeof(uint8_t), 1, f);
		efread(&weapons[i].tx,              sizeof(uint8_t), 1, f);
		efread(&weapons[i].ty,              sizeof(uint8_t), 1, f);
		efread(&weapons[i].aim,             sizeof(uint8_t), 1, f);
		efread(&weapons[i].attack,          sizeof(uint8_t), 8, f);
		efread(&weapons[i].del,             sizeof(uint8_t), 8, f);
		efread(&weapons[i].sx,              sizeof(int8_t), 8, f);
		efread(&weapons[i].sy,              sizeof(int8_t), 8, f);
		efread(&weapons[i].bx,              sizeof(int8_t), 8, f);
		efread(&weapons[i].by,              sizeof(int8_t), 8, f);
		efread(&weapons[i].sg,              sizeof(uint16_t), 8, f);
		efread(&weapons[i].acceleration,    sizeof(int8_t), 1, f);
		efread(&weapons[i].accelerationx,   sizeof(int8_t), 1, f);
		efread(&weapons[i].circlesize,      sizeof(uint8_t), 1, f);
		efread(&weapons[i].sound,           sizeof(uint8_t), 1, f);
		efread(&weapons[i].trail,           sizeof(uint8_t), 1, f);
		efread(&weapons[i].shipblastfilter, sizeof(uint8_t), 1, f);
	}

#ifdef TYRIAN2000
	if (episodeNum <= 3) fseek(f, 0x252A4, SEEK_SET);
	if (episodeNum == 4) fseek(f, 0xC1F5E, SEEK_SET);
	if (episodeNum == 5) fseek(f, 0x5C5B8, SEEK_SET);
#endif
	
	for (int i = 0; i < PORT_NUM + 1; ++i)
	{
		fseek(f, 1, SEEK_CUR); /* skip string length */
		efread(&weaponPort[i].name,        1, 30, f);
		weaponPort[i].name[30] = '\0';
		efread(&weaponPort[i].opnum,       sizeof(uint8_t), 1, f);
		for (int j = 0; j < 2; ++j)
		{
			efread(&weaponPort[i].op[j],   sizeof(uint16_t), 11, f);
		}
		efread(&weaponPort[i].cost,        sizeof(uint16_t), 1, f);
		efread(&weaponPort[i].itemgraphic, sizeof(uint16_t), 1, f);
		efread(&weaponPort[i].poweruse,    sizeof(uint16_t), 1, f);
	}

	int specials_count = SPECIAL_NUM;
#ifdef TYRIAN2000
	if (episodeNum <= 3) fseek(f, 0x2662E, SEEK_SET);
	if (episodeNum == 4) fseek(f, 0xC32E8, SEEK_SET);
	if (episodeNum == 5) fseek(f, 0x5D942, SEEK_SET);
	if (episodeNum >= 4) specials_count = SPECIAL_NUM + 8; /*this ugly hack will need a fix*/
#endif
	
	for (int i = 0; i < specials_count + 1; ++i)
	{
		fseek(f, 1, SEEK_CUR); /* skip string length */
		efread(&special[i].name,        1, 30, f);
		special[i].name[30] = '\0';
		efread(&special[i].itemgraphic, sizeof(uint16_t), 1, f);
		efread(&special[i].pwr,         sizeof(uint8_t), 1, f);
		efread(&special[i].stype,       sizeof(uint8_t), 1, f);
		efread(&special[i].wpn,         sizeof(uint16_t), 1, f);
	}

#ifdef TYRIAN2000
	if (episodeNum <= 3) fseek(f, 0x26E21, SEEK_SET);
	if (episodeNum == 4) fseek(f, 0xC3ADB, SEEK_SET);
	if (episodeNum == 5) fseek(f, 0x5E135, SEEK_SET);
#endif
		
	for (int i = 0; i < POWER_NUM + 1; ++i)
	{
		fseek(f, 1, SEEK_CUR); /* skip string length */
		efread(&powerSys[i].name,        1, 30, f);
		powerSys[i].name[30] = '\0';
		efread(&powerSys[i].itemgraphic, sizeof(uint16_t), 1, f);
		efread(&powerSys[i].power,       sizeof(int8_t), 1, f);
		efread(&powerSys[i].speed,       sizeof(uint8_t), 1, f);
		efread(&powerSys[i].cost,        sizeof(uint16_t), 1, f);
	}

#ifdef TYRIAN2000
	if (episodeNum <= 3) fseek(f, 0x26F24, SEEK_SET);
	if (episodeNum == 4) fseek(f, 0xC3BDE, SEEK_SET);
	if (episodeNum == 5) fseek(f, 0x5E238, SEEK_SET);
#endif
	
	for (int i = 0; i < SHIP_NUM + 1; ++i)
	{
		fseek(f, 1, SEEK_CUR); /* skip string length */
		efread(&ships[i].name,           1, 30, f);
		ships[i].name[30] = '\0';
		efread(&ships[i].shipgraphic,    sizeof(uint16_t), 1, f);
		efread(&ships[i].itemgraphic,    sizeof(uint16_t), 1, f);
		efread(&ships[i].ani,            sizeof(uint8_t), 1, f);
		efread(&ships[i].spd,            sizeof(int8_t), 1, f);
		efread(&ships[i].dmg,            sizeof(uint8_t), 1, f);
		efread(&ships[i].cost,           sizeof(uint16_t), 1, f);
		efread(&ships[i].bigshipgraphic, sizeof(uint8_t), 1, f);
	}

#ifdef TYRIAN2000
	if (episodeNum <= 3) fseek(f, 0x2722F, SEEK_SET);
	if (episodeNum == 4) fseek(f, 0xC3EE9, SEEK_SET);
	if (episodeNum == 5) fseek(f, 0x5E543, SEEK_SET); 
#endif
	
	for (int i = 0; i < OPTION_NUM + 1; ++i)
	{
		fseek(f, 1, SEEK_CUR); /* skip string length */
		efread(&options[i].name,        1, 30, f);
		options[i].name[30] = '\0';
		efread(&options[i].pwr,         sizeof(uint8_t), 1, f);
		efread(&options[i].itemgraphic, sizeof(uint16_t), 1, f);
		efread(&options[i].cost,        sizeof(uint16_t), 1, f);
		efread(&options[i].tr,          sizeof(uint8_t), 1, f);
		efread(&options[i].option,      sizeof(uint8_t), 1, f);
		efread(&options[i].opspd,       sizeof(int8_t), 1, f);
		efread(&options[i].ani,         sizeof(uint8_t), 1, f);
		efread(&options[i].gr,          sizeof(uint16_t), 20, f);
		efread(&options[i].wport,       sizeof(uint8_t), 1, f);
		efread(&options[i].wpnum,       sizeof(uint16_t), 1, f);
		efread(&options[i].ammo,        sizeof(uint8_t), 1, f);
		efread(&options[i].stop,        1, 1, f); /* override sizeof(bool) */
		efread(&options[i].icongr,      sizeof(uint8_t), 1, f);
	}

#ifdef TYRIAN2000
	if (episodeNum <= 3) fseek(f, 0x27EF3, SEEK_SET);
	if (episodeNum == 4) fseek(f, 0xC4BAD, SEEK_SET);
	if (episodeNum == 5) fseek(f, 0x5F207, SEEK_SET);
#endif
		
	for (int i = 0; i < SHIELD_NUM + 1; ++i)
	{
		fseek(f, 1, SEEK_CUR); /* skip string length */
		efread(&shields[i].name,        1, 30, f);
		shields[i].name[30] = '\0';
		efread(&shields[i].tpwr,        sizeof(uint8_t), 1, f);
		efread(&shields[i].mpwr,        sizeof(uint8_t), 1, f);
		efread(&shields[i].itemgraphic, sizeof(uint16_t), 1, f);
		efread(&shields[i].cost,        sizeof(uint16_t), 1, f);
	}
	
	for (int i = 0; i < ENEMY_NUM + 1; ++i)
	{
		efread(&enemyDat[i].ani,           sizeof(uint8_t), 1, f);
		efread(&enemyDat[i].tur,           sizeof(uint8_t), 3, f);
		efread(&enemyDat[i].freq,          sizeof(uint8_t), 3, f);
		efread(&enemyDat[i].xmove,         sizeof(int8_t), 1, f);
		efread(&enemyDat[i].ymove,         sizeof(int8_t), 1, f);
		efread(&enemyDat[i].xaccel,        sizeof(int8_t), 1, f);
		efread(&enemyDat[i].yaccel,        sizeof(int8_t), 1, f);
		efread(&enemyDat[i].xcaccel,       sizeof(int8_t), 1, f);
		efread(&enemyDat[i].ycaccel,       sizeof(int8_t), 1, f);
		efread(&enemyDat[i].startx,        sizeof(int16_t), 1, f);
		efread(&enemyDat[i].starty,        sizeof(int16_t), 1, f);
		efread(&enemyDat[i].startxc,       sizeof(int8_t), 1, f);
		efread(&enemyDat[i].startyc,       sizeof(int8_t), 1, f);
		efread(&enemyDat[i].armor,         sizeof(uint8_t), 1, f);
		efread(&enemyDat[i].esize,         sizeof(uint8_t), 1, f);
		efread(&enemyDat[i].egraphic,      sizeof(uint16_t), 20, f);
		efread(&enemyDat[i].explosiontype, sizeof(uint8_t), 1, f);
		efread(&enemyDat[i].animate,       sizeof(uint8_t), 1, f);
		efread(&enemyDat[i].shapebank,     sizeof(uint8_t), 1, f);
		efread(&enemyDat[i].xrev,          sizeof(int8_t), 1, f);
		efread(&enemyDat[i].yrev,          sizeof(int8_t), 1, f);
		efread(&enemyDat[i].dgr,           sizeof(uint16_t), 1, f);
		efread(&enemyDat[i].dlevel,        sizeof(int8_t), 1, f);
		efread(&enemyDat[i].dani,          sizeof(int8_t), 1, f);
		efread(&enemyDat[i].elaunchfreq,   sizeof(uint8_t), 1, f);
		efread(&enemyDat[i].elaunchtype,   sizeof(uint16_t), 1, f);
		efread(&enemyDat[i].value,         sizeof(int16_t), 1, f);
		efread(&enemyDat[i].eenemydie,     sizeof(uint16_t), 1, f);
	}
	
	fclose(f);
}

void JE_initEpisode( uint8_t newEpisode )
{
	if (newEpisode == episodeNum)
		return;
	
	episodeNum = newEpisode;
	
	sprintf(levelFile,    "tyrian%d.lvl",  episodeNum);
	sprintf(cube_file,    "cubetxt%d.dat", episodeNum);
	sprintf(episode_file, "levels%d.dat",  episodeNum);
	
	JE_analyzeLevel();
	JE_loadItemDat();
}

void JE_scanForEpisodes( void )
{
	for (int i = 0; i < EPISODE_MAX; ++i)
	{
		char ep_file[20];
		snprintf(ep_file, sizeof(ep_file), "tyrian%d.lvl", i + 1);
		episodeAvail[i] = dir_file_exists(data_dir(), ep_file);
	}
}

unsigned int JE_findNextEpisode( void )
{
	unsigned int newEpisode = episodeNum;
	
	jumpBackToEpisode1 = false;
	
	while (true)
	{
		newEpisode++;
		
		if (newEpisode > EPISODE_MAX)
		{
			newEpisode = 1;
			jumpBackToEpisode1 = true;
			gameHasRepeated = true;
		}
		
		if (episodeAvail[newEpisode-1] || newEpisode == episodeNum)
		{
			break;
		}
	}
	
	return newEpisode;
}

