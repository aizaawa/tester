#ifndef GLOBAL_H_
#define GLOBAL_H_
//----------------------------------------------------------------------------
// STANDARD INCLUDE FILES
//----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//----------------------------------------------------------------------------
// ADDITIONAL INCLUDE FILES
//----------------------------------------------------------------------------
#include"CLKCTL.h"
#include"ENUMs.h"
#include"LED.h"
#include"KEY_Struct.h"
#include"QUEUE.h"
#include"UART.h"
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Determines the number of elements in an array
#define NUM_ELEMENTS( a ) ( sizeof( a ) / sizeof( a[ 0 ] ) )
// Hardware Register Access Macro (HWREG)>>(HX) // Multiple bits setup
#define HX( y )  (*( ( volatile uint32_t* )( y ) ) )
/*BBA-single bit setup, HW has dedicated memory for aliasing.
 Use BBA whenever wants to manipulate single bit in an instance, otherwise
 Use HX() when you want to manipulate multiple bits. BBA can do the same
 but then it's repetitive.
 Bit-Band Alias Macro
 a = address in the bit-band region:
 0x20000000-0x200FFFFF
 0x40000000-0x400FFFFF
 b = bit number:
 0-31
 */
#define BBA( a, b ) ( *( ( volatile uint32_t* )                       \
                      ( ( ( uint32_t )( a ) & 0x60000000 )            \
                      | 0x02000000                                    \
                      | ( ( ( ( uint32_t )( a ) & 0x000FFFFF ) << 5 ) \
                      + ( ( ( uint8_t )( b ) & 0x1F ) << 2 ) ) ) ) )

//----------------------------------------------------------------------------

#define F_SYSTICK      0 //SYSTICK Flag
#define F_UARTRXD      1 //UART received Data Flag
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------
void Init_Flags(void); // Initializes system event flags to zero
void Set_Flag(uint32_t uiSysFlag);
void Clear_Flag(uint32_t uiSysFlag);
bool Check_Flag(uint32_t uiSysFlag);

//----------------------------------------------------------------------------
// NVIC registers (NVIC) E000.E000
//----------------------------------------------------------------------------
#define NVIC_ST_CTRL            0xE000E010  // SysTick Control and Status
#define NVIC_ST_RELOAD          0xE000E014  // SysTick Reload Value Register
#define NVIC_ST_CURRENT         0xE000E018  // SysTick Current Value Register
#define NVIC_EN0                0xE000E100  // Interrupt Set Enable
#define NVIC_DIS0               0xE000E180  // Interrupt Clear Enable
//----------------------------------------------------------------------------
//System Control registers (SYSCTL)
//SYSCTL_BASE:0x400F.E000
//----------------------------------------------------------------------------
#define SYSCTL_RCC      0x400FE060 //RunMode Clock
#define SYSCTL_RIS      0x400FE050 //RawInterrupt Status Reg
#define SYSCTL_RCGCPWM  0x400FE640  // RunMode PWM module Clock M0:M1
#define SYSCTL_RCGCGPIO 0x400FE608  // RunMode GPIO Port Clock A:F
#define SYSCTL_RCC2     0x400FE070  // Run-Mode Clock Configuration 2 for PLL
#define SYSCTL_PLLSTAT  0x400FE168  // PLL Status
#define SYSCTL_RCGCUART         0x400FE618  // UART Run Mode Clock Gating Control
#define SYSCTL_SCGCUART         0x400FE718  // UART Sleep Mode Clock Gating Control
//*****************************************************************************
//GPIO registers (PORTA)
//PORTA: 0x4000.4000
//*****************************************************************************
#define PortA_BASE          0x40004000
#define PortA_DATA          0x400043FC // For BBA
#define PortA_DIR           0x40004400
#define PortA_AFSEL         0x40004420
#define PortA_DR2R          0x40004500
#define PortA_DR4R          0x40004504
#define PortA_DR8R          0x40004508
#define PortA_PUR           0x40004510
#define PortA_PDR           0x40004514
#define PortA_DEN           0x4000451C
#define PortA_LOCK          0x40004520
#define PortA_CR            0x40004524
#define PortA_PCTL          0x4000452C
#define PortA_DATA_S(x) (*((volatile uint32_t*)(PortA_BASE+(x<<2))))|=(x) //No need for HWREG
#define PortA_DATA_C(x) (*((volatile uint32_t*)(PortA_BASE+(x<<2))))&=~(x)
#define PortA_DATA_T(x) (*((volatile uint32_t*)(PortA_BASE+(x<<2))))^=(x)
//*****************************************************************************
//GPIO registers (PORTB)
//PORTB: 0x4000.5000
//*****************************************************************************
#define PortB_BASE         0x40005000
#define PortB_DATA         0x400053FC // For BBA
#define PortB_DIR          0x40005400
#define PortB_AFSEL        0x40005420
#define PortB_DR2R         0x40005500
#define PortB_DR4R         0x40005504
#define PortB_DR8R         0x40005508
#define PortB_PUR          0x40005510
#define PortB_PDR          0x40005514
#define PortB_DEN          0x4000551C
#define PortB_LOCK         0x40005520
#define PortB_CR           0x40005524
#define PortB_PCTL         0x4000552C
#define PortB_DATA_S(x)    (*((volatile uint32_t*)(PortB_BASE+(x<<2))))|=(x)
#define PortB_DATA_C(x)    (*((volatile uint32_t*)(PortB_BASE+(x<<2))))&=~(x)
#define PortB_DATA_T(x)    (*((volatile uint32_t*)(PortB_BASE+(x<<2))))^=(x)
//*****************************************************************************
//GPIO registers (PORTC)
//PORTC: 0x4000.6000
//*****************************************************************************
#define PortC_BASE         0x40006000
#define PortC_DATA         0x400063FC // For BBA
#define PortC_DIR          0x40006400
#define PortC_AFSEL        0x40006420
#define PortC_DR2R         0x40006500
#define PortC_DR4R         0x40006504
#define PortC_DR8R         0x40006508
#define PortC_PUR          0x40006510
#define PortC_PDR          0x40006514
#define PortC_DEN          0x4000651C
#define PortC_LOCK         0x40006520
#define PortC_CR           0x40006524
#define PortC_PCTL         0x4000652C
#define PortC_DATA_R(x) (*((volatile uint32_t*)(PortC_BASE+(x<<2))))
#define PortC_DATA_S(x)    (*((volatile uint32_t*)(PortC_BASE+(x<<2))))|=(x)
#define PortC_DATA_C(x)    (*((volatile uint32_t*)(PortC_BASE+(x<<2))))&=~(x)
#define PortC_DATA_T(x)    (*((volatile uint32_t*)(PortC_BASE+(x<<2))))^=(x)
//*****************************************************************************
//GPIO registers (PORTD)
//PORTB: 0x4000.7000
//*****************************************************************************
#define PortD_BASE         0x40007000
#define PortD_DATA         0x400073FC // For BBA
#define PortD_DIR          0x40007400
#define PortD_GPIOIM       0x40007410
#define PortD_AFSEL        0x40007420
#define PortD_DR2R         0x40007500
#define PortD_DR4R         0x40007504
#define PortD_DR8R         0x40007508
#define PortD_PUR          0x40007510
#define PortD_PDR          0x40007514
#define PortD_DEN          0x4000751C
#define PortD_LOCK         0x40007520
#define PortD_CR           0x40007524
#define PortD_PCTL         0x4000752C
#define PortD_DATA_R(x)    (*((volatile uint32_t*)(PortD_BASE+(x<<2))))
#define PortD_DATA_S(x)    (*((volatile uint32_t*)(PortD_BASE+(x<<2))))|=(x)
#define PortD_DATA_C(x)    (*((volatile uint32_t*)(PortD_BASE+(x<<2))))&=~(x)
#define PortD_DATA_T(x)    (*((volatile uint32_t*)(PortD_BASE+(x<<2))))^=(x)

//*****************************************************************************
// GPIO registers (PORTE)
//PORTE:0x4002.4000
//*****************************************************************************
#define PortE_BASE          0x40024000
#define PortE_DATA          0x400243FC // For BBA
#define PortE_DIR           0x40024400
#define PortE_AFSEL         0x40024420
#define PortE_DR2R          0x40024500
#define PortE_DR4R          0x40024504
#define PortE_DR8R          0x40024508
#define PortE_PUR           0x40024510
#define PortE_PDR           0x40024514
#define PortE_DEN           0x4002451C
#define PortE_LOCK          0x40024520
#define PortE_CR            0x40024524
#define PortE_PCTL          0x4002452C
#define PortE_DATA_R(x) (*((volatile uint32_t*)(PortE_BASE+(x<<2))))
#define PortE_DATA_S(x) (*((volatile uint32_t*)(PortE_BASE+(x<<2))))|=(x) //No need for HWREG
#define PortE_DATA_C(x) (*((volatile uint32_t*)(PortE_BASE+(x<<2))))&=~(x)
#define PortE_DATA_T(x) (*((volatile uint32_t*)(PortE_BASE+(x<<2))))^=(x)
//*****************************************************************************
//GPIO registers (PORTF)
//PORTF: 0x4002.5000
//*****************************************************************************
#define PortF_BASE          0x40025000
#define PortF_DATA          0x400253FC // For BBA
#define PortF_DIR           0x40025400
#define PortF_AFSEL         0x40025420
#define PortF_DR2R          0x40025500
#define PortF_DR4R          0x40025504
#define PortF_DR8R          0x40025508
#define PortF_PUR           0x40025510
#define PortF_PDR           0x40025514
#define PortF_DEN           0x4002551C
#define PortF_LOCK          0x40025520
#define PortF_CR            0x40025524
#define PortF_PCTL          0x4002552C
#define PortF_DATA_S(x) (*((volatile uint32_t*)(PortF_BASE+(x<<2))))|=(x)
#define PortF_DATA_C(x) (*((volatile uint32_t*)(PortF_BASE+(x<<2))))&=~(x)
#define PortF_DATA_T(x) (*((volatile uint32_t*)(PortF_BASE+(x<<2))))^=(x)

//*****************************************************************************
//PWM registers (PWM0)
//PWM0: 0x4002.8000
//*****************************************************************************
#define PWM0_ENABLE           0x40028008 // PWM Output Enable
#define PWM0_CTL              0x40028040// PWM0 Control
#define PWM0_LOAD             0x40028050
#define PWM0_CMPA             0x40028058
#define PWM0_CMPB             0x4002805C
#define PWM0_GENA             0x40028060
#define PWM0_GENB             0x40028064

//*****************************************************************************
#define UART0_BASE           0x4000C000  // UART
#define UART0_DR             0x4000C000  // UART Data
#define UART0_RSR            0x4000C004
#define UART0_ECR            0x4000C004
#define UART0_FR             0x4000C018 // UART Flag
#define UART0_ILPR           0x4000C020
#define UART0_IBRD           0x4000C024 // UART Integer Baud-Rate Divisor
#define UART0_FBRD           0x4000C028 // UART Fractional Baud-Rate Divisor
#define UART0_LCRH           0x4000C02C // UART Line Control
#define UART0_CTL            0x4000C030 // UART Control
#define UART0_IFLS           0x4000C034
#define UART0_IM             0x4000C038 // UART Interrupt Mask
#define UART0_RIS            0x4000C03C // UART Raw Interrupt Status
#define UART0_MIS            0x4000C040 // UART Masked Interrupt Status
#define UART0_ICR            0x4000C044 // UART Interrupt Clear
#define UART0_DMACTL         0x4000C048
#define UART0_9BITADDR       0x4000C0A4
#define UART0_9BITAMAS       0x4000C0A8
#define UART0_PP             0x4000CFC0
#define UART0_CC             0x4000CFC8
#endif /* GLOBAL_H_ */
