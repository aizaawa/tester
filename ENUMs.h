
#ifndef ENUMS_H_
#define ENUMS_H_

//----------------------------------------------------------------------------
//For GPIO Ports
enum Ports
{
    PORTA = 0, PORTB, PORTC, PORTD, PORTE, PORTF
};
//----------------------------------------------------------------------------
//For LED FSM states
enum LED_FSM_STATES
{
    sP1 = 0, sFPON, sFPOFF, sP2, sHBON, sHBOFF
};
//----------------------------------------------------------------------------
//For LED FSM Timings
enum LED_FSM_TIMING
{
    tFPON = 125,
    tHBON = 250,
    tFPOFF = 375,
    tHBOFF = 750,
    tp0 = 1000,
    tP1 = 2000,
    tP2 = 2375
};
//----------------------------------------------------------------------------
//For BBA(PortF_DATA,Color)
enum led_color
{
    Red = 1, Blue, Green
};


#endif /* ENUMS_H_ */
