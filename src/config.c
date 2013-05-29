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
#include "joystick.h"
#include "loudness.h"
#include "mtrand.h"
#include "nortsong.h"
#include "opentyr.h"
#include "player.h"
#include "varz.h"
#include "vga256d.h"
#include "video.h"
#include "video_scale.h"

#include <unistd.h>
#include <sys/stat.h>


/* Configuration Load/Save handler */

const uint8_t cryptKey[10] = /* [1..10] */
{
	15, 50, 89, 240, 147, 34, 86, 9, 32, 208
};

const JE_KeySettingType defaultKeySettings =
{
	SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_SPACE, SDLK_RETURN, SDLK_LCTRL, SDLK_LALT
/*	72, 80, 75, 77, 57, 28, 29, 56*/
};

const char defaultHighScoreNames[34][23] = /* [1..34] of string [22] */
{/*1P*/
/*TYR*/   "The Prime Chair", /*13*/
          "Transon Lohk",
          "Javi Onukala",
          "Mantori",
          "Nortaneous",
          "Dougan",
          "Reid",
          "General Zinglon",
          "Late Gyges Phildren",
          "Vykromod",
          "Beppo",
          "Borogar",
          "ShipMaster Carlos",

/*OTHER*/ "Jill", /*5*/
          "Darcy",
          "Jake Stone",
          "Malvineous Havershim",
          "Marta Louise Velasquez",

/*JAZZ*/  "Jazz Jackrabbit", /*3*/
          "Eva Earlong",
          "Devan Shell",

/*OMF*/   "Crystal Devroe", /*11*/
          "Steffan Tommas",
          "Milano Angston",
          "Christian",
          "Shirro",
          "Jean-Paul",
          "Ibrahim Hothe",
          "Angel",
          "Cossette Akira",
          "Raven",
          "Hans Kreissack",

/*DARE*/  "Tyler", /*2*/
          "Rennis the Rat Guard"
};

const char defaultTeamNames[22][25] = /* [1..22] of string [24] */
{
	"Jackrabbits",
	"Team Tyrian",
	"The Elam Brothers",
	"Dare to Dream Team",
	"Pinball Freaks",
	"Extreme Pinball Freaks",
	"Team Vykromod",
	"Epic All-Stars",
	"Hans Keissack's WARriors",
	"Team Overkill",
	"Pied Pipers",
	"Gencore Growlers",
	"Microsol Masters",
	"Beta Warriors",
	"Team Loco",
	"The Shellians",
	"Jungle Jills",
	"Murderous Malvineous",
	"The Traffic Department",
	"Clan Mikal",
	"Clan Patrok",
	"Carlos' Crawlers"
};


const JE_EditorItemAvailType initialItemAvail =
{
	1,1,1,0,0,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0, /* Front/Rear Weapons 1-38  */
	0,0,0,0,0,0,0,0,0,0,1,                                                           /* Fill                     */
	1,0,0,0,0,1,0,0,0,1,1,0,1,0,0,0,0,0,                                             /* Sidekicks          51-68 */
	0,0,0,0,0,0,0,0,0,0,0,                                                           /* Fill                     */
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                                   /* Special Weapons    81-93 */
	0,0,0,0,0                                                                        /* Fill                     */
};

/* Last 2 bytes = Word
 *
 * Max Value = 1680
 * X div  60 = Armor  (1-28)
 * X div 168 = Shield (1-12)
 * X div 280 = Engine (1-06)
 */


bool smoothies[9] = /* [1..9] */
{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };


uint8_t starShowVGASpecialCode;

/* Stars */
StarDatType starDat[MAX_STARS]; /* [1..Maxstars] */
uint16_t starY;


/* CubeData */
uint16_t lastCubeMax, cubeMax;
uint16_t cubeList[4]; /* [1..4] */

/* High-Score Stuff */
bool gameHasRepeated;  // can only get highscore on first play-through

/* Difficulty */
int8_t difficultyLevel, oldDifficultyLevel,
            initialDifficulty;  // can only get highscore on initial episode

/* Player Stuff */
uint32_t    power, lastPower, powerAdd;
uint8_t shieldWait, shieldT;

uint8_t          shotRepeat[11], shotMultiPos[11];
bool       portConfigChange, portConfigDone;

/* Level Data */
char    lastLevelName[11], levelName[11]; /* string [10] */
uint8_t mainLevel, nextLevel, saveLevel;   /*Current Level #*/

/* Keyboard Junk */
JE_KeySettingType keySettings;

/* Configuration */
int8_t levelFilter, levelFilterNew, levelBrightness, levelBrightnessChg;
bool  filtrationAvail, filterActive, filterFade, filterFadeStart;

bool gameJustLoaded;

bool galagaMode;

bool extraGame;

bool twoPlayerMode, twoPlayerLinked, onePlayerAction, superTyrian;
bool trentWin = false;
uint8_t    superArcadeMode;

uint8_t    superArcadePowerUp;

float linkGunDirec;
uint8_t inputDevice[2] = { 1, 2 }; // 0:any  1:keyboard  2:mouse  3+:joystick

uint8_t secretHint;
uint8_t background3over;
uint8_t background2over;
uint8_t gammaCorrection;
bool superPause = false;
bool explosionTransparent,
           youAreCheating,
           displayScore,
           background2, smoothScroll, wild, superWild, starActive,
           topEnemyOver,
           skyEnemyOverAll,
           background2notTransparent;

uint8_t soundEffects; // dummy value for config
uint8_t versionNum;   /* SW 1.0 and SW/Reg 1.1 = 0 or 1
                       * EA 1.2 = 2 */

uint8_t    fastPlay;
bool pentiumMode;

/* Savegame files */
uint8_t    gameSpeed;
uint8_t    processorType;  /* 1=386 2=486 3=Pentium Hyper */

JE_SaveFilesType saveFiles; /*array[1..saveLevelnum] of savefiletype;*/
JE_SaveGameTemp saveTemp;

uint16_t editorLevel;   /*Initial value 800*/


cJSON *load_json( const char *filename )
{
	FILE *f = dir_fopen_warn(get_user_directory(), filename, "rb");
	if (f == NULL)
		return NULL;
	
	size_t buffer_len = ftell_eof(f);
	char *buffer = malloc(buffer_len + 1);
	
	fread(buffer, 1, buffer_len, f);
	buffer[buffer_len] = '\0';
	
	fclose(f);
	
	cJSON *root = cJSON_Parse(buffer);
	
	free(buffer);
	
	return root;
}

void save_json( cJSON *root, const char *filename )
{
	FILE *f = dir_fopen_warn(get_user_directory(), filename, "w+");
	if (f == NULL)
		return;
	
	char *buffer = cJSON_Print(root);
	
	if (buffer != NULL)
	{
		fputs(buffer, f);
		free(buffer);
	}
	
	fclose(f);
}

bool load_opentyrian_config( void )
{
	// defaults
	fullscreen_enabled = false;
	set_scaler_by_name("Scale2x");
	
	cJSON *root = load_json("opentyrian.conf");
	if (root == NULL)
		return false;
	
	cJSON *section = cJSON_GetObjectItem(root, "video");
	if (section != NULL)
	{
		cJSON *setting;
		
		if ((setting = cJSON_GetObjectItem(section, "fullscreen")))
			fullscreen_enabled = (setting->type == cJSON_True);
		
		if ((setting = cJSON_GetObjectItem(section, "scaler")))
			set_scaler_by_name(setting->valuestring);
	}
	
	cJSON_Delete(root);
	
	return true;
}

bool save_opentyrian_config( void )
{
	cJSON *root = load_json("opentyrian.conf");
	if (root == NULL)
		root = cJSON_CreateObject();
	
	cJSON *section;
	
	section = cJSON_CreateOrGetObjectItem(root, "video");
	cJSON_ForceType(section, cJSON_Object);
	
	{
		cJSON *setting;
		
		setting = cJSON_CreateOrGetObjectItem(section, "fullscreen");
		cJSON_SetBoolean(setting, fullscreen_enabled);
		
		setting = cJSON_CreateOrGetObjectItem(section, "scaler");
		cJSON_SetString(setting, scalers[scaler].name);
	}
	
	save_json(root, "opentyrian.conf");
	
	cJSON_Delete(root);
	
	return true;
}


void JE_setupStars( void )
{
	int z;

	for (z = MAX_STARS; z--; )
	{
		starDat[z].sLoc = (mt_rand() % 320) + (mt_rand() % 200) * VGAScreen->pitch;
		starDat[z].sMov = ((mt_rand() % 3) + 2) * VGAScreen->pitch;
		starDat[z].sC = (mt_rand() % 16) + (9 * 16);
	}
}

static void playeritems_to_pitems( JE_PItemsType pItems, PlayerItems *items, uint8_t initial_episode_num )
{
	pItems[0]  = items->weapon[FRONT_WEAPON].id;
	pItems[1]  = items->weapon[REAR_WEAPON].id;
	pItems[2]  = items->super_arcade_mode;
	pItems[3]  = items->sidekick[LEFT_SIDEKICK];
	pItems[4]  = items->sidekick[RIGHT_SIDEKICK];
	pItems[5]  = items->generator;
	pItems[6]  = items->sidekick_level;
	pItems[7]  = items->sidekick_series;
	pItems[8]  = initial_episode_num;
	pItems[9]  = items->shield;
	pItems[10] = items->special;
	pItems[11] = items->ship;
}

static void pitems_to_playeritems( PlayerItems *items, JE_PItemsType pItems, uint8_t *initial_episode_num )
{
	items->weapon[FRONT_WEAPON].id  = pItems[0];
	items->weapon[REAR_WEAPON].id   = pItems[1];
	items->super_arcade_mode        = pItems[2];
	items->sidekick[LEFT_SIDEKICK]  = pItems[3];
	items->sidekick[RIGHT_SIDEKICK] = pItems[4];
	items->generator                = pItems[5];
	items->sidekick_level           = pItems[6];
	items->sidekick_series          = pItems[7];
	if (initial_episode_num != NULL)
		*initial_episode_num        = pItems[8];
	items->shield                   = pItems[9];
	items->special                  = pItems[10];
	items->ship                     = pItems[11];
}

void JE_saveGame( uint8_t slot, const char *name )
{
	saveFiles[slot-1].initialDifficulty = initialDifficulty;
	saveFiles[slot-1].gameHasRepeated = gameHasRepeated;
	saveFiles[slot-1].level = saveLevel;
	
	if (superTyrian)
		player[0].items.super_arcade_mode = SA_SUPERTYRIAN;
	else if (superArcadeMode == SA_NONE && onePlayerAction)
		player[0].items.super_arcade_mode = SA_ARCADE;
	else
		player[0].items.super_arcade_mode = superArcadeMode;
	
	playeritems_to_pitems(saveFiles[slot-1].items, &player[0].items, initial_episode_num);
	
	if (twoPlayerMode)
		playeritems_to_pitems(saveFiles[slot-1].lastItems, &player[1].items, 0);
	else
		playeritems_to_pitems(saveFiles[slot-1].lastItems, &player[0].last_items, 0);
	
	saveFiles[slot-1].score  = player[0].cash;
	saveFiles[slot-1].score2 = player[1].cash;
	
	memcpy(&saveFiles[slot-1].levelName, &lastLevelName, sizeof(lastLevelName));
	saveFiles[slot-1].cubes  = lastCubeMax;

	if (strcmp(lastLevelName, "Completed") == 0)
	{
		temp = episodeNum - 1;
		if (temp < 1)
		{
			temp = EPISODE_AVAILABLE; /* JE: {Episodemax is 4 for completion purposes} */
		}
		saveFiles[slot-1].episode = temp;
	}
	else
	{
		saveFiles[slot-1].episode = episodeNum;
	}

	saveFiles[slot-1].difficulty = difficultyLevel;
	saveFiles[slot-1].secretHint = secretHint;
	saveFiles[slot-1].input1 = inputDevice[0];
	saveFiles[slot-1].input2 = inputDevice[1];

	strcpy(saveFiles[slot-1].name, name);
	
	for (uint32_t port = 0; port < 2; ++port)
	{
		// if two-player, use first player's front and second player's rear weapon
		saveFiles[slot-1].power[port] = player[twoPlayerMode ? port : 0].items.weapon[port].power;
	}
	
	JE_saveConfiguration();
}

void JE_loadGame( uint8_t slot )
{
	uint8_t temp5;

	superTyrian = false;
	onePlayerAction = false;
	twoPlayerMode = false;
	extraGame = false;
	galagaMode = false;

	initialDifficulty = saveFiles[slot-1].initialDifficulty;
	gameHasRepeated   = saveFiles[slot-1].gameHasRepeated;
	twoPlayerMode     = (slot-1) > 10;
	difficultyLevel   = saveFiles[slot-1].difficulty;
	
	pitems_to_playeritems(&player[0].items, saveFiles[slot-1].items, &initial_episode_num);
	
	superArcadeMode = player[0].items.super_arcade_mode;
	
	if (superArcadeMode == SA_SUPERTYRIAN)
		superTyrian = true;
	if (superArcadeMode != SA_NONE)
		onePlayerAction = true;
	if (superArcadeMode > SA_NORTSHIPZ)
		superArcadeMode = SA_NONE;
	
	if (twoPlayerMode)
	{
		onePlayerAction = false;
		
		pitems_to_playeritems(&player[1].items, saveFiles[slot-1].lastItems, NULL);
	}
	else
	{
		pitems_to_playeritems(&player[0].last_items, saveFiles[slot-1].lastItems, NULL);
	}

	/* Compatibility with old version */
	if (player[1].items.sidekick_level < 101)
	{
		player[1].items.sidekick_level = 101;
		player[1].items.sidekick_series = player[1].items.sidekick[LEFT_SIDEKICK];
	}
	
	player[0].cash = saveFiles[slot-1].score;
	player[1].cash = saveFiles[slot-1].score2;
	
	mainLevel   = saveFiles[slot-1].level;
	cubeMax     = saveFiles[slot-1].cubes;
	lastCubeMax = cubeMax;

	secretHint = saveFiles[slot-1].secretHint;
	inputDevice[0] = saveFiles[slot-1].input1;
	inputDevice[1] = saveFiles[slot-1].input2;

	for (uint32_t port = 0; port < 2; ++port)
	{
		// if two-player, use first player's front and second player's rear weapon
		player[twoPlayerMode ? port : 0].items.weapon[port].power = saveFiles[slot-1].power[port];
	}
	
	temp5 = saveFiles[slot-1].episode;

	memcpy(&levelName, &saveFiles[slot-1].levelName, sizeof(levelName));

	if (strcmp(levelName, "Completed") == 0)
	{
		if (temp5 == EPISODE_AVAILABLE)
		{
			temp5 = 1;
		} else if (temp5 < EPISODE_AVAILABLE) {
			temp5++;
		}
		/* Increment episode.  Episode EPISODE_AVAILABLE goes to 1. */
	}

	JE_initEpisode(temp5);
	saveLevel = mainLevel;
	memcpy(&lastLevelName, &levelName, sizeof(levelName));
}

void JE_initProcessorType( void )
{
	/* SYN: Originally this proc looked at your hardware specs and chose appropriate options. We don't care, so I'll just set
	   decent defaults here. */

	wild = false;
	superWild = false;
	smoothScroll = true;
	explosionTransparent = true;
	filtrationAvail = false;
	background2 = true;
	displayScore = true;

	switch (processorType)
	{
		case 1: /* 386 */
			background2 = false;
			displayScore = false;
			explosionTransparent = false;
			break;
		case 2: /* 486 - Default */
			break;
		case 3: /* High Detail */
			smoothScroll = false;
			break;
		case 4: /* Pentium */
			wild = true;
			filtrationAvail = true;
			break;
		case 5: /* Nonstandard VGA */
			smoothScroll = false;
			break;
		case 6: /* SuperWild */
			wild = true;
			superWild = true;
			filtrationAvail = true;
			break;
	}

	switch (gameSpeed)
	{
		case 1:  /* Slug Mode */
			fastPlay = 3;
			break;
		case 2:  /* Slower */
			fastPlay = 4;
			break;
		case 3: /* Slow */
			fastPlay = 5;
			break;
		case 4: /* Normal */
			fastPlay = 0;
			break;
		case 5: /* Pentium Hyper */
			fastPlay = 1;
			break;
	}

}

void JE_setNewGameSpeed( void )
{
	pentiumMode = false;

	switch (fastPlay)
	{
	case 0:
		speed = 0x4300;
		smoothScroll = true;
		frameCountMax = 2;
		break;
	case 1:
		speed = 0x3000;
		smoothScroll = true;
		frameCountMax = 2;
		break;
	case 2:
		speed = 0x2000;
		smoothScroll = false;
		frameCountMax = 2;
		break;
	case 3:
		speed = 0x5300;
		smoothScroll = true;
		frameCountMax = 4;
		break;
	case 4:
		speed = 0x4300;
		smoothScroll = true;
		frameCountMax = 3;
		break;
	case 5:
		speed = 0x4300;
		smoothScroll = true;
		frameCountMax = 2;
		pentiumMode = true;
		break;
	}

  frameCount = frameCountMax;
  JE_resetTimerInt();
  JE_setTimerInt();
}

void JE_encryptSaveTemp( void )
{
	JE_SaveGameTemp s3;
	uint16_t x;
	uint8_t y;

	memcpy(&s3, &saveTemp, sizeof(s3));

	y = 0;
	for (x = 0; x < SAVE_FILE_SIZE; x++)
	{
		y += s3[x];
	}
	saveTemp[SAVE_FILE_SIZE] = y;

	y = 0;
	for (x = 0; x < SAVE_FILE_SIZE; x++)
	{
		y -= s3[x];
	}
	saveTemp[SAVE_FILE_SIZE+1] = y;

	y = 1;
	for (x = 0; x < SAVE_FILE_SIZE; x++)
	{
		y = (y * s3[x]) + 1;
	}
	saveTemp[SAVE_FILE_SIZE+2] = y;

	y = 0;
	for (x = 0; x < SAVE_FILE_SIZE; x++)
	{
		y = y ^ s3[x];
	}
	saveTemp[SAVE_FILE_SIZE+3] = y;

	for (x = 0; x < SAVE_FILE_SIZE; x++)
	{
		saveTemp[x] = saveTemp[x] ^ cryptKey[(x+1) % 10];
		if (x > 0)
		{
			saveTemp[x] = saveTemp[x] ^ saveTemp[x - 1];
		}
	}
}

void JE_decryptSaveTemp( void )
{
	bool correct = true;
	JE_SaveGameTemp s2;
	int x;
	uint8_t y;

	/* Decrypt save game file */
	for (x = (SAVE_FILE_SIZE - 1); x >= 0; x--)
	{
		s2[x] = (uint8_t)saveTemp[x] ^ (uint8_t)(cryptKey[(x+1) % 10]);
		if (x > 0)
		{
			s2[x] ^= (uint8_t)saveTemp[x - 1];
		}

	}

	/* for (x = 0; x < SAVE_FILE_SIZE; x++) printf("%c", s2[x]); */

	/* Check save file for correctitude */
	y = 0;
	for (x = 0; x < SAVE_FILE_SIZE; x++)
	{
		y += s2[x];
	}
	if (saveTemp[SAVE_FILE_SIZE] != y)
	{
		correct = false;
		printf("Failed additive checksum: %d vs %d\n", saveTemp[SAVE_FILE_SIZE], y);
	}

	y = 0;
	for (x = 0; x < SAVE_FILE_SIZE; x++)
	{
		y -= s2[x];
	}
	if (saveTemp[SAVE_FILE_SIZE+1] != y)
	{
		correct = false;
		printf("Failed subtractive checksum: %d vs %d\n", saveTemp[SAVE_FILE_SIZE+1], y);
	}

	y = 1;
	for (x = 0; x < SAVE_FILE_SIZE; x++)
	{
		y = (y * s2[x]) + 1;
	}
	if (saveTemp[SAVE_FILE_SIZE+2] != y)
	{
		correct = false;
		printf("Failed multiplicative checksum: %d vs %d\n", saveTemp[SAVE_FILE_SIZE+2], y);
	}

	y = 0;
	for (x = 0; x < SAVE_FILE_SIZE; x++)
	{
		y = y ^ s2[x];
	}
	if (saveTemp[SAVE_FILE_SIZE+3] != y)
	{
		correct = false;
		printf("Failed XOR'd checksum: %d vs %d\n", saveTemp[SAVE_FILE_SIZE+3], y);
	}

	/* Barf and die if save file doesn't validate */
	if (!correct)
	{
		fprintf(stderr, "Error reading save file!\n");
		exit(255);
	}

	/* Keep decrypted version plz */
	memcpy(&saveTemp, &s2, sizeof(s2));
}

#ifndef TARGET_MACOSX
const char *get_user_directory( void )
{
	static char user_dir[500] = "";
	
	if (strlen(user_dir) == 0)
	{
#ifdef TARGET_UNIX
		if (getenv("HOME"))
			snprintf(user_dir, sizeof(user_dir), "%s/.opentyrian", getenv("HOME"));
#else
		strcpy(user_dir, ".");
#endif // TARGET_UNIX
	}
	
	return user_dir;
}
#endif // TARGET_MACOSX

// for compatibility
Uint8 joyButtonAssign[4] = {1, 4, 5, 5};
Uint8 inputDevice_ = 0, jConfigure = 0, midiPort = 1;

void JE_loadConfiguration( void )
{
	FILE *fi;
	int z;
	uint8_t *p;
	int y;
	
	fi = dir_fopen_warn(get_user_directory(), "tyrian.cfg", "rb");
	if (fi && ftell_eof(fi) == 20 + sizeof(keySettings))
	{
		/* SYN: I've hardcoded the sizes here because the .CFG file format is fixed
		   anyways, so it's not like they'll change. */
		background2 = 0;
		efread(&background2, 1, 1, fi);
		efread(&gameSpeed, 1, 1, fi);
		
		efread(&inputDevice_, 1, 1, fi);
		efread(&jConfigure, 1, 1, fi);
		
		efread(&versionNum, 1, 1, fi);
		
		efread(&processorType, 1, 1, fi);
		efread(&midiPort, 1, 1, fi);
		efread(&soundEffects, 1, 1, fi);
		efread(&gammaCorrection, 1, 1, fi);
		efread(&difficultyLevel, 1, 1, fi);
		
		efread(joyButtonAssign, 1, 4, fi);
		
		efread(&tyrMusicVolume, 2, 1, fi);
		efread(&fxVolume, 2, 1, fi);
		
		efread(inputDevice, 1, 2, fi);
		
		efread(keySettings, sizeof(*keySettings), COUNTOF(keySettings), fi);
		
		fclose(fi);
	}
	else
	{
		printf("\nInvalid or missing TYRIAN.CFG! Continuing using defaults.\n\n");
		
		soundEffects = 1;
		memcpy(&keySettings, &defaultKeySettings, sizeof(keySettings));
		background2 = true;
		tyrMusicVolume = fxVolume = 128;
		gammaCorrection = 0;
		processorType = 3;
		gameSpeed = 4;
	}
	
	load_opentyrian_config();
	
	if (tyrMusicVolume > 255)
		tyrMusicVolume = 255;
	if (fxVolume > 255)
		fxVolume = 255;
	
	JE_calcFXVol();
	
	set_volume(tyrMusicVolume, fxVolume);
	
	fi = dir_fopen_warn(get_user_directory(), "tyrian.sav", "rb");
	if (fi)
	{

		fseek(fi, 0, SEEK_SET);
		efread(saveTemp, 1, sizeof(saveTemp), fi);
		JE_decryptSaveTemp();

		/* SYN: The original mostly blasted the save file into raw memory. However, our lives are not so
		   easy, because the C struct is necessarily a different size. So instead we have to loop
		   through each record and load fields manually. *emo tear* :'( */

		p = saveTemp;
		for (z = 0; z < SAVE_FILES_NUM; z++)
		{
			memcpy(&saveFiles[z].encode, p, sizeof(uint16_t)); p += 2;
			saveFiles[z].encode = SDL_SwapLE16(saveFiles[z].encode);
			
			memcpy(&saveFiles[z].level, p, sizeof(uint16_t)); p += 2;
			saveFiles[z].level = SDL_SwapLE16(saveFiles[z].level);
			
			memcpy(&saveFiles[z].items, p, sizeof(JE_PItemsType)); p += sizeof(JE_PItemsType);
			
			memcpy(&saveFiles[z].score, p, sizeof(int32_t)); p += 4;
			saveFiles[z].score = SDL_SwapLE32(saveFiles[z].score);
			
			memcpy(&saveFiles[z].score2, p, sizeof(int32_t)); p += 4;
			saveFiles[z].score2 = SDL_SwapLE32(saveFiles[z].score2);
			
			/* SYN: Pascal strings are prefixed by a byte holding the length! */
			memset(&saveFiles[z].levelName, 0, sizeof(saveFiles[z].levelName));
			memcpy(&saveFiles[z].levelName, &p[1], *p);
			p += 10;
			
			/* This was a BYTE array, not a STRING, in the original. Go fig. */
			memcpy(&saveFiles[z].name, p, 14);
			p += 14;
			
			memcpy(&saveFiles[z].cubes, p, sizeof(uint8_t)); p++;
			memcpy(&saveFiles[z].power, p, sizeof(uint8_t) * 2); p += 2;
			memcpy(&saveFiles[z].episode, p, sizeof(uint8_t)); p++;
			memcpy(&saveFiles[z].lastItems, p, sizeof(JE_PItemsType)); p += sizeof(JE_PItemsType);
			memcpy(&saveFiles[z].difficulty, p, sizeof(uint8_t)); p++;
			memcpy(&saveFiles[z].secretHint, p, sizeof(uint8_t)); p++;
			memcpy(&saveFiles[z].input1, p, sizeof(uint8_t)); p++;
			memcpy(&saveFiles[z].input2, p, sizeof(uint8_t)); p++;
			
			/* booleans were 1 byte in pascal -- working around it */
			Uint8 temp;
			memcpy(&temp, p, 1); p++;
			saveFiles[z].gameHasRepeated = temp != 0;
			
			memcpy(&saveFiles[z].initialDifficulty, p, sizeof(uint8_t)); p++;
			
			memcpy(&saveFiles[z].highScore1, p, sizeof(int32_t)); p += 4;
			saveFiles[z].highScore1 = SDL_SwapLE32(saveFiles[z].highScore1);
			
			memcpy(&saveFiles[z].highScore2, p, sizeof(int32_t)); p += 4;
			saveFiles[z].highScore2 = SDL_SwapLE32(saveFiles[z].highScore2);
			
			memset(&saveFiles[z].highScoreName, 0, sizeof(saveFiles[z].highScoreName));
			memcpy(&saveFiles[z].highScoreName, &p[1], *p);
			p += 30;
			
			memcpy(&saveFiles[z].highScoreDiff, p, sizeof(uint8_t)); p++;
		}

		/* SYN: This is truncating to bytes. I have no idea what this is doing or why. */
		/* TODO: Figure out what this is about and make sure it isn't broked. */
		editorLevel = (saveTemp[SIZEOF_SAVEGAMETEMP - 5] << 8) | saveTemp[SIZEOF_SAVEGAMETEMP - 6];

		fclose(fi);
	} else {
		/* We didn't have a save file! Let's make up random stuff! */
		editorLevel = 800;

		for (z = 0; z < 100; z++)
		{
			saveTemp[SAVE_FILES_SIZE + z] = initialItemAvail[z];
		}

		for (z = 0; z < SAVE_FILES_NUM; z++)
		{
			saveFiles[z].level = 0;

			for (y = 0; y < 14; y++)
			{
				saveFiles[z].name[y] = ' ';
			}
			saveFiles[z].name[14] = 0;

			saveFiles[z].highScore1 = ((mt_rand() % 20) + 1) * 1000;

			if (z % 6 > 2)
			{
				saveFiles[z].highScore2 = ((mt_rand() % 20) + 1) * 1000;
				strcpy(saveFiles[z].highScoreName, defaultTeamNames[mt_rand() % 22]);
			} else {
				strcpy(saveFiles[z].highScoreName, defaultHighScoreNames[mt_rand() % 34]);
			}
		}
	}
	
	JE_initProcessorType();
}

void JE_saveConfiguration( void )
{
#ifdef TARGET_UNIX
	if (getenv("HOME"))
	{
		char dir[1000];
		snprintf(dir, sizeof(dir), "%s/.opentyrian", getenv("HOME"));
		mkdir(dir, 0755);
	}
#endif /* HOME */
	
	FILE *f;
	uint8_t *p;
	int z;

	p = saveTemp;
	for (z = 0; z < SAVE_FILES_NUM; z++)
	{
		JE_SaveFileType tempSaveFile;
		memcpy(&tempSaveFile, &saveFiles[z], sizeof(tempSaveFile));
		
		tempSaveFile.encode = SDL_SwapLE16(tempSaveFile.encode);
		memcpy(p, &tempSaveFile.encode, sizeof(uint16_t)); p += 2;
		
		tempSaveFile.level = SDL_SwapLE16(tempSaveFile.level);
		memcpy(p, &tempSaveFile.level, sizeof(uint16_t)); p += 2;
		
		memcpy(p, &tempSaveFile.items, sizeof(JE_PItemsType)); p += sizeof(JE_PItemsType);
		
		tempSaveFile.score = SDL_SwapLE32(tempSaveFile.score);
		memcpy(p, &tempSaveFile.score, sizeof(int32_t)); p += 4;
		
		tempSaveFile.score2 = SDL_SwapLE32(tempSaveFile.score2);
		memcpy(p, &tempSaveFile.score2, sizeof(int32_t)); p += 4;
		
		/* SYN: Pascal strings are prefixed by a byte holding the length! */
		memset(p, 0, sizeof(tempSaveFile.levelName));
		*p = strlen(tempSaveFile.levelName);
		memcpy(&p[1], &tempSaveFile.levelName, *p);
		p += 10;
		
		/* This was a BYTE array, not a STRING, in the original. Go fig. */
		memcpy(p, &tempSaveFile.name, 14);
		p += 14;
		
		memcpy(p, &tempSaveFile.cubes, sizeof(uint8_t)); p++;
		memcpy(p, &tempSaveFile.power, sizeof(uint8_t) * 2); p += 2;
		memcpy(p, &tempSaveFile.episode, sizeof(uint8_t)); p++;
		memcpy(p, &tempSaveFile.lastItems, sizeof(JE_PItemsType)); p += sizeof(JE_PItemsType);
		memcpy(p, &tempSaveFile.difficulty, sizeof(uint8_t)); p++;
		memcpy(p, &tempSaveFile.secretHint, sizeof(uint8_t)); p++;
		memcpy(p, &tempSaveFile.input1, sizeof(uint8_t)); p++;
		memcpy(p, &tempSaveFile.input2, sizeof(uint8_t)); p++;
		
		/* booleans were 1 byte in pascal -- working around it */
		Uint8 temp = tempSaveFile.gameHasRepeated != false;
		memcpy(p, &temp, 1); p++;
		
		memcpy(p, &tempSaveFile.initialDifficulty, sizeof(uint8_t)); p++;
		
		tempSaveFile.highScore1 = SDL_SwapLE32(tempSaveFile.highScore1);
		memcpy(p, &tempSaveFile.highScore1, sizeof(int32_t)); p += 4;
		
		tempSaveFile.highScore2 = SDL_SwapLE32(tempSaveFile.highScore2);
		memcpy(p, &tempSaveFile.highScore2, sizeof(int32_t)); p += 4;
		
		memset(p, 0, sizeof(tempSaveFile.highScoreName));
		*p = strlen(tempSaveFile.highScoreName);
		memcpy(&p[1], &tempSaveFile.highScoreName, *p);
		p += 30;
		
		memcpy(p, &tempSaveFile.highScoreDiff, sizeof(uint8_t)); p++;
	}
	
	saveTemp[SIZEOF_SAVEGAMETEMP - 6] = editorLevel >> 8;
	saveTemp[SIZEOF_SAVEGAMETEMP - 5] = editorLevel;
	
	JE_encryptSaveTemp();
	
	f = dir_fopen_warn(get_user_directory(), "tyrian.sav", "wb");
	if (f)
	{
		efwrite(saveTemp, 1, sizeof(saveTemp), f);
		fclose(f);
#if (_BSD_SOURCE || _XOPEN_SOURCE >= 500)
		sync();
#endif
	}
	JE_decryptSaveTemp();
	
	f = dir_fopen_warn(get_user_directory(), "tyrian.cfg", "wb");
	if (f)
	{
		efwrite(&background2, 1, 1, f);
		efwrite(&gameSpeed, 1, 1, f);
		
		efwrite(&inputDevice_, 1, 1, f);
		efwrite(&jConfigure, 1, 1, f);
		
		efwrite(&versionNum, 1, 1, f);
		efwrite(&processorType, 1, 1, f);
		efwrite(&midiPort, 1, 1, f);
		efwrite(&soundEffects, 1, 1, f);
		efwrite(&gammaCorrection, 1, 1, f);
		efwrite(&difficultyLevel, 1, 1, f);
		efwrite(joyButtonAssign, 1, 4, f);
		
		efwrite(&tyrMusicVolume, 2, 1, f);
		efwrite(&fxVolume, 2, 1, f);
		
		efwrite(inputDevice, 1, 2, f);
		
		efwrite(keySettings, sizeof(*keySettings), COUNTOF(keySettings), f);
		
		fclose(f);
	}
	
	save_opentyrian_config();
	
#if (_BSD_SOURCE || _XOPEN_SOURCE >= 500)
	sync();
#endif
}
