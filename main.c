#include"GLOBAL.h"

FSM R1;
extern KeyInfo KEY[1520];
char sBuffer[80];
void Initiate()
{

    SYSCLK_Init(); //1
    SCLK_PROBE_Init(); //2
    SYSTICK_Init(); //3
    LED_Init(); //4
    LED_FSM_Init(&R1, sP1, tHBON);
    UART_Init();
    UART_SendMessage("\e[2J\e[2H"); // Clear screen and home cursor
    UART_SendMessage("\e(B");  // ASCII Set
    UART_SendMessage("\e[1;97m"); // Styled Attributes
    UART_SendMessage("BAtti guLL\r\n");
}

void main(void)
{
    Initiate();
    while (1)
    {
        asm(" wfi");

        if (Check_Flag(F_SYSTICK))
        {
            Clear_Flag(F_SYSTICK);
            LED_FSM(&R1);
        }

        if (Check_Flag(F_UARTRXD))
        {
            Clear_Flag(F_UARTRXD); // Clear the UART received data flag

            uint8_t uiReceivedChar;
            static uint8_t inp[4] = { 0 }; // Make 'inp' static to retain its value between function calls
            static uint16_t uiUserInput = 0;
            if (UART_GetChar(&uiReceivedChar))
            {
                static uint8_t i = 0;

                if ((uiReceivedChar >= '0') && (uiReceivedChar <= '9'))
                {
                    inp[i++] = uiReceivedChar - '0'; // Convert character to numeric value
                    UART_PutChar(uiReceivedChar);
                    if (i >= 4)
                    {
                        uiUserInput = ((inp[0] * 1000) + (inp[1] * 100)
                                + (inp[2] * 10) + (inp[3] * 1));
                        sprintf(sBuffer, "\r\nINPUT: %u\r\n", uiUserInput);
                        UART_SendMessage(sBuffer);
                        UART_SendMessage("\r\n");
                        i = 0;

                        for (int index = 0; index < NUM_ELEMENTS(KEY); index++)
                        {
                            //We can use both this pointer(->) or[index] to find the location.

                            if (KEY[index].CardNumber == uiUserInput)
                            {
                                UART_SendMessage(KEY[index].CDescription);
                                UART_SendMessage(",");
                                sprintf(sBuffer,"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,",KEY[index].Depth[0],KEY[index].Depth[1],KEY[index].Depth[2],KEY[index].Depth[3],
                                        KEY[index].Depth[4],KEY[index].Depth[5],KEY[index].Depth[6],KEY[index].Depth[7],KEY[index].Depth[8],KEY[index].Depth[9],KEY[index].Depth[10]);
                                UART_SendMessage(sBuffer);
                                sprintf(sBuffer, "%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,",KEY[index].Space[0],KEY[index].Space[1],KEY[index].Space[2],KEY[index].Space[3],
                                        KEY[index].Space[4],KEY[index].Space[5],KEY[index].Space[6],KEY[index].Space[7],KEY[index].Space[8],KEY[index].Space[9],KEY[index].Space[10]);
                                UART_SendMessage(sBuffer);
                                sprintf(sBuffer, "%u,",KEY[index].Flat);
                                UART_SendMessage(sBuffer);
                                sprintf(sBuffer, "%u,",KEY[index].Totalaxis);
                                UART_SendMessage(sBuffer);
                                sprintf(sBuffer, "%u,",KEY[index].BladeWidth);
                                UART_SendMessage(sBuffer);
                                sprintf(sBuffer, "%u,",KEY[index].CutSpeed);
                                UART_SendMessage(sBuffer);
                                sprintf(sBuffer, "%c,",
                                        KEY[index].ReverseBitting);
                                UART_SendMessage(sBuffer);
                                sprintf(sBuffer, "%c",KEY[index].Jawside);
                                UART_SendMessage(sBuffer);
                                UART_SendMessage("\r\n");
                            }

                        }

                    }
                }
            }

        }

    }
}
/*
 { if (KEY[index].num == uiUserInput)
 sprintf(sBuffer, "No: %d\r\n", KEY->num);
 UART_SendMessage(sBuffer);
 sprintf(sBuffer, "NAME: %s\r\n",KEY[index].name);
 UART_SendMessage(sBuffer);
 sprintf(sBuffer, "Depth: %d, %d, %d, %d\r\n", KEY->depth[0], KEY->depth[1], KEY->depth[2], KEY->depth[3]);
 UART_SendMessage(sBuffer);
 UART_SendMessage("\r\n");
 }
 */
