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
/* Debug Start                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c

#ifndef DEBUG
#define DEBUG 1 
#endif

#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

/*---------------------------------------------------------------------------------------------------------*/
/* Debug End                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Timer Start                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/

unsigned long timerCount = 0;
void setTime(){
    timerCount++;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Timer End                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* I2C Start                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/


#include <stdbool.h>
#include <stdlib.h>

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
                    memcpy(i2cTempWriteBuffer, i2cWriteData[i2cWriteDataStartPos++], 16);
                   
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
                    memset(i2cWriteData[i2cWriteDataStartPos], 0, 16);
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
            memset(i2cTempReadBuffer, 0, 16);
            readDataLen = 0;
            
            // set up the slave driver buffer receive pointer
            I2C1_WritePointerSet(&i2c1_slaveWriteData);
            break;
        
        /* ??mcu read pi????? */
        case I2C1_SLAVE_RECEIVED_DATA_DETECTED:

            i2cTempReadBuffer[readDataLen++] = i2c1_slaveWriteData;
            
            if(readDataLen == 16){
                memcpy(i2cReadData[i2cReadDataStartPos++], i2cTempReadBuffer, 16);
                memset(i2cTempReadBuffer, 0, 16);
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



/*---------------------------------------------------------------------------------------------------------*/
/* GPIO Start                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

#include <string.h>

/* first pad pressed time
 * p~: pressed down time by timer with ms unit
 */
uint32_t pressedKeyTime[48] = {0};

/* if the second pad is pressed
 * 0: not pressed 
 * 1: only pressed first pad
 * 2: pressed the second pad
 * 3: released the second pad
 */
uint8_t pressedKey[48] = {0};		

uint8_t powerButton = 0;
uint8_t sensitiveButton = 0;
uint8_t sustainButton = 0;
uint8_t lowerOctaceButton = 0;
uint8_t raiseOctaceButton = 0;
uint8_t pauseButton = 0;
uint8_t speedButton = 0;

uint8_t pedalpluggedIn = 0;
uint8_t pedalDown = 0;

/* forward: 0>1>0>1, backward: 0>2>0>2
 * 0:floating state
 * 1:foward state
 * 2:backward state
 */
uint8_t sectionKnobState = 0;

uint8_t speedKnobState = 0;

uint8_t GetInput(uint8_t pinNumber){
    
    switch(pinNumber){
        case 1:
            return IO_RC7_GetValue();
            break;
        case 2:
            return IO_RC6_GetValue();
            break;
        case 3:
            return IO_RC10_GetValue();
            break;
        case 4:
            return IO_RC15_GetValue();
            break;
        case 5:
            return IO_RC14_GetValue();
            break;
        case 6:
            return IO_RC5_GetValue();
            break;
        case 7:
            return IO_RC4_GetValue();
            break;
        case 8:
            return IO_RC3_GetValue();
            break;  
    }
    return 1;
}

void ReadPanel(){
    
    /*
     * RC7  in 1
     * RC6  in 2
     * RC10 in 3
     * RC15 in 4
     * RC14 in 5
     * RC5  in 6
     * RC4  in 7
     * RC3  in 8
     * -
     * RA5  out A
     * RD1  out B
     * RC8  out C
     * RA15 out Enable
     */

    uint8_t hasThisSpeedKnobState = 0;

    /*
     * if this run is after debouce
     */
    uint8_t isAfterDebounce = 0;

    uint8_t pressedNum = 0;

    char tempCommand[16];
    char value[4];
    double velocity = 0;

    long readCount = 0;

    //printf("ReadPanel\n");
    
    for(uint8_t i = 0; i < 16; i++){
        
        IO_RA5_SetValue(i & 0x1);
        IO_RD1_SetValue(i & 0x2);
        IO_RC8_SetValue(i & 0x4);
        IO_RA15_SetValue(i & 0x8);
        
        for(uint8_t j = 0; j < 8; j++){

            if(i == 8 || i ==10 || i == 12){	// press first pad

                pressedNum = (i-8)/2*8+j;
                if(GetInput(j) == 0){
                    if(pressedKey[pressedNum] == 0){
                        pressedKeyTime[pressedNum] = timerCount;
                        pressedKey[pressedNum] = 1;

                        //printf("read input %s %d\n",tempCommand, i2cWriteDataEndPos);
                        //printf("read input [%d] %d %d at %ds\n", pressedNum, i, j, timerCount);
                    }
                }
                else if(pressedKey[pressedNum] == 2 ||          // means the second pad was pressed
                        pressedKey[pressedNum] == 3){           // means after release second pad
                    //printf("release [%d] - %d %d at %ds\n", pressedNum, i, j, timerCount);
                    memset(tempCommand, 0x0, 16);
                    sprintf(value, "%03d", 127 + 23 - pressedNum);
                    strncpy(tempCommand, value, 3);
                    strncat(tempCommand, ",-1", 3);
                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                    i2cWriteDataEndPos++;
                    if(i2cWriteDataEndPos == 16)
                            i2cWriteDataEndPos = 0;

                    //printf("%s\n", tempCommand);

                    pressedKey[pressedNum] = 0;
                }
                else if(pressedKey[pressedNum] == 1){	// means on pressed first pad then release
                    pressedKeyTime[pressedNum] = 0;
                    pressedKey[pressedNum] = 0;
                    //printf("release [%d]\n", pressedNum);
                }

            }
            else if(i == 9 || i ==11 || i == 13){	// press second pad
                if(GetInput(j) == 0){
                    pressedNum = (i-8)/2*8+j;
                    if(pressedKey[pressedNum] == 1){
                        //printf("read input [%d] %d %d at %ds\n", pressedNum, i, j, timerCount);
                        velocity = (timerCount - pressedKeyTime[pressedNum]) / 2;
                        pressedKey[pressedNum] = 2;

                        memset(tempCommand, 0x0, 16);
                        sprintf(value, "%03d", 127 + 23 - pressedNum);
                        strncpy(tempCommand, value, 3);
                        strncat(tempCommand, ",", 1);

                        sprintf(value, "%04d", (int)(velocity));
                        strncat(tempCommand, value, 4);
                        memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                        i2cWriteDataEndPos++;
                        if(i2cWriteDataEndPos == 16)
                            i2cWriteDataEndPos = 0;

                        debug_print("%s %s %04d\n",value , tempCommand, (int)velocity);

                    }
                    //printf("read input [%d] %d %d at %ds\n", pressedNum, i, j, timerCount);
                }
            }
#ifdef VK49KEY
            /*		i	j				i	j
             *	24: 3  67			36: 7  23
             *	25:	2  67			37: 6  23
             *	26:	1  67			38: 5  23
             *	27:	0	 67			39: 4  23
             *	28:	7  45			40: 3  23
             *	29:	6  45			41: 2  23
             *	30:	5  45			42: 1  23
             *	31:	4  45			43: 0  23
             *	32:	3  45			44: 7  01
             *	33:	2  45			45: 6  01
             *	34:	1  45			46: 5  01
             *	35:	0  45			47: 4  01
             */
            else if(i == 0 || i == 2 || i == 4 || i == 6){

                pressedNum = (6-i)/2*8+24 + (j<4?(3-j):(7-j-4));
                if(GetPin(0, j) == 0 && pressedNum < 48 && pressedNum > 23){
                    if(pressedKey[pressedNum] == 0){
                        pressedKeyTime[pressedNum] = timerCount;
                        pressedKey[pressedNum] = 1;

                        /*
                        memset(tempCommand, 0x0, 16);
                        sprintf(value, "%03d", 127 + 23 - pressedNum);
                        strncpy(tempCommand, value, 3);
                        //printf("read input %s \n",tempCommand);
                        strncat(tempCommand, ",", 1);
                        //tempCommand[4] = ',';
                        sprintf(value, "%03d", rand()%128);
                        strncat(tempCommand, value, 3);
                        //printf("read input %s \n",tempCommand);
                        //strncat(tempCommand, "\0", 1);

                        //printf("end char %d \n",tempCommand[7]);

                        memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                        i2cWriteDataEndPos++;
                        if(i2cWriteDataEndPos == 16)
                            i2cWriteDataEndPos = 0;
                        */

                        //printf("read input %s %d\n",tempCommand, i2cWriteDataEndPos);
                        //printf("read input [%d] %d %d at %ds\n", pressedNum, i, j, timerCount);

                    }
                }
                else if((pressedKey[pressedNum] == 2 || // means the second pad was pressed
                        pressedKey[pressedNum] == 3) && pressedNum < 48 && pressedNum > 23){  // means after release second pad
                    //printf("release [%d] - %d %d at %ds\n", pressedNum, i, j, timerCount);

                    memset(tempCommand, 0x0, 16);
                    sprintf(value, "%03d", 127 + pressedNum);
                    strncpy(tempCommand, value, 3);
                    strncat(tempCommand, ",-1", 3);
                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                    i2cWriteDataEndPos++;
                    if(i2cWriteDataEndPos == 16)
                           i2cWriteDataEndPos = 0;


                    //printf("%s\n", tempCommand);

                    pressedKey[pressedNum] = 0;
                }
                else if(pressedKey[pressedNum] == 1 && pressedNum < 48 && pressedNum > 23){	// means on pressed first pad then release
                        pressedKeyTime[pressedNum] = 0;
                        pressedKey[pressedNum] = 0;
                        //printf("release [%d]\n", pressedNum);
                }


            }
            else if(i == 1 || i == 3 || i == 5 || i == 7){	// press second pad
                if(GetPin(0, j) == 0){
                    pressedNum = (7-i)/2*8+24 + (j<4?(3-j):(7-j-4));
                    if(pressedKey[pressedNum] == 1 && pressedNum < 48 && pressedNum > 23){
                        //printf("read input [%d] %d %d at %ds\n", pressedNum, i, j, timerCount);
                        velocity = (timerCount - pressedKeyTime[pressedNum]);
                        pressedKey[pressedNum] = 2;

                        memset(tempCommand, 0x0, 16);
                        sprintf(value, "%03d", 127 + pressedNum);
                        strncpy(tempCommand, value, 3);
                        strncat(tempCommand, ",", 1);
                        sprintf(value, "%04d", (int)(velocity));
                        strncat(tempCommand, value, 4);
                        memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                        i2cWriteDataEndPos++;
                        if(i2cWriteDataEndPos == 16)
                            i2cWriteDataEndPos = 0;

                        printf("%s %s %04d\n",value , tempCommand, (int)velocity);

                    }
                    //printf("read input [%d] %d %d at %ds\n", pressedNum, i, j, timerCount);
                }
            }
#endif
            else{

                if(GetInput(j) == 0){
                    if(i ==14 && (j == 4 || j == 5)){

                        switch(sectionKnobState){
                        case 0:
                            if(GetInput(4) == 0 && GetInput(5) != 0){
                                sectionKnobState = 1;
                                printf("section knob forward. %d\n", i2cWriteDataEndPos);

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "1020,1");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;

                            }
                            else if(GetInput(5) == 0 && GetInput(4) != 0){
                                sectionKnobState = 2;
                                printf("section knob backward. %d\n", i2cWriteDataEndPos);

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "1020,-1");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        case 1:
                        case 2:
                                if(GetInput(4) == 0 && GetInput(5) == 0){
                                    sectionKnobState = 0;
                                    printf("section knob floating.\n");
                                }
                                break;
                        }
                    }
                    else{
                        if(i == 14){
                            switch(j){
                            case 1:
                                if(sensitiveButton != 1){
                                    sensitiveButton = 1;
                                    printf("Press Sensitive button.\n");

                                    memset(tempCommand, 0x0, 16);
                                    sprintf(tempCommand, "1002,1");
                                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                    i2cWriteDataEndPos++;
                                    if(i2cWriteDataEndPos == 16)
                                            i2cWriteDataEndPos = 0;
                                }
                                break;
                            case 2:
                                if(sustainButton != 1){
                                    sustainButton = 1;
                                    printf("Press Sustain button.\n");

                                    memset(tempCommand, 0x0, 16);
                                    sprintf(tempCommand, "1001,1");
                                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                    i2cWriteDataEndPos++;
                                    if(i2cWriteDataEndPos == 16)
                                        i2cWriteDataEndPos = 0;
                                }
                                break;
                            case 3:
                                if(speedButton != 1){
                                    speedButton = 1;
                                    printf("Press Speed button.\n");

                                    memset(tempCommand, 0x0, 16);
                                    sprintf(tempCommand, "1003,1");
                                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                    i2cWriteDataEndPos++;
                                    if(i2cWriteDataEndPos == 16)
                                        i2cWriteDataEndPos = 0;
                                }
                                break;


                            }

                        }
                        else if(i ==15){
                            switch(j){
                            case 0:
                                if(pauseButton != 1){
                                        pauseButton = 1;
                                        printf("Press Pause button.\n");

                                        memset(tempCommand, 0x0, 16);
                                        sprintf(tempCommand, "1012,1");
                                        memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                        i2cWriteDataEndPos++;
                                        if(i2cWriteDataEndPos == 16)
                                            i2cWriteDataEndPos = 0;
                                }
                                break;
                            case 1:
                                if(raiseOctaceButton != 1){
                                    raiseOctaceButton = 1;
                                    printf("Press Raise octave button.\n");

                                    memset(tempCommand, 0x0, 16);
                                    sprintf(tempCommand, "1010,1");
                                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                    i2cWriteDataEndPos++;
                                    if(i2cWriteDataEndPos == 16)
                                        i2cWriteDataEndPos = 0;
                                }
                                break;
                            case 2:
                                if(lowerOctaceButton != 1){
                                    lowerOctaceButton = 1;
                                    printf("Press Lower octave button.\n");

                                    memset(tempCommand, 0x0, 16);
                                    sprintf(tempCommand, "1011,1");
                                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                    i2cWriteDataEndPos++;
                                    if(i2cWriteDataEndPos == 16)
                                        i2cWriteDataEndPos = 0;
                                }
                                break;
                            case 3:
                                if(pedalDown != 1){
                                    pedalDown = 1;
                                    printf("Pedal Down.\n");

                                    memset(tempCommand, 0x0, 16);
                                    sprintf(tempCommand, "500,1");
                                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                    i2cWriteDataEndPos++;
                                    if(i2cWriteDataEndPos == 16)
                                        i2cWriteDataEndPos = 0;
                                }
                                break;
                                    /*
                            case 4:
                                    break;
                                    if(debounceTimerCount + 500 < timerCount){
                                            isAfterDebounce = 1;
                                            hasThisSpeedKnobState = 1;
                                            debounceTimerCount = timerCount;
                                            if(hasLastSpeedKnobState == 1){
                                                    break;
                                            }

                                            printf("Speed knob forward. %d\n", i2cWriteDataEndPos);
                                            memset(tempCommand, 0x0, 16);
                                          sprintf(tempCommand, "1021,1");
                                          memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                          i2cWriteDataEndPos++;
                                          if(i2cWriteDataEndPos == 16)
                                                i2cWriteDataEndPos = 0;
                                    }
                                    break;
                            case 5:
                                    break;
                                    if(debounceTimerCount + 500 < timerCount){
                                            isAfterDebounce = 1;
                                            hasThisSpeedKnobState = 1;
                                            debounceTimerCount = timerCount;
                                            if(hasLastSpeedKnobState == 1){
                                                    break;
                                            }

                                            printf("Speed knob backward. %d\n", i2cWriteDataEndPos);
                                            memset(tempCommand, 0x0, 16);
                                            sprintf(tempCommand, "1021,-1");
                                            memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                            i2cWriteDataEndPos++;
                                            if(i2cWriteDataEndPos == 16)
                                                    i2cWriteDataEndPos = 0;
                                    }
                                    break;
                                    */
                            }
                            if(i ==15 && (j == 4 || j == 5)){
                                /*
                                // 0 (no) -> 1 (4) -> 2 (5) -> 3 (4off) ->4 (5off)
                                switch(speedKnobState){
                                case 0:
                                        if(GetPin(0, 4) != 0 && GetPin(0, 5) == 0){
                                                speedKnobState = 1;
                                                //printf("speed knob forward. %d\n", i2cWriteDataEndPos);
                                                printf("state 1\n");
                                        }
                                        break;
                                case 1:
                                        if(GetPin(0, 4) != 0 && GetPin(0, 5) != 0){
                                                speedKnobState = 2;
                                                //printf("speed knob forward. %d\n", i2cWriteDataEndPos);
                                                printf("state 2\n");

                                        }
                                        else if(GetPin(0, 4) == 0)
                                                speedKnobState = 0;
                                        break;

                                case 2:
                                        if(GetPin(0, 4) == 0 && GetPin(0, 5) != 0){
                                                speedKnobState = 3;
                                                printf("state 3\n");
                                        }
                                        else if(GetPin(0, 5) == 0)
                                                speedKnobState = 0;
                                        break;
                                case 3:
                                        if(GetPin(0, 4) == 0 && GetPin(0, 5) == 0){
                                                speedKnobState = 4;
                                                printf("speed knob forward.\n");
                                                speedKnobState = 0;
                                        }
                                        else if(GetPin(0, 4) != 0)
                                                speedKnobState = 0;

                                        break;
                                }
                                */

                                switch(speedKnobState){
                                case 0:
                                    if(GetInput(4) == 0 && GetInput(5) != 0){
                                        speedKnobState = 1;
                                        printf("speed knob forward. %d\n", i2cWriteDataEndPos);

                                        memset(tempCommand, 0x0, 16);
                                        sprintf(tempCommand, "1020,1");
                                        memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                        i2cWriteDataEndPos++;
                                        if(i2cWriteDataEndPos == 16)
                                            i2cWriteDataEndPos = 0;

                                    }
                                    else if(GetInput(5) == 0 && GetInput(4) != 0){
                                        speedKnobState = 2;
                                        printf("speed knob backward. %d\n", i2cWriteDataEndPos);

                                        memset(tempCommand, 0x0, 16);
                                        sprintf(tempCommand, "1020,-1");
                                        memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                        i2cWriteDataEndPos++;
                                        if(i2cWriteDataEndPos == 16)
                                            i2cWriteDataEndPos = 0;
                                    }
                                    break;
                                case 1:
                                case 2:
                                    if(GetInput(4) == 0 && GetInput(5) == 0){
                                        speedKnobState = 0;
                                        printf("speed knob floating.\n");
                                    }
                                    break;
                                }
                            }
                        }


                        //printf("read input i:%d j:%d at %ds\n",i, j, timerCount);
                    }
                    // lower octave: i=15, j=2
                    // raise octave: i=15, j=1
                    // left button(velocity): i=14, j=2
                    // right button(sustain): i=14, j=1
                    // pause: i=15, j=0
                    // speed button: i=14, j=3
                    // speed forward(clockwise): i=15, j=4
                    // speed backward(counterclockwise): i=15, j=5
                    // section forward(clockwise): i=14, j=4&5 (->) i=14, j=4 (->) i=14, j=4&5
                    // section backward(counterclockwise): i=14, j=4&5 (->) i=14, j=5 (->) i=14, j=4&5
                    // pedal: i=15, j=3
                }
                else{
                    if(i == 14){
                        switch(j){
                        case 1:
                            if(sensitiveButton == 1){
                                sensitiveButton = 0;
                                printf("Release Sensitive button.\n");

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "1002,0");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        case 2:
                            if(sustainButton == 1){
                                sustainButton = 0;
                                printf("Release Sustain button.\n");

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "1001,0");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        case 3:
                            if(speedButton == 1){
                                speedButton = 0;
                                printf("Release Speed button.\n");

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "1003,0");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        }
                    }
                    else if(i ==15){
                        switch(j){
                        case 0:
                            if(pauseButton == 1){
                                pauseButton = 0;
                                printf("Release Pause button.\n");

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "1012,0");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        case 1:
                            if(raiseOctaceButton == 1){
                                raiseOctaceButton = 0;
                                printf("Release Raise octave button.\n");

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "1010,0");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        case 2:
                            if(lowerOctaceButton == 1){
                                lowerOctaceButton = 0;
                                printf("Release Lower octave button.\n");

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "1011,0");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        case 3:
                            if(pedalDown == 1){
                                pedalDown = 0;
                                printf("Pedal Up.\n");

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "500,0");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        case 4:
                            // avoid continuous signal
                                    //printf("Remove Speed knob forwarded state.\n");
                            break;
                        case 5:
                            // avoid continuous signal
                                    //printf("Remove Speed knob backwarded state.\n");
                            break;
                        }
                    }
                }
            }
        }
    }

    if(IO_RA7_GetValue() == 1){
        if(powerButton == 0){
            powerButton = 1;
            printf("Press power button.\n");

            memset(tempCommand, 0x0, 16);
            sprintf(tempCommand, "1000,1");
            memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
            i2cWriteDataEndPos++;
            if(i2cWriteDataEndPos == 16)
                i2cWriteDataEndPos = 0;
        }
    }
    else{
        if(powerButton == 1){
            powerButton = 0;
            printf("Release power button.\n");

            memset(tempCommand, 0x0, 16);
            sprintf(tempCommand, "1000,0");
            memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
            i2cWriteDataEndPos++;
            if(i2cWriteDataEndPos == 16)
                i2cWriteDataEndPos = 0;
        }
    }

    if(IO_RA8_GetValue() == 1){
        if(pedalpluggedIn == 0){
            pedalpluggedIn = 1;
            printf("Pedal plugged in.\n");

            memset(tempCommand, 0x0, 16);
            sprintf(tempCommand, "1502,1");
            memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
            i2cWriteDataEndPos++;
            if(i2cWriteDataEndPos == 16)
                i2cWriteDataEndPos = 0;
        }
    }
    else{
        if(pedalpluggedIn == 1){
            pedalpluggedIn = 0;
            printf("Pedal plugged out.\n");

            memset(tempCommand, 0x0, 16);
            sprintf(tempCommand, "1502,0");
            memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
            i2cWriteDataEndPos++;
            if(i2cWriteDataEndPos == 16)
                i2cWriteDataEndPos = 0;
        }
    }

    //printf("power:%d pedal:%d\n", P40, P13);

}


/*---------------------------------------------------------------------------------------------------------*/
/* GPIO End                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* SPI Start                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/


uint8_t indicatorLights[16] = {0};

// https://github.com/microchip-pic-avr-examples/pic18f47q10-cnano-spi-slave-receive-fs/blob/54b82942d9a8ed549d34c55be96780cba1e1cc4d/pic18f47q10-cnano-spi-slave-receive-fs.X/mcc_generated_files/spi_slave.c
void SetOutput(){
    spi1_open(SPI1_DEFAULT);
    
    spi1_writeByte(indicatorLights[1]);
    spi1_writeByte(indicatorLights[0]);
    
    spi1_close();
}

/*
 * Light number: 
 * -1: turn off all lights
 * 0: power
 * 1: sentivity
 * 2: sustain
 * 3: raise octave
 *    *13
 * *8    *12
 * *9    *11
 *    *10
 * 14: lower octave
 */
void SetIndicatorLights(int lightNumber, int on){
		
    // 0: power
    // 1: sentivity
    // 2: sustain
    // 3: raise octave
    //    *13
    // *8    *12
    // *9    *11
    //    *10
    // 14: lower octave

    uint8_t i, j;

    if(lightNumber > 15)
        return ;

    if(lightNumber < 0){
        for(i = 0; i < 16; i++){
            indicatorLights[i] = 0;
        }
    }
    else{

        if(on == 1)
            indicatorLights[lightNumber] = 1;
        else if(on == 0)
            indicatorLights[lightNumber] = 0;
    }

    SetOutput();
    
}

/* positive speed	
 *    *0
 * *     *1
 * *4    *2
 *    *3
 * negative speed
 *    *0
 * *-1   *
 * *-2   *-4
 *    *-3
 *
 * 5: reset
 */
void SetSpeedKnobLightRing(int lightNumber){
    //    *13
    // *8    *12
    // *9    *11
    //    *10

    uint8_t i, j;

    if(lightNumber > 5 || lightNumber < -4)
        return ;

    // reset light ring
    for(i = 8; i < 14; i++)
        indicatorLights[i] = 0;

    if(lightNumber < 0){
        indicatorLights[13] = 1;
        for(i = 8; i < 8 - lightNumber; i++){
            indicatorLights[i] = 1;
        }
    }
    else if(lightNumber > 0 && lightNumber < 5){
        indicatorLights[13] = 1;
        for(i = 12; i > 12 - lightNumber; i--){
            indicatorLights[i] = 1;
        }
    }
    else if(lightNumber == 0){
        indicatorLights[13] = 1;
    }

    SetOutput();
}

/*---------------------------------------------------------------------------------------------------------*/
/* SPI End                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* PWM Start                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/


/* 
 * 0: turn off
 * 1: revolve around
 * 2: pause revolve around
 * 3: fading
 * 4: pause fading
 */
uint8_t pwmLightEffectType;

/*
 * unit: ms
 */
uint16_t pwmTimeLength;
uint16_t pwmTimeLengthLeft;

/*
* position: 0~7, -1:turn 0ff
 */
void SetPwmLightRingRevolving(int position){
	
    //				*B3
    //		*A0			*B2
    //	*A1					*B1
    //		*A2			*B0
    //				*A3

    uint16_t brightnessArray[8] = {0};
    uint16_t i;

    for(i = 0;i < 8; i++){
        if(i > position)
            brightnessArray[i] = 0x0;
        else if(i == position)
            brightnessArray[i] = 0xFFF;
        else
            brightnessArray[i] = 0x100;
    }
    
    MCCP1_COMPARE_DualEdgeBufferedConfig(0, brightnessArray[7]);
    MCCP2_COMPARE_DualEdgeBufferedConfig(0, brightnessArray[6]);
    MCCP3_COMPARE_DualEdgeBufferedConfig(0, brightnessArray[5]);
    SCCP4_COMPARE_DualEdgeBufferedConfig(0, brightnessArray[4]);
    SCCP5_COMPARE_DualEdgeBufferedConfig(0, brightnessArray[3]);
    SCCP6_COMPARE_DualEdgeBufferedConfig(0, brightnessArray[2]);
    SCCP7_COMPARE_DualEdgeBufferedConfig(0, brightnessArray[1]);
    SCCP8_COMPARE_DualEdgeBufferedConfig(0, brightnessArray[0]);
}

/*
 * brightness:
 * 1>2>4>8>16>32>64>128>256>512>1024>2048
 */
void SetPwmLightRingFading(double brightness){
	
    uint16_t brightnessConverted;

    if(brightness > 11)
            return;

    //brightnessConverted = 0x1 << brightness / 2;
    brightnessConverted = pow(2, brightness);

    //printf("brightness %f %d\n",brightness , brightnessConverted);
    
    MCCP1_COMPARE_DualEdgeBufferedConfig(0, brightnessConverted);
    MCCP2_COMPARE_DualEdgeBufferedConfig(0, brightnessConverted);
    MCCP3_COMPARE_DualEdgeBufferedConfig(0, brightnessConverted);
    SCCP4_COMPARE_DualEdgeBufferedConfig(0, brightnessConverted);
    SCCP5_COMPARE_DualEdgeBufferedConfig(0, brightnessConverted);
    SCCP6_COMPARE_DualEdgeBufferedConfig(0, brightnessConverted);
    SCCP7_COMPARE_DualEdgeBufferedConfig(0, brightnessConverted);
    SCCP8_COMPARE_DualEdgeBufferedConfig(0, brightnessConverted);
}

/*
 * Effect type: 
 * 0: turn off
 * 1: revolve around
 * 2: fading
 * 3: pause(<0) or resume(>0) effect
 */
void SetLightRingEffect(int effectType, float timeLength){
    switch(effectType){
        case 0:
            pwmLightEffectType = 0;
            break;
        case 1:
            pwmLightEffectType = 1;
            pwmTimeLength = timeLength * 1000;
            pwmTimeLengthLeft = pwmTimeLength;
            printf("pwm light effect 1 with time %d ms\n", pwmTimeLengthLeft);
            break;
        case 2:
            pwmLightEffectType = 3;
            pwmTimeLength = timeLength * 1000;
            pwmTimeLengthLeft = pwmTimeLength;
            break;
        case 3:
            if(pwmLightEffectType == 1 && timeLength < 0){	// pause revolve
                    pwmLightEffectType = 2;
            }
            else if(pwmLightEffectType == 2 && timeLength > 0){	// resume revolve
                    pwmLightEffectType = 1;
            }
            else if(pwmLightEffectType == 3 && timeLength < 0){	// pause fading
                    pwmLightEffectType = 4;
            }
            else if(pwmLightEffectType == 4 && timeLength > 0){	// resume fading
                    pwmLightEffectType = 3;
            }
            break;
    }
}

void UpdateLightRingEffect(uint16_t elapsedFrameTime){
		
    float brightness;
    int position;

    if(pwmLightEffectType == 0){
        SetPwmLightRingRevolving(0);
        return;
    }

    if(pwmLightEffectType == 2 || pwmLightEffectType == 4)
        return;

    if(pwmLightEffectType == 1){

        if(elapsedFrameTime < pwmTimeLengthLeft)
            pwmTimeLengthLeft -= elapsedFrameTime;
        else{
            pwmLightEffectType = 0;
            SetPwmLightRingRevolving(-1);
            return;
        }

        /* position = round(time left / total time * 8) */
        position = 8 - (int)(((float)pwmTimeLengthLeft) * 8.f / (float)pwmTimeLength + 1.0f);
        //printf("test float %f , %f , %f, %f\n", (float)pwmTimeLengthLeft ,((float)pwmTimeLengthLeft) * 8.f, ((float)pwmTimeLengthLeft) * 8.f / (float)pwmTimeLength, round(((float)pwmTimeLengthLeft) * 8.f / (float)pwmTimeLength));
        //printf("position %d, pwm time left %d\n", position, pwmTimeLengthLeft);
        if(position < 8){
            SetPwmLightRingRevolving(position);
        }
        else if(position == 8){
            SetPwmLightRingRevolving(7);
        }
    }
    else if(pwmLightEffectType == 3){

        pwmTimeLengthLeft -= elapsedFrameTime;

        if(pwmTimeLengthLeft < pwmTimeLength / 2){	// former half - lighting up
            brightness = (float)pwmTimeLengthLeft / (float)(pwmTimeLength / 2) * 12.f;
        }
        else if(pwmTimeLengthLeft < pwmTimeLength){	// latter half - darken down
            brightness = (float)(pwmTimeLength - pwmTimeLengthLeft) / (float)(pwmTimeLength / 2) * 12.f;
        }
        else{																				// restart a run
            pwmTimeLengthLeft = pwmTimeLength;
            return;
        }

        if(brightness < 12){
            SetPwmLightRingFading(brightness);
        }
        else if(brightness >= 12){
            SetPwmLightRingFading(12);
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* PWM End                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* ADC Start                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

/*
 * 0 -> 1 -> 2 -> 0
 * 0: no state
 * 1: a1 reading
 * 2: a2 reading
 */
uint8_t adcState = 0;

uint8_t an9 = 0;
uint8_t an10 = 0;

/*
 * equal to timerCount / 256
 */
uint32_t adcTimerCount = 0;

void ProcessAdc(){
    
    switch(adcState){
        case 0:
            if(timerCount << 2 > adcTimerCount + 3){    // trigger every 256 * 4 ticks, which is 0.0256s * 4
                adcTimerCount = timerCount << 2;
                ADC1_ChannelSelect(channel_AN9);
                ADC1_SoftwareTriggerEnable();
                adcState = 1;
            }
            break;
        case 1:
            ADC1_SoftwareTriggerDisable();
            uint8_t tempAn9 = ADC1_ConversionResultGet(channel_AN9);
            if(tempAn9 != an9){
                
            }
                
            ADC1_ChannelSelect(channel_AN10);
            ADC1_SoftwareTriggerEnable();
            adcState = 2;
            break;
        case 2:
            ADC1_SoftwareTriggerDisable();
            uint8_t tempAn10 = ADC1_ConversionResultGet(channel_AN10);
            if(tempAn10 != an10){
                
            }
            adcState = 0;
    }
    
}

/*---------------------------------------------------------------------------------------------------------*/
/* ADC End                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Decode Start                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/


void DecodeMessage(char* message){
	
    // https://medium.com/andy%E7%9A%84%E8%B6%A3%E5%91%B3%E7%A8%8B%E5%BC%8F%E7%B7%B4%E5%8A%9F%E5%9D%8A/c%E8%AA%9E%E8%A8%80-03-%E7%A7%92%E6%87%82%E5%AD%97%E4%B8%B2%E8%99%95%E7%90%86%E5%87%BD%E6%95%B8-%E4%B8%8B-824253379483

    char* pointer;
    float value1, value2;

    pointer = strtok(message, ",");
    if(pointer == NULL)
        return;

    if(strcmp(pointer, "RV") == 0){			// RV - Revolve around light ring
        pointer = strtok(NULL, ",");
        value1 = atof(pointer);
        if(value1 > 0)
            SetLightRingEffect(1, value1);
        else if(value1 == 0)
            SetLightRingEffect(2, value1);	// pause revolving ring
        else
            SetLightRingEffect(0, value1);	// turn off light ring
    }
    else if(strcmp(pointer, "BT") == 0){// BT - bluetooth light ring
        pointer = strtok(NULL, ",");
        value1 = atof(pointer);
        if(value1 > 0)
            SetLightRingEffect(2, value1);
        else
            SetLightRingEffect(0, value1);	// turn off
    }
    else if(strcmp(pointer, "PS") == 0){	//PS - pause light ring
        pointer = strtok(NULL, ",");
        value1 = atof(pointer);

        SetLightRingEffect(3, value1);		// pause or resume
    }
    else if(strcmp(pointer, "IR") == 0){	//IR - indicator light
        pointer = strtok(NULL, ",");
        value1 = atoi(pointer);

        if(pointer != NULL)
            pointer = strtok(NULL, ",");
        else
            return;

        value2 = atoi(pointer);

        SetIndicatorLights(value1, value2);		// choose light to turn on or off
    }
    else if(strcmp(pointer, "SK") == 0){	//SK - Speed knob light ring
        pointer = strtok(NULL, ",");
        value1 = atoi(pointer);

        SetSpeedKnobLightRing(value1);		// set speed light ring or reset
    }
    else if(strcmp(pointer, "RS") == 0){	//RS - Reset all lights
        pointer = strtok(NULL, ",");
        value1 = atoi(pointer);

        SetLightRingEffect(0, 0);		// Turn off section light ring
        SetIndicatorLights(-1, 0);
    }
}

void ProcessI2cMessage(){
	
    char newCommand[16];

    if(i2cReadDataStartPos != i2cReadDataEndPos){

        memset(newCommand, 0x0, 16);
        memcpy(newCommand, i2cReadData[i2cReadDataStartPos], 16);

        i2cReadDataStartPos++;

        if(i2cReadDataStartPos == 16)
            i2cReadDataStartPos = 0;	

        printf("%s | %d \n", newCommand, timerCount);

        DecodeMessage(newCommand);

    }
}



/*---------------------------------------------------------------------------------------------------------*/
/* Decode End                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR1_SetInterruptHandler(&setTime);
    ADC1_Enable();
    
    uint32_t lastFrameTimerCount = 0;
    
    while (1)
    {
        // Add your application code
        ReadPanel();
        ProcessAdc();
        
        /* 50 fps / update frame every 20 ms */
        if(timerCount > lastFrameTimerCount + 200 || timerCount < lastFrameTimerCount){


                if(timerCount > lastFrameTimerCount)
                        UpdateLightRingEffect((timerCount - lastFrameTimerCount) / 10);
                lastFrameTimerCount = timerCount;

                ProcessI2cMessage();
        }
    }
    return 1; 
}
/**
 End of File
*/

