/*******************************************************************************
* File Name: main.c
*
* Version: 1.30
*
* Description:
*  The project explains the usage of CapSense CSD component. The 2 buttons and
*  linear sliders are used as sensing elements. LED displays buttons active
*  state and slider position is shown on LCD.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include "project.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void CapSense_DisplayState(void);
uint32_t bright = 0;
uint16_t Button[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t Status[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t myData[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
char8 DataStr[13][6];

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  Main function performs following functions:
*   1. Enable global interrupts.
*   2. Initialize CapSense CSD and Start the sensor scanning loop.
*   3. Process scanning results and display it on with LEDs.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
int main()
{
    /* Enable global interrupts */
    CyGlobalIntEnable;

    /* Start PWM and CapSense components */
    LED_CONTROL_Start();
    
    for(int i=0; i<13; i++){
        CapSense_CSD_SetScanResolution(i,CapSense_CSD_RESOLUTION_14_BITS);
    }
    CapSense_CSD_Start();
    SW_Tx_UART_1_Start();

    /* Initialize baselines */
    CapSense_CSD_InitializeAllBaselines();
    
    
    
    while(1u)
    {
        /* Check whether the scanning of all enabled widgets is completed. */
        if(0u == CapSense_CSD_IsBusy())
        {
            /* Update all baselines */
            CapSense_CSD_UpdateEnabledBaselines();
            
       		/* Start scanning all enabled sensors */
        	CapSense_CSD_ScanEnabledWidgets();

            /* Start scanning all enabled sensors */
            for(int i=0; i<13; i++){
                Button[i] = CapSense_CSD_CheckIsWidgetActive(i);
                myData[i] = CapSense_CSD_GetDiffCountData(i);
                //myData[i] = Button[i];

                if(Button[i]==1){
                    bright = 65535;
                    if(Status[i]==0){
                        Status[i]=1;
                        
                    }
                }else{
                    if(Status[i]==1){
                        Status[i]=0;
                    }
                }
                itoa(myData[i],DataStr[i],10);
                SW_Tx_UART_1_PutString(DataStr[i]);
                SW_Tx_UART_1_PutChar(',');
                
            }
            SW_Tx_UART_1_PutCRLF();
        }

        /* Display CapSense state using LEDs */
        LED_CONTROL_WriteCompare(65535-bright);
        bright*=0.9999;
        
    }
}



/* [] END OF FILE */
