
//----------------------------------------------------------------------------
// INCLUSION LOCK
//----------------------------------------------------------------------------
#ifndef CLKCTL_H_
#define CLKCTL_H_
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------
#include"GLOBAL.h"
//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------
#define SYSCLK         80000-1
//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//--------------------SYSCLK--------------------------------------------------
void SYSCLK_Init( void );//Configures the MOSC & MOSCPLL+MOSC Failure monitor
void MovetoPLL(void);//Sub func for Init MOSCPLL
void SCLK_PROBE_Init( void );//Sets PWM probe for SYSFREQ


//--------------------SYSTICK-------------------------------------------------
void SYSTICK_IntHandler(void);//SYSTICK event handler, executed after every tick
void SYSTICK_Init( void );//Sets SYSTICK for timer
void SYSTICK_Delay( uint32_t uiDelay );

#endif /* CLKCTL_H_ */
