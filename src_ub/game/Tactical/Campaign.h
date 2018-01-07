#ifndef __CAMPAIGN_H
#define __CAMPAIGN_H

#include "Soldier Profile.h"

#define SALARYAMT               0
#define HEALTHAMT               1
#define AGILAMT                 2
#define DEXTAMT                 3
#define WISDOMAMT               4
#define MEDICALAMT              5
#define EXPLODEAMT              6
#define MECHANAMT               7
#define MARKAMT                 8
#define EXPERAMT                9
#define STRAMT									10
#define LDRAMT									11
#define ASSIGNAMT               12
#define NAMEAMT                 13

#define FIRST_CHANGEABLE_STAT		HEALTHAMT
#define LAST_CHANGEABLE_STAT		LDRAMT
#define CHANGEABLE_STAT_COUNT		( LDRAMT - HEALTHAMT + 1 )

#define MAX_STAT_VALUE 100			// for stats and skills
#define MAXEXPLEVEL     10      // maximum merc experience level

#define SKILLS_SUBPOINTS_TO_IMPROVE										15		//18	//Ja2 Value: 25
#define ATTRIBS_SUBPOINTS_TO_IMPROVE									30		//35	//Ja2 Value: 50

#define LEVEL_SUBPOINTS_TO_IMPROVE										200		//245	//Ja2 Value: 350    // per current level!	(Can't go over 6500, 10x must fit in USHORT!)
#define LEVEL_SUBPOINTS_TO_IMPROVE_IMPORTED_GAME			245		//Ja2 Value: 350    // per current level!	(Can't go over 6500, 10x must fit in USHORT!)

// DONT use LEVEL_SUBPOINTS_TO_IMPROVE directory, use the function GetLevelSubPointsToImprove()
INT32 GetLevelSubPointsToImprove();

#define WORKIMPROVERATE  2      // increase to make working  mercs improve more
#define TRAINIMPROVERATE 2      // increase to make training mercs improve more

#define SALARY_CHANGE_PER_LEVEL 1.25		// Mercs salary is multiplied by this
#define MAX_DAILY_SALARY        30000		// must fit into an INT16 (32k)
#define MAX_LARGE_SALARY				500000	// no limit, really

// training cap: you can't train any stat/skill beyond this value
#define TRAINING_RATING_CAP	85

// stat change causes
#define FROM_SUCCESS		0
#define FROM_TRAINING		1
#define FROM_FAILURE		2

// types of experience bonus awards
enum
{
	EXP_BONUS_MINIMUM,
	EXP_BONUS_SMALL,
	EXP_BONUS_AVERAGE,
	EXP_BONUS_LARGE,
	EXP_BONUS_MAXIMUM,
	NUM_EXP_BONUS_TYPES,
};


void StatChange(SOLDIERTYPE *pSoldier, UINT8 ubStat, UINT16 usNumChances, UINT8 ubReason);
void ProfileStatChange(MERCPROFILESTRUCT *pProfile, UINT8 ubStat, UINT16 usNumChances, UINT8 ubReason);
void ProcessStatChange(MERCPROFILESTRUCT *pProfile, UINT8 ubStat, UINT16 usNumChances, UINT8 ubReason);

void UpdateStats( SOLDIERTYPE *pSoldier );
void ProfileUpdateStats( MERCPROFILESTRUCT *pProfile );
void ProcessUpdateStats( MERCPROFILESTRUCT *pProfile, SOLDIERTYPE *pSoldier );

UINT32 CalcNewSalary(UINT32 uiOldSalary, BOOLEAN fIncrease, UINT32 uiMaxLimit);
UINT32 RoundOffSalary(UINT32 uiSalary);
UINT16 SubpointsPerPoint(UINT8 ubStat, INT8 bExpLevel);

void HandleUnhiredMercImprovement( MERCPROFILESTRUCT *pProfile );
void HandleUnhiredMercDeaths( INT32 iProfileID );

UINT8 CurrentPlayerProgressPercentage(void);
UINT8 HighestPlayerProgressPercentage(void);

void HourlyProgressUpdate(void);

void HandleAnyStatChangesAfterAttack( void );

void AwardExperienceBonusToActiveSquad( UINT8 ubExpBonusType );

void BuildStatChangeString( STR16 wString, STR16 wName, BOOLEAN fIncrease, INT16 sPtsChanged, UINT8 ubStat );

//Ja25: void MERCMercWentUpALevelSendEmail( UINT8 ubMercMercIdValue );



#endif