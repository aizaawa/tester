//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------
#include"LED.h"

void LED_Init(void)
{
    // Configure PF1,2,3 as an output, they're R,B,G LEDs.
    //System Clock / Enable GPIO PortF
    BBA(SYSCTL_RCGCGPIO,PORTF) = 1;    // System Clock / Enable GPIO Port F
    HX(PortF_DEN) |= 0xE;
    HX(PortF_DIR) |= 0xE;
}

void LED_FSM_Init(FSM *pFSM, uint8_t state, uint16_t timer)
{
  if (pFSM)
    {
        pFSM->state = state;
        pFSM->time = timer;
    }
}
void LED_FSM(FSM *pFSM)
{
    switch (pFSM->state)
    {
    case sP1:
        LED(Red, 1);
        if (!--pFSM->time)
        {
            pFSM->time = tHBON*3;
            pFSM->state = sHBON;
        }
        break;
    case sHBON:
        LED(Red, 0);
        if (!--pFSM->time)
        {
            pFSM->time = tHBON;
            pFSM->state = sP1;
        }
        break;
    }
}
