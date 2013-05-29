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
#ifndef CONFIG_H
#define CONFIG_H

#include "cJSON.h"
#include "opentyr.h"

#include <stdio.h>


#define SAVE_FILES_NUM (11 * 2)

#define MAX_STARS 100

/* These are necessary because the size of the structure has changed from the original, but we
   need to know the original sizes in order to find things in TYRIAN.SAV */
#define SAVE_FILES_SIZE 2398
#define SIZEOF_SAVEGAMETEMP SAVE_FILES_SIZE + 4 + 100
#define SAVE_FILE_SIZE (SIZEOF_SAVEGAMETEMP - 4)

/*#define SAVE_FILES_SIZE (2502 - 4)
#define SAVE_FILE_SIZE (SAVE_FILES_SIZE)*/

typedef SDLKey JE_KeySettingType[8]; /* [1..8] */
typedef uint8_t JE_PItemsType[12]; /* [1..12] */

typedef uint8_t JE_EditorItemAvailType[100]; /* [1..100] */

typedef struct
{
	uint16_t       encode;
	uint16_t       level;
	JE_PItemsType items;
	int32_t    score;
	int32_t    score2;
	char          levelName[11]; /* string [9]; */ /* SYN: Added one more byte to match lastLevelName below */
	char       name[15]; /* [1..14] */ /* SYN: Added extra byte for null */
	uint8_t       cubes;
	uint8_t       power[2]; /* [1..2] */
	uint8_t       episode;
	JE_PItemsType lastItems;
	uint8_t       difficulty;
	uint8_t       secretHint;
	uint8_t       input1;
	uint8_t       input2;
	bool    gameHasRepeated; /*See if you went from one episode to another*/
	uint8_t       initialDifficulty;

	/* High Scores - Each episode has both sets of 1&2 player selections - with 3 in each */
	int32_t    highScore1,
	              highScore2;
	char          highScoreName[30]; /* string [29] */
	uint8_t       highScoreDiff;
} JE_SaveFileType;

typedef JE_SaveFileType JE_SaveFilesType[SAVE_FILES_NUM]; /* [1..savefilesnum] */
typedef uint8_t JE_SaveGameTemp[SAVE_FILES_SIZE + 4 + 100]; /* [1..sizeof(savefilestype) + 4 + 100] */

typedef struct
{
	uint8_t sC;
	uint16_t sLoc;
	uint16_t sMov;
} StarDatType;

extern const uint8_t cryptKey[10];
extern const JE_KeySettingType defaultKeySettings;
extern const char defaultHighScoreNames[34][23];
extern const char defaultTeamNames[22][25];
extern const JE_EditorItemAvailType initialItemAvail;
extern bool smoothies[9];
extern uint8_t starShowVGASpecialCode;
extern StarDatType starDat[MAX_STARS];
extern uint16_t starY;
extern uint16_t lastCubeMax, cubeMax;
extern uint16_t cubeList[4];
extern bool gameHasRepeated;
extern int8_t difficultyLevel, oldDifficultyLevel, initialDifficulty;
extern uint32_t power, lastPower, powerAdd;
extern uint8_t shieldWait, shieldT;

enum
{
	SHOT_FRONT,
	SHOT_REAR,
	SHOT_LEFT_SIDEKICK,
	SHOT_RIGHT_SIDEKICK,
	SHOT_MISC,
	SHOT_P2_CHARGE,
	SHOT_P1_SUPERBOMB,
	SHOT_P2_SUPERBOMB,
	SHOT_SPECIAL,
	SHOT_NORTSPARKS,
	SHOT_SPECIAL2
};

extern uint8_t shotRepeat[11], shotMultiPos[11];
extern bool portConfigChange, portConfigDone;
extern char lastLevelName[11], levelName[11];
extern uint8_t mainLevel, nextLevel, saveLevel;
extern JE_KeySettingType keySettings;
extern int8_t levelFilter, levelFilterNew, levelBrightness, levelBrightnessChg;
extern bool filtrationAvail, filterActive, filterFade, filterFadeStart;
extern bool gameJustLoaded;
extern bool galagaMode;
extern bool extraGame;
extern bool twoPlayerMode, twoPlayerLinked, onePlayerAction, superTyrian, trentWin;
extern uint8_t superArcadeMode;
extern uint8_t superArcadePowerUp;
extern float linkGunDirec;
extern uint8_t inputDevice[2];
extern uint8_t secretHint;
extern uint8_t background3over;
extern uint8_t background2over;
extern uint8_t gammaCorrection;
extern bool superPause, explosionTransparent, youAreCheating, displayScore, background2, smoothScroll, wild, superWild, starActive, topEnemyOver, skyEnemyOverAll, background2notTransparent;
extern uint8_t versionNum;
extern uint8_t fastPlay;
extern bool pentiumMode;
extern uint8_t gameSpeed;
extern uint8_t processorType;
extern JE_SaveFilesType saveFiles;
extern JE_SaveGameTemp saveTemp;
extern uint16_t editorLevel;

void JE_initProcessorType( void );
void JE_setNewGameSpeed( void );
const char *get_user_directory( void );
void JE_loadConfiguration( void );
void JE_saveConfiguration( void );

void JE_setupStars( void );

void JE_saveGame( uint8_t slot, const char *name );
void JE_loadGame( uint8_t slot );

void JE_encryptSaveTemp( void );
void JE_decryptSaveTemp( void );

cJSON *load_json( const char *filename );
void save_json( cJSON *root, const char *filename );

#endif /* CONFIG_H */

