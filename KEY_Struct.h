//----------------------------------------------------------------------------
// INCLUSION LOCK
//----------------------------------------------------------------------------
#ifndef KEY_STRUCT_H_
#define KEY_STRUCT_H_
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------
#include"GLOBAL.h"
/*
 typedef struct
 {
 uint16_t num;
 char name[30];
 uint16_t depth[4];
 }KeyInfo;
 */
typedef struct
{
    uint16_t CardNumber;
    char CDescription[50]; //char CDescription*
    uint8_t Flat;
    uint16_t BladeWidth;
    uint8_t Totalaxis;
    char Jawside;
    char ReverseBitting;
    uint8_t CutSpeed;
    uint16_t CodeMax;
    char HPC1200[6];
    uint16_t ITL;
    uint16_t Depth[10];
    uint16_t Space[10];
} KeyInfo;

#endif /* KEY_STRUCT_H_ */
