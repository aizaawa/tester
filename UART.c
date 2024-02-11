//----------------------------------------------------------------------------
// MODULE DESCRIPTION
//----------------------------------------------------------------------------
//
// Provides code to support UART0 for a serial communication interface.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "UART.h" //This also included QUEUE.h
//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

QUEUE *g_pQueueTransmit;
QUEUE *g_pQueueReceive;

//----------------------------------------------------------------------------
// FUNCTION : UART0_IntHandler( void )
// PURPOSE  : Interrupt handler
//----------------------------------------------------------------------------

void UART0_IntHandler(void)
{
    uint32_t uiIntStatus = HX(UART0_MIS);

    // Check for transmit (FIFO empty) interrupt

    if (BBA(UART0_MIS, 5))
    {
        // Clear interrupt
        BBA(UART0_ICR,5) = 1;

        // Continue transmitting anything left in the queue
        UART_TransmitFromQueue();
    }

    // Check for receive interrupt
    if (BBA(UART0_MIS, 4))
    {
        // Clear interrupt
        BBA(UART0_ICR,4) = 1;

        // Transfer received data to queue
        UART_ReceiveToQueue();

        // Set a global flag to indicate that data was received
        Set_Flag(F_UARTRXD);
    }
    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_ReceiveToQueue( void )
// PURPOSE  : Receives information from UART0 and enqueues it to the queue
//----------------------------------------------------------------------------

void UART_ReceiveToQueue(void)
{
    uint8_t uiData;

    // Read data and queue
    uiData = HX(UART0_DR);
    QUEUE_Enqueue(g_pQueueReceive, uiData);

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_TransmitFromQueue( void )
// PURPOSE  : Dequeues information from the a queue and sends it via UART0
//----------------------------------------------------------------------------

void UART_TransmitFromQueue(void)
{
    uint8_t uiData;

    // If the transmitter holding register is empty, start a transmission
    if (BBA(UART0_FR, 7))
    {
        // Remove byte from queue and transmit
        if (QUEUE_Dequeue(g_pQueueTransmit, &uiData))
        {
            HX(UART0_DR) = uiData;
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_GetChar( uint8_t *puiData )
// PURPOSE  : Gets one character from the receive queue
//----------------------------------------------------------------------------

bool UART_GetChar(uint8_t *puiData)
{
    bool bResult = false;

    UART0_IntDisable();
    {
        bResult = QUEUE_Dequeue(g_pQueueReceive, puiData);
    }
    UART0_IntEnable();

    return bResult;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_PutChar( uint8_t uiData )
// PURPOSE  : Places one character into the transmit queue
//----------------------------------------------------------------------------

bool UART_PutChar(uint8_t uiData)
{
    bool bResult = false;

    UART0_IntDisable();
    {
        if (QUEUE_Enqueue(g_pQueueTransmit, uiData))
        {
            UART_TransmitFromQueue();
            bResult = true;
        }
    }
    UART0_IntEnable();

    return bResult;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_SendMessage( char* sMessage )
// PURPOSE  : Writes a message to the transmit queue
//----------------------------------------------------------------------------

void UART_SendMessage(char *sMessage)
{
    // Enqueue entire message using the UART transmit queue
    while (*sMessage)
    {
        if (UART_PutChar(*(uint8_t*) sMessage))
        {
            sMessage++;
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_SendEncodedMessage( char* sMessage, uint8_t uiKey )
// PURPOSE  : Writes an encoded message the transmit queue
//----------------------------------------------------------------------------

void UART_SendEncodedMessage(char *sMessage, uint8_t uiKey)
{
    while (*sMessage)
    {
        if (UART_PutChar(*(uint8_t*) sMessage ^ uiKey))
        {
            sMessage++;
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_Init( void )
// PURPOSE  : Initializes the UART peripheral
//----------------------------------------------------------------------------

void UART_Init(void)
{
    // UART 0 Run Mode Clock Gating Control
    BBA(SYSCTL_RCGCUART,0) = 1;
    // UART 0 Sleep Mode Clock Gating Control
    BBA( SYSCTL_SCGCUART,0) = 1;

    // GPIO Port A Run Mode Clock Gating Control (Port A)
    BBA(SYSCTL_RCGCGPIO ,PORTA) = 1;

    // GPIO Port A Digital Enable (UART U0Rx/U0Tx = PA0/PA1)
    HX(PortA_DEN) |= 0x03;

    // GPIO Port A Alternate Function Select (UART U0Rx/U0Tx = PA0/PA1)
    HX(PortA_AFSEL) |= 0x03;

    // GPIO Port A Port Control (assign as U0Rx and U0Tx)
    HX(PortA_PCTL) = (1 << 0) | (1 << 4);

    // Initialize control (default)
    HX(UART0_CTL) = 0;

    // Compute the fractional baud rate divisor
    uint32_t uiDivisor = ((( UART_CLOCK * 8) / UART_BAUDRATE) + 1) / 2;

    // Set the baud rate
    HX(UART0_IBRD) = uiDivisor / 64;
    HX(UART0_FBRD) = uiDivisor % 64;

    // Set parity, data length, and number of stop bits
    HX(UART0_LCRH) = (0x03 << 5);

    // Enable the UART in two operations
    // Bit 9: RXE
    // Bit 8: TXE
    // Bit 0: UARTEN
    HX(UART0_CTL) = ((1 << 9) | (1 << 8) | (1 << 0));

    // UART Interrupt Mask
    // Bit 5: TXIM
    // Bit 4: RXIM
    HX(UART0_IM) = ((1 << 5) | (1 << 4));

    // Enable Interrupts for UART0
    UART0_IntEnable();

    // Create a transmit queue
    g_pQueueTransmit = QUEUE_Create(1024);
    //cross check if the heap is actually created
    if (!g_pQueueTransmit)
        LED(Red, 1);
    // Create a receive queue
    g_pQueueReceive = QUEUE_Create(32);
    //cross check if the heap is actually created
    if (!g_pQueueReceive)
        LED(Red, 1);
    return;
}

//----------------------------------------------------------------------------
// END UART.C
//----------------------------------------------------------------------------
