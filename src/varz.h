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
#ifndef VARZ_H
#define VARZ_H

#include "episodes.h"
#include "opentyr.h"
#include "player.h"
#include "sprite.h"

#include <stdbool.h>

#define SA 7

enum
{
	SA_NONE = 0,
	SA_NORTSHIPZ = 7,
	
	// only used for code entry
	SA_DESTRUCT = 8,
	SA_ENGAGE = 9,
	
	// only used in pItems[P_SUPERARCADE]
	SA_SUPERTYRIAN = 254,
	SA_ARCADE = 255
};

#define MAX_PWEAPON     81 /* 81*/
#define ENEMY_SHOT_MAX  60 /* 60*/

#define CURRENT_KEY_SPEED 1  /*Keyboard/Joystick movement rate*/

#define MAX_EXPLOSIONS           200
#define MAX_REPEATING_EXPLOSIONS 20
#define MAX_SUPERPIXELS          101

struct JE_SingleEnemyType
{
	uint8_t     fillbyte;
	int16_t  ex, ey;     /* POSITION */
	int8_t exc, eyc;   /* CURRENT SPEED */
	int8_t exca, eyca; /* RANDOM ACCELERATION */
	int8_t excc, eycc; /* FIXED ACCELERATION WAITTIME */
	int8_t exccw, eyccw;
	uint8_t     armorleft;
	uint8_t     eshotwait[3], eshotmultipos[3]; /* [1..3] */
	uint8_t     enemycycle;
	uint8_t     ani;
	uint16_t     egr[20]; /* [1..20] */
	uint8_t     size;
	uint8_t     linknum;
	uint8_t     aniactive;
	uint8_t     animax;
	uint8_t     aniwhenfire;
	Sprite2_array *sprite2s;
	int8_t exrev, eyrev;
	int16_t  exccadd, eyccadd;
	uint8_t     exccwmax, eyccwmax;
	void       *enemydatofs;
	bool  edamaged;
	uint16_t     enemytype;
	uint8_t     animin;
	uint16_t     edgr;
	int8_t edlevel;
	int8_t edani;
	uint8_t     fill1;
	uint8_t     filter;
	int16_t  evalue;
	int16_t  fixedmovey;
	uint8_t     freq[3]; /* [1..3] */
	uint8_t     launchwait;
	uint16_t     launchtype;
	uint8_t     launchfreq;
	uint8_t     xaccel;
	uint8_t     yaccel;
	uint8_t     tur[3]; /* [1..3] */
	uint16_t     enemydie; /* Enemy created when this one dies */
	bool  enemyground;
	uint8_t     explonum;
	uint16_t     mapoffset;
	bool  scoreitem;

	bool  special;
	uint8_t     flagnum;
	bool  setto;

	uint8_t     iced; /*Duration*/

	uint8_t     launchspecial;

	int16_t  xminbounce;
	int16_t  xmaxbounce;
	int16_t  yminbounce;
	int16_t  ymaxbounce;
	uint8_t     fill[3]; /* [1..3] */
};

typedef struct JE_SingleEnemyType JE_MultiEnemyType[100]; /* [1..100] */

typedef uint16_t JE_DanCShape[(24 * 28) / 2]; /* [1..(24*28) div 2] */

typedef char JE_CharString[256]; /* [1..256] */

typedef uint8_t JE_Map1Buffer[24 * 28 * 13 * 4]; /* [1..24*28*13*4] */

typedef uint8_t *JE_MapType[300][14]; /* [1..300, 1..14] */
typedef uint8_t *JE_MapType2[600][14]; /* [1..600, 1..14] */
typedef uint8_t *JE_MapType3[600][15]; /* [1..600, 1..15] */

struct JE_EventRecType
{
	uint16_t     eventtime;
	uint8_t     eventtype;
	int16_t  eventdat, eventdat2;
	int8_t eventdat3, eventdat5, eventdat6;
	uint8_t     eventdat4;
};

struct JE_MegaDataType1
{
	JE_MapType mainmap;
	struct
	{
		JE_DanCShape sh;
	} shapes[72]; /* [0..71] */
	uint8_t tempdat1;
	/*JE_DanCShape filler;*/
};

struct JE_MegaDataType2
{
	JE_MapType2 mainmap;
	struct
	{
		uint8_t nothing[3]; /* [1..3] */
		uint8_t fill;
		JE_DanCShape sh;
	} shapes[71]; /* [0..70] */
	uint8_t tempdat2;
};

struct JE_MegaDataType3
{
	JE_MapType3 mainmap;
	struct
	{
		uint8_t nothing[3]; /* [1..3] */
		uint8_t fill;
		JE_DanCShape sh;
	} shapes[70]; /* [0..69] */
	uint8_t tempdat3;
};

typedef JE_MultiEnemyType JE_EnemyType;
typedef uint8_t JE_EnemyAvailType[100]; /* [1..100] */

typedef struct {
	int16_t sx, sy;
	int16_t sxm, sym;
	int8_t sxc, syc;
	uint8_t tx, ty;
	uint16_t sgr;
	uint8_t sdmg;
	uint8_t duration;
	uint16_t animate;
	uint16_t animax;
	uint8_t fill[12];
} EnemyShotType;

typedef struct {
	int16_t shotX, shotY, shotXM, shotYM, shotXC, shotYC;
	bool shotComplicated;
	int16_t shotDevX, shotDirX, shotDevY, shotDirY, shotCirSizeX, shotCirSizeY;
	uint8_t shotTrail;
	uint16_t shotGr, shotAni, shotAniMax;
	Uint8 shotDmg;
	uint8_t shotBlastFilter, chainReaction, playerNumber, aimAtEnemy, aimDelay, aimDelayMax;
} PlayerShotDataType;

typedef struct {
	unsigned int ttl;
	signed int x, y;
	signed int delta_x, delta_y;
	bool fixed_position;
	bool follow_player;
	unsigned int sprite;
} explosion_type;

typedef struct {
	unsigned int delay;
	unsigned int ttl;
	unsigned int x, y;
	bool big;
} rep_explosion_type;

typedef struct {
	unsigned int x, y, z;
	signed int delta_x, delta_y;
	Uint8 color;
} superpixel_type;

extern int16_t tempDat, tempDat2, tempDat3;
extern const uint8_t SANextShip[SA + 2];
extern const uint16_t SASpecialWeapon[SA];
extern const uint16_t SASpecialWeaponB[SA];
extern const uint8_t SAShip[SA];
extern const uint16_t SAWeapon[SA][5];
extern const uint8_t specialArcadeWeapon[PORT_NUM];
extern const uint8_t optionSelect[16][3][2];
extern const uint16_t PGR[21];
extern const uint8_t PAni[21];
extern const uint16_t linkGunWeapons[38];
extern const uint16_t chargeGunWeapons[38];
extern const uint16_t linkMultiGr[17];
extern const uint16_t linkSonicGr[17];
extern const uint16_t linkMult2Gr[17];
extern const uint8_t randomEnemyLaunchSounds[3];
extern const uint8_t keyboardCombos[26][8];
extern const uint8_t shipCombosB[21];
extern const uint8_t superTyrianSpecials[4];
extern const uint8_t shipCombos[14][3];
extern uint8_t SFCurrentCode[2][21];
extern uint8_t SFExecuted[2];
extern uint8_t lvlFileNum;
extern uint16_t maxEvent, eventLoc;
extern uint16_t tempBackMove, explodeMove;
extern uint8_t levelEnd;
extern uint16_t levelEndFxWait;
extern int8_t levelEndWarp;
extern bool endLevel, reallyEndLevel, waitToEndLevel, playerEndLevel, normalBonusLevelCurrent, bonusLevelCurrent, smallEnemyAdjust, readyToEndLevel, quitRequested;
extern uint8_t newPL[10];
extern uint16_t returnLoc;
extern bool returnActive;
extern uint16_t galagaShotFreq;
extern int32_t galagaLife;
extern bool debug;
extern uint32_t debugTime, lastDebugTime;
extern int32_t debugHistCount;
extern float debugHist;
extern uint16_t curLoc;
extern bool firstGameOver, gameLoaded, enemyStillExploding;
extern uint16_t totalEnemy;
extern uint16_t enemyKilled;
extern struct JE_MegaDataType1 megaData1;
extern struct JE_MegaDataType2 megaData2;
extern struct JE_MegaDataType3 megaData3;
extern uint8_t flash;
extern int8_t flashChange;
extern uint8_t displayTime;

extern bool play_demo, record_demo, stopped_demo;
extern Uint8 demo_num;
extern FILE *demo_file;

extern Uint8 demo_keys, next_demo_keys;
extern Uint16 demo_keys_wait;

extern uint8_t soundQueue[8];
extern bool enemyContinualDamage;
extern bool enemiesActive;
extern bool forceEvents;
extern bool stopBackgrounds;
extern uint8_t stopBackgroundNum;
extern uint8_t damageRate;
extern bool background3x1;
extern bool background3x1b;
extern bool levelTimer;
extern uint16_t levelTimerCountdown;
extern uint16_t levelTimerJumpTo;
extern bool randomExplosions;
extern bool editShip1, editShip2;
extern bool globalFlags[10];
extern uint8_t levelSong;
extern bool loadDestruct;
extern uint16_t mapOrigin, mapPNum;
extern uint8_t mapPlanet[5], mapSection[5];
extern bool moveTyrianLogoUp;
extern bool skipStarShowVGA;
extern JE_EnemyType enemy;
extern JE_EnemyAvailType enemyAvail;
extern uint16_t enemyOffset;
extern uint16_t enemyOnScreen;
extern uint8_t enemyShapeTables[6];
extern uint16_t superEnemy254Jump;
extern explosion_type explosions[MAX_EXPLOSIONS];
extern int16_t explosionFollowAmountX, explosionFollowAmountY;
extern bool fireButtonHeld;
extern bool enemyShotAvail[ENEMY_SHOT_MAX];
extern EnemyShotType enemyShot[ENEMY_SHOT_MAX];
extern uint8_t zinglonDuration;
extern uint8_t astralDuration;
extern uint16_t flareDuration;
extern bool flareStart;
extern int8_t flareColChg;
extern uint8_t specialWait;
extern uint8_t nextSpecialWait;
extern bool spraySpecial;
extern uint8_t doIced;
extern bool infiniteShot;
extern PlayerShotDataType playerShotData[MAX_PWEAPON + 1];
extern uint8_t chain;
extern bool allPlayersGone;
extern uint8_t shotAvail[MAX_PWEAPON];
extern const uint32_t shadowYDist;
extern float optionSatelliteRotate;
extern int16_t optionAttachmentMove;
extern bool optionAttachmentLinked, optionAttachmentReturn;
extern uint8_t chargeWait, chargeLevel, chargeMax, chargeGr, chargeGrWait;
extern uint16_t neat;
extern rep_explosion_type rep_explosions[MAX_REPEATING_EXPLOSIONS];
extern superpixel_type superpixels[MAX_SUPERPIXELS];
extern unsigned int last_superpixel;
extern int16_t tempI, tempI2, tempI3, tempI4;
extern int32_t tempL;
extern uint8_t temp, temp2, temp3, temp4, temp5, tempPos;
extern uint16_t tempX, tempY, tempX2, tempY2;
extern uint16_t tempW, tempW2;
extern bool doNotSaveBackup;
extern uint16_t x, y;
extern int16_t b;
extern uint8_t playerNum;
extern uint8_t **BKwrap1to, **BKwrap2to, **BKwrap3to, **BKwrap1, **BKwrap2, **BKwrap3;
extern int8_t specialWeaponFilter, specialWeaponFreq;
extern uint16_t specialWeaponWpn;
extern bool linkToPlayer;
extern uint16_t shipGr, shipGr2;
extern Sprite2_array *shipGrPtr, *shipGr2ptr;

static const int hud_sidekick_y[2][2] =
{
	{  64,  82 }, // one player HUD
	{ 108, 126 }, // two player HUD
};

void JE_getShipInfo( void );
uint16_t JE_SGr( uint16_t ship, Sprite2_array **ptr );

void JE_drawOptions( void );

void JE_tyrianHalt( uint8_t code ); /* This ends the game */
void JE_initPlayerShot( uint16_t portnum, uint32_t shot_i, uint16_t px, uint16_t py,
                        uint16_t mousex, uint16_t mousey,
                        uint16_t wpnum, uint8_t playernum );
void JE_specialComplete( uint8_t playernum, uint8_t specialType );
void JE_doSpecialShot( uint8_t playernum, uint32_t *armor, uint32_t *shield );

void JE_wipeShieldArmorBars( void );
uint8_t JE_playerDamage( uint8_t temp, Player * );

void JE_setupExplosion( signed int x, signed int y, signed int delta_y, unsigned int type, bool fixed_position, bool follow_player );
void JE_setupExplosionLarge( bool enemyground, uint8_t explonum, int16_t x, int16_t y );

void JE_drawShield( void );
void JE_drawArmor( void );

uint16_t JE_portConfigs( void );

/*SuperPixels*/
void JE_doSP( uint16_t x, uint16_t y, uint16_t num, uint8_t explowidth, uint8_t color );
void JE_drawSP( void );

void JE_drawOptionLevel( void );


#endif /* VARZ_H */

