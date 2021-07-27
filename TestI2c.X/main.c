/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "Meteo_Utility.h"

/*---------------------------------------------------------------------------------------------------------*/
/* I2C Start                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/


#include <stdbool.h>

uint8_t i2cWriteData[16][16];		// mcu write pi
uint8_t i2cReadData[16][16];		// mcu read pi

/*
 * start - ????
 * end - ????
 * ????end????????end++???end?????start(??16?)??start++?????????
 * ?????start????????Start++???Start==end?????????
 */

volatile uint8_t i2cWriteDataStartPos = 0;
volatile uint8_t i2cWriteDataEndPos = 0;

volatile uint8_t i2cReadDataStartPos = 0;
volatile uint8_t i2cReadDataEndPos = 0;

uint8_t i2cTempWriteBuffer[16] = {0};
uint8_t i2cTempReadBuffer[16] = {0};

volatile uint8_t writeDataLen = 0;
volatile uint8_t readDataLen = 0;

static uint8_t i2c1_slaveWriteData = 0xAA;
#include <stdlib.h>
bool I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS status)
{


    //static uint16_t address, addrByteCount;
    static bool     addressState = false;

    switch (status)
    {
        /* ??mcu write pi??? */
        case I2C1_SLAVE_TRANSMIT_REQUEST_DETECTED:
            // mcu write pi?pi??mcu
            if(writeDataLen == 16){
                if(i2cWriteDataEndPos != i2cWriteDataStartPos){
                    Meteo_Memcpy(i2cTempWriteBuffer, i2cWriteData[i2cWriteDataStartPos++], 16);
                   
                    i2cWriteDataStartPos++;
                    if(i2cWriteDataStartPos == 16)
                        i2cWriteDataStartPos = 0;
                    
                    if(i2cWriteDataStartPos == i2cWriteDataEndPos){
                        i2cWriteDataEndPos++;
                        if(i2cWriteDataEndPos == 16)
                            i2cWriteDataEndPos = 0;
                    }
                }
                else{
                    // ?????????????????????pi?
                    Meteo_Memset(i2cWriteData[i2cWriteDataStartPos], 0, 16);
                }
                writeDataLen = 0;
            }
            
            I2C1_ReadPointerSet(&i2cWriteData[i2cWriteDataStartPos][writeDataLen++]);
            
            break;
        
        /* ????mcu read pi????? */
        case I2C1_SLAVE_RECEIVE_REQUEST_DETECTED:
            // ??mcu write pi????????????mcu read pi?mcu write pi????????????????????mcu write pi???????
            // TODO: ????????i2c routine???
            writeDataLen = 0;
            
            // ?????mcu read pi buffer
            Meteo_Memset(i2cTempReadBuffer, 0, 16);
            readDataLen = 0;
            
            // set up the slave driver buffer receive pointer
            I2C1_WritePointerSet(&i2c1_slaveWriteData);
            break;
        
        /* ??mcu read pi????? */
        case I2C1_SLAVE_RECEIVED_DATA_DETECTED:

            i2cTempReadBuffer[readDataLen++] = i2c1_slaveWriteData;
            
            if(readDataLen == 16){
                Meteo_Memcpy(i2cReadData[i2cReadDataStartPos++], i2cTempReadBuffer, 16);
                Meteo_Memset(i2cTempReadBuffer, 0, 16);
                readDataLen = 0;
                
                if(i2cReadDataStartPos == 16)
                    i2cReadDataStartPos = 0;
                
                if(i2cReadDataStartPos == i2cReadDataEndPos){
                    i2cReadDataEndPos++;
                    if(i2cReadDataEndPos == 16)
                        i2cReadDataEndPos = 0;
                }
            }
            break;

        case I2C1_SLAVE_10BIT_RECEIVE_REQUEST_DETECTED:

            // do something here when 10-bit address is detected

            // 10-bit address is detected

            break;

        default:
            break;

    }

    return true;
}

/*---------------------------------------------------------------------------------------------------------*/
/* I2C End                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/


/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    
    while (1)
    {
        // Add your application code
    }
    return 1; 
}
/**
 End of File
*/

