#include"CLKCTL.h"

void SYSCLK_Init(void)
{
    //starts with PIOS 16MHz
    HX(SYSCTL_RCC) &= ~0x01; //Enable MOSC
    while (!(HX(SYSCTL_RIS) & (1 << 8))); //Wait for MOSc to get ready
    HX( SYSCTL_RCC ) &= ~(0x03 << 4); //Select MOSC as clock source 16MHz
    MovetoPLL(); //Runs at 80Mhz
}
void MovetoPLL(void)
{
    HX( SYSCTL_RCC ) &= ~(0x1F << 6); // Clear current crystal value
    HX( SYSCTL_RCC ) |= (0x15 << 6); // Configure for a 16 MHz External crystal
    HX( SYSCTL_RCC2 ) = (1 << 31)  //USE RCC2
    | (1 << 30) // DIV400
            | (4 << 22) // Divisor = 5
            | (1 << 14) // USB Power Down
            | (1 << 11); // Bypass the PLL for now, can also be set zero now.
    // Wait for PLL to lock
    while (!(HX(SYSCTL_PLLSTAT) & 1));
    // Wait until PLL has stabilized
    while (!(HX( SYSCTL_RIS) & (1 << 6)));
    HX( SYSCTL_RCC2 ) &= ~(1 << 11); // Switch to PLL OUTPUT,by setting 0 @ 11
}
void SCLK_PROBE_Init(void) //Sets PWM probe for SYSFREQ
{
    BBA(SYSCTL_RCGCPWM,0) = 1;
    BBA(SYSCTL_RCGCGPIO,PORTB) = 1; // System Clock / Enable GPIO Port B

    // Configure PB7 as M0PWM1
    BBA(PortB_DEN,7) = 1;
    BBA(PortB_DIR,7) = 0;
    BBA(PortB_AFSEL,7) = 1;
    HX( PortB_PCTL) &= 0x0FFFFFFF;
    HX( PortB_PCTL) |= 0x40000000;
//---Configure PWM0 to produce a 50% duty cycle signal at System Clock/1000---
    //Generated pwm0B' signal is passed to the M0PWM1 pin.
    BBA(PWM0_ENABLE,1) = 1;
    // Clear and Enable probe signal
    BBA(PWM0_CTL,0) = 1;
    // Generator A not used
    HX(PWM0_GENA) = 0x00000000;
    // Drive probe signal (M0PWM1) high on LOAD, low on crossing CMPA down
    HX(PWM0_GENB) = 0x0000008C;
    // Probe signal is System Clock / 1000
    HX(PWM0_LOAD) = 1000 - 1;
    // Configure a 50% duty cycle
    HX(PWM0_CMPA) = 500 - 1;
    // Comparator B is not used
    HX(PWM0_CMPB) = 0;
}

void SYSTICK_IntHandler(void)
{
    // Generate a PWM tick pulse at PA2.
    BBA(PortA_DATA,2) = 1;    // HIGH at PA2
    BBA(PortA_DATA,2) = 0; //LOW at PA2
    //LOW at PA2
//----------------------------------------------------------------------------
// Set a global flag to indicate that a system tick interval has elapsed.
    Set_Flag(F_SYSTICK);
}
void SYSTICK_Init(void)
{
    // Configure PA2 as an output (TICK signal) by using both HWREG(HX) and BBA method
    //System Clock / Enable GPIO PortA
    BBA(SYSCTL_RCGCGPIO,PORTA ) = 1;// System Clock / Enable GPIO Port A
    BBA(PortA_DEN,2) = 1;
    BBA(PortA_DIR,2) = 1;
//----------------------------------------------------------------------------
    // Configure System Tick
    HX( NVIC_ST_CURRENT) = 0;
    HX( NVIC_ST_RELOAD) = SYSCLK;
    HX( NVIC_ST_CTRL) = 0x07;
}

