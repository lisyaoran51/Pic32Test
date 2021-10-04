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
#include "mcc_generated_files/mcc.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Debug Start                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c

#ifndef DEBUG
#define DEBUG 0
#endif

//#define debug_print(fmt, ...) \
//            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#define debug_print(f_, ...) if (DEBUG) printf((f_), ##__VA_ARGS__)

/*---------------------------------------------------------------------------------------------------------*/
/* Debug End                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Timer Start                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/

uint32_t timerCount = 0;
unsigned long tempSecond = 0;

unsigned long loopCount = 0;
unsigned long lastLoopCount = 0;

void setTime(){
    timerCount++;
    return;
    
    if(!DEBUG) return;
    if(timerCount >> 10 > tempSecond){
        tempSecond = timerCount >> 10;
        
        debug_print(" [%d] loop at time %d\n", loopCount - lastLoopCount , timerCount);
        lastLoopCount = loopCount;
    }
    return;
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

volatile uint8_t writeDataLen = 16;
volatile uint8_t readDataLen = 0;

// when first time write to pi, we need to give a byte to know if there is data.
// 0: no data
// 0x80: has data for first time
// 0x81: second time write same data
static bool isFirstWrite = true;

// 1: first write
// 2: second write
static uint8_t tempWriteTime = 1;
uint8_t firstByte = 0x0;

static uint8_t i2c1_slaveWriteData = 0xAA;

bool I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS status)
{


    //static uint16_t address, addrByteCount;
    static bool     addressState = false;

    switch (status)
    {
        /* ??mcu write pi??? */
        case I2C1_SLAVE_TRANSMIT_REQUEST_DETECTED:
            
#if 1
            
            if(writeDataLen == 16 && isFirstWrite){
                //debug_print("first write!\n");
                if(i2cWriteDataEndPos != i2cWriteDataStartPos){
                    // mcu write data to pi and there is data. so put 0x80 to the first byte
                    if(tempWriteTime == 1)
                        firstByte = 0x80;   // first time send
                    else if(tempWriteTime == 2)
                        firstByte = 0x81;   // second time confirm
                    I2C1_ReadPointerSet(&firstByte);
                    isFirstWrite = false;
                    break;
                }
                else{   // i2cWriteDataEndPos == i2cWriteDataStartPos
                    // mcu write data to pi but there is no data. so put 0x0 to the first byte
                    firstByte = 0x0;
                    I2C1_ReadPointerSet(&firstByte);
                    isFirstWrite = false;
                    tempWriteTime = 1;
                    break;
                }
            }
            else if(writeDataLen == 16 && !isFirstWrite){
                // mcu write data to pi and there is data. gonna put it twice to confirm. so put 0x81 to the first byte
                
                if(i2cWriteDataEndPos != i2cWriteDataStartPos){

                    memcpy(i2cTempWriteBuffer, i2cWriteData[i2cWriteDataStartPos], 16);
                    
                    if(tempWriteTime == 1){
                        tempWriteTime = 2;
                    }
                    else if (tempWriteTime == 2){
                        i2cWriteDataStartPos++;
                        if(i2cWriteDataStartPos == 16)
                            i2cWriteDataStartPos = 0;
                        tempWriteTime = 1;
                    }

                }
                else{
                    // ?????????????????????pi?
                    memset(i2cTempWriteBuffer, 0, 16);
                }
                writeDataLen = 0;
                
            }
            
            //I2C1_ReadPointerSet(&i2cWriteData[i2cWriteDataStartPos][writeDataLen++]);
            I2C1_ReadPointerSet(&i2cTempWriteBuffer[writeDataLen++]);
            
            // if one line is write over, reset "isFirstWrite" to true
            if(writeDataLen == 16)
                isFirstWrite = true;
            
            break;
            
#else       
            
            // mcu write pi?pi??mcu
            if(writeDataLen == 16){
                if(i2cWriteDataEndPos != i2cWriteDataStartPos){
                    memcpy(i2cTempWriteBuffer, i2cWriteData[i2cWriteDataStartPos], 16);
                   
                    i2cWriteDataStartPos++;
                    if(i2cWriteDataStartPos == 16)
                        i2cWriteDataStartPos = 0;
                    
                }
                else{
                    // ?????????????????????pi?
                    memset(i2cTempWriteBuffer, 0, 16);
                }
                writeDataLen = 0;
            }
            
            I2C1_ReadPointerSet(&i2cTempWriteBuffer[writeDataLen++]);
            
            break;
              
           
        
#endif
            
        /* ????mcu read pi????? */
        case I2C1_SLAVE_RECEIVE_REQUEST_DETECTED:
            // ??mcu write pi????????????mcu read pi?mcu write pi????????????????????mcu write pi???????
            // TODO: ????????i2c routine???
            //writeDataLen = 16;
            //isFirstWrite = true;
            
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
                memcpy(i2cReadData[i2cReadDataEndPos++], i2cTempReadBuffer, 16);
                memset(i2cTempReadBuffer, 0, 16);
                readDataLen = 0;
                
                if(i2cReadDataEndPos == 16)
                    i2cReadDataEndPos = 0;
                
                if(i2cReadDataStartPos == i2cReadDataEndPos){
                    
                    i2cReadDataStartPos++;
                    if(i2cReadDataStartPos == 16)
                        i2cReadDataStartPos = 0;
                    
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

#define VK24KEY_X2
//#define VK49KEY

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
        case 0:
            return IO_RC7_GetValue();
            break;
        case 1:
            return IO_RC6_GetValue();
            break;
        case 2:
            return IO_RC10_GetValue();
            break;
        case 3:
            return IO_RC15_GetValue();
            break;
        case 4:
            return IO_RC14_GetValue();
            break;
        case 5:
            return IO_RC5_GetValue();
            break;
        case 6:
            return IO_RC4_GetValue();
            break;
        case 7:
            return IO_RC3_GetValue();
            break;  
    }
    return 1;
}

void ReadAllDebug(){
    //loopCount++;
    for(uint8_t i = 0; i < 16; i++){
        
        IO_RA5_SetValue(i & 0x1);
        IO_RD1_SetValue(i & 0x2);
        IO_RC8_SetValue(i & 0x4);
        IO_RA15_SetValue(i & 0x8);
        
        for(uint8_t j = 0; j < 8; j++){
            if(GetInput(j) == 0){
                debug_print("read input %d %d at %ds\n", i, j, timerCount);
                //for(int k = 0; k < 1000000; k++);
            }
        }
    }
}

void ReadKeyboard(){
    
    //loopCount++;
    
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
    uint8_t firstPadSequence[6] = {0x0, 0x4, 0x6, 0x2, 0xA, 0x8};

    uint8_t pressedNumber = 0;

    char tempCommand[16];
    char value[4];
    double velocity = 0;

    long readCount = 0;
    
            /*		i       j				i       j
             *	0:  4,5     7           12: 3,2     3
             *	1:	4,5     6			13: 3,2     2
             *	2:	4,5     5			14: 3,2     1
             *	3:	4,5     4			15: 3,2     0
             *	4:	4,5     3			16: 0,1     7
             *	5:	4,5     2			17: 0,1     6
             *	6:	4,5     1			18: 0,1     5
             *	7:	4,5     0			19: 0,1     4
             *	8:  3,2     7			20: 0,1     3
             *	9:	3,2     6			21: 0,1     2
             *	10:	3,2     5			22: 0,1     1
             *	11:	3,2     4			23: 0,1     0
             */
            /*		i       j				i       j
             *	24: 10,11	7           36: 8,9     3
             *	25:	10,11	6			37: 8,9     2
             *	26:	10,11	5			38: 8,9     1
             *	27:	10,11	4			39: 8,9     0
             *	28:	10,11	3			40: 6,7     7
             *	29:	10,11	2			41: 6,7     6
             *	30:	10,11	1			42: 6,7     5
             *	31:	10,11	0			43: 6,7     4
             *	32: 8,9     7			44: 6,7     3
             *	33:	8,9     6			45: 6,7     2
             *	34:	8,9     5			46: 6,7     1
             *	35:	8,9     4			47: 6,7     0
             */
    
    /* first pad */
    IO_RA5_SetValue(false);
    IO_RD1_SetValue(false);
    IO_RC8_SetValue(false);
    IO_RA15_SetValue(false);
    for(uint8_t i = 0; i < 6; i++){
        switch(i){
            case 0:
                //IO_RA15_SetValue(false);    //0000 - 0
                break;
            case 1:
                IO_RC8_SetValue(true);      //0100 - 4
                break;
            case 2:
                IO_RD1_SetValue(true);      //0110 - 6
                break;
            case 3:
                IO_RC8_SetValue(false);     //0010 - 2
                break;
            case 4:
                IO_RA15_SetValue(true);     //1010 - 10
                break;
            case 5:
                IO_RD1_SetValue(false);     //1000 - 8
                break;
        }
        
        for(uint8_t j = 0; j < 8; j++){
            if(firstPadSequence[i] <= 4){
                pressedNumber = 23 - firstPadSequence[i] * 8 / 2 - j;
            }
            else{   // (firstPadSequence[i] > 4)
                pressedNumber = 47 - (firstPadSequence[i] - 6) * 8 / 2 - j;
            }
            
            if(GetInput(j) == 0){
                
                if(pressedKey[pressedNumber] == 0){
                    pressedKeyTime[pressedNumber] = timerCount;
                    pressedKey[pressedNumber] = 1;

                    //printf("read input %s %d\n",tempCommand, i2cWriteDataEndPos);
                    //debug_print("read input [%d] %d %d at %ds\n", pressedNumber, i, j, timerCount);
                }
                
                
                //debug_print("read input %d %d at %ds\n", i, j, timerCount * 10);
            }
            else if(pressedKey[pressedNumber] == 2 ||          // means the second pad was pressed
                    pressedKey[pressedNumber] == 3){           // means after release second pad
                //printf("release [%d] - %d %d at %ds\n", pressedNum, i, j, timerCount);
                memset(tempCommand, 0x0, 16);
                //sprintf(value, "%03d", 127 + 23 - pressedNumber);
                sprintf(value, "%03d", 127 + 150 - (127 + 23 - pressedNumber));
                strncpy(tempCommand, value, 3);
                strncat(tempCommand, ",-1", 3);
                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                i2cWriteDataEndPos++;
                if(i2cWriteDataEndPos == 16)
                    i2cWriteDataEndPos = 0;

                debug_print("%s\n", tempCommand);

                pressedKey[pressedNumber] = 0;
            }
            else if(pressedKey[pressedNumber] == 1){	// means on pressed first pad then release
                pressedKeyTime[pressedNumber] = 0;
                pressedKey[pressedNumber] = 0;
                //printf("release [%d]\n", pressedNum);
            }
        }
    }
    
    /* second pad */
    uint8_t tempOutputConfiguration = -1;
    for(uint8_t pNum = 0; pNum < 48; pNum++){
        if(pressedKey[pNum] == 1){
            uint8_t configuration;
            if(pNum < 24){
                configuration = 5 - pNum / 8 * 2;
            }
            else{   // (pNum >= 24)
                configuration = 11 - (pNum-24) / 8 * 2;
            }
            if(configuration != tempOutputConfiguration){
                tempOutputConfiguration = configuration;
                //debug_print("temp configuration is %d\n", tempOutputConfiguration);
                
                IO_RA5_SetValue(tempOutputConfiguration & 0x1);
                IO_RD1_SetValue(tempOutputConfiguration & 0x2);
                IO_RC8_SetValue(tempOutputConfiguration & 0x4);
                IO_RA15_SetValue(tempOutputConfiguration & 0x8);
            }
            
            //uint8_t readPin = 7 - pNum % 8;
            uint8_t readPin = 7 - pNum & 0x7;
            
            if(GetInput(readPin) == 0){
                //printf("read input [%d] %d at %ds\n", pNum, timerCount, pressedKeyTime[pNum]);
                velocity = (timerCount - pressedKeyTime[pNum]);
                pressedKey[pNum] = 2;

                memset(tempCommand, 0x0, 16);
                //sprintf(value, "%03d", 127 + 23 - pNum);
                sprintf(value, "%03d", 127 + 150 - (127 + 23 - pNum));
                strncpy(tempCommand, value, 3);
                strncat(tempCommand, ",", 1);

                sprintf(value, "%04d", (int)(velocity * 10));
                strncat(tempCommand, value, 4);
                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                i2cWriteDataEndPos++;
                if(i2cWriteDataEndPos == 16)
                    i2cWriteDataEndPos = 0;

                debug_print("%s %s %04d\n",value , tempCommand, (int)velocity);
                //debug_print("i2c position %d %d \n",i2cWriteDataStartPos , i2cWriteDataEndPos);

            }
        }
    }
    
    return;
    
}

void ReadPedal(){
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
    
    /*                      i       j
     *  pedal:              15      3
     *  lower octave:       15      2
     *  raise octave:       15      1
     *  pause:              15      0
     *  sensitive           14      2
     *  sustain             14      1
     *  speed               14      3
     *  section knob        14      4,5
     *  speed knob          15      4,5
     */
    
    //debug_print("read pedal in loop count [%d]\n", loopCount);
    
    char tempCommand[16];
    
    
    //15 = 0b1111
    IO_RA5_SetValue(15 & 0x1);
    IO_RD1_SetValue(15 & 0x2);
    IO_RC8_SetValue(15 & 0x4);
    IO_RA15_SetValue(15 & 0x8);
    
    bool inputState3, inputState4, inputState5;
    inputState3 = GetInput(3);
    inputState4 = GetInput(4);
    inputState5 = GetInput(5);
    
    // speed knob
    
    // 0 (no) -> 1 (5) -> 2 (45) -> 3 (4) ->4 (no) counter-clockwise
    switch(speedKnobState){
    case 0:
        if(inputState4 == 0 && inputState5 != 0){
            speedKnobState = 1;
            debug_print("speed knob forward . %d\n", i2cWriteDataEndPos);

            memset(tempCommand, 0x0, 16);
            sprintf(tempCommand, "1021,1");
            memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
            i2cWriteDataEndPos++;
            if(i2cWriteDataEndPos == 16)
                i2cWriteDataEndPos = 0;

        }
        else if(inputState5 == 0 && inputState4 != 0){
            speedKnobState = 2;
            debug_print("speed knob backward. %d\n", i2cWriteDataEndPos);

            memset(tempCommand, 0x0, 16);
            sprintf(tempCommand, "1021,-1");
            memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
            i2cWriteDataEndPos++;
            if(i2cWriteDataEndPos == 16)
                i2cWriteDataEndPos = 0;
        }
        break;
    case 1:
    case 2:
            if(inputState4 == 0 && inputState5 == 0){
                speedKnobState = 0;
                debug_print("speed knob floating.\n");
            }
            break;
    }
    
    
    
    if(inputState3 == 0 && pedalDown != 1){
        pedalDown = 1;
        debug_print("Pedal Down.\n");

        memset(tempCommand, 0x0, 16);
        sprintf(tempCommand, "500,1");
        memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
        i2cWriteDataEndPos++;
        if(i2cWriteDataEndPos == 16)
            i2cWriteDataEndPos = 0;
    }
    
    if(inputState3 == 1 && pedalDown == 1){
        pedalDown = 0;
        debug_print("Pedal Up.\n");

        memset(tempCommand, 0x0, 16);
        sprintf(tempCommand, "500,0");
        memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
        i2cWriteDataEndPos++;
        if(i2cWriteDataEndPos == 16)
            i2cWriteDataEndPos = 0;
    }
    
    // section knob
    // 14 = 0b1110
    IO_RA5_SetValue(14 & 0x1);
    //IO_RD1_SetValue(14 & 0x2);
    //IO_RC8_SetValue(14 & 0x4);
    //IO_RA15_SetValue(14 & 0x8);
    
    inputState4 = GetInput(4);
    inputState5 = GetInput(5);
    
    switch(sectionKnobState){
    case 0:
        if(inputState4 == 0 && inputState5 != 0){
            sectionKnobState = 1;
            debug_print("section knob forward. %d\n", i2cWriteDataEndPos);

            memset(tempCommand, 0x0, 16);
            sprintf(tempCommand, "1020,1");
            memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
            i2cWriteDataEndPos++;
            if(i2cWriteDataEndPos == 16)
                i2cWriteDataEndPos = 0;

        }
        else if(inputState5 == 0 && inputState4 != 0){
            sectionKnobState = 2;
            debug_print("section knob backward. %d\n", i2cWriteDataEndPos);

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
            if(inputState4 == 0 && inputState5 == 0){
                sectionKnobState = 0;
                debug_print("section knob floating.\n");
            }
            break;
    }
    
}

void ReadPanel(){
    
    //loopCount++;
    
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
    
    for(uint8_t i = 14; i < 16; i++){
        
        if(i == 14){
            IO_RA5_SetValue(i & 0x1);
            IO_RD1_SetValue(i & 0x2);
            IO_RC8_SetValue(i & 0x4);
            IO_RA15_SetValue(i & 0x8);
        }
        else{   // i == 15
            IO_RA5_SetValue(i & 0x1);
            //IO_RD1_SetValue(i & 0x2);
            //IO_RC8_SetValue(i & 0x4);
            //IO_RA15_SetValue(i & 0x8);
        }
        
        for(uint8_t j = 0; j < 6; j++){
            /*
            if(GetInput(j) == 0){
                debug_print("read input %d %d at %ds\n", i, j, timerCount);
                
            }
            continue;
            */
            
            /*                      i       j
             *  pedal:              15      3
             *  lower octave:       15      2
             *  raise octave:       15      1
             *  pause:              15      0
             *  sensitive           14      2
             *  sustain             14      1
             *  speed               14      3
             *  section knob        14      4,5
             *  speed knob          15      4,5
             */
            
            if(true){

                if(GetInput(j) == 0){
                    if(false && i ==14 && (j == 4 || j == 5)){
                        if(0)
                        switch(sectionKnobState){
                        case 0:
                            if(GetInput(4) == 0 && GetInput(5) != 0){
                                sectionKnobState = 1;
                                debug_print("section knob forward. %d\n", i2cWriteDataEndPos);

                                memset(tempCommand, 0x0, 16);
                                sprintf(tempCommand, "1020,1");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;

                            }
                            else if(GetInput(5) == 0 && GetInput(4) != 0){
                                sectionKnobState = 2;
                                debug_print("section knob backward. %d\n", i2cWriteDataEndPos);

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
                                    debug_print("section knob floating.\n");
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
                                    debug_print("Press Sensitive button.\n");

                                    memset(tempCommand, 0x0, 16);
                                    //sprintf(tempCommand, "1002,1");
                                    sprintf(tempCommand, "1001,1");
                                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                    i2cWriteDataEndPos++;
                                    if(i2cWriteDataEndPos == 16)
                                            i2cWriteDataEndPos = 0;
                                }
                                break;
                            case 2:
                                if(sustainButton != 1){
                                    sustainButton = 1;
                                    debug_print("Press Sustain button.\n");

                                    memset(tempCommand, 0x0, 16);
                                    //sprintf(tempCommand, "1001,1");
                                    sprintf(tempCommand, "1002,1");
                                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                    i2cWriteDataEndPos++;
                                    if(i2cWriteDataEndPos == 16)
                                        i2cWriteDataEndPos = 0;
                                }
                                break;
                            case 3:
                                if(speedButton != 1){
                                    speedButton = 1;
                                    debug_print("Press Speed button.\n");

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
                                        debug_print("Press Pause button.\n");

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
                                    debug_print("Press Raise octave button.\n");

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
                                    debug_print("Press Lower octave button.\n");

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
                                    debug_print("Pedal Down.\n");

                                    memset(tempCommand, 0x0, 16);
                                    sprintf(tempCommand, "500,1");
                                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                    i2cWriteDataEndPos++;
                                    if(i2cWriteDataEndPos == 16)
                                        i2cWriteDataEndPos = 0;
                                }
                                break;
                                   
                            }
                            if(i ==15 && (j == 4 || j == 5)){
                                
                                // 0 (no) -> 1 (5) -> 2 (45) -> 3 (4) ->4 (no) counter-clockwise
                                if(0)
                                switch(speedKnobState){
                                case 0:
                                    if(GetInput(4) == 0 && GetInput(5) != 0){
                                        speedKnobState = 1;
                                        debug_print("speed knob forward. %d\n", i2cWriteDataEndPos);

                                        memset(tempCommand, 0x0, 16);
                                        sprintf(tempCommand, "1020,1");
                                        memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                        i2cWriteDataEndPos++;
                                        if(i2cWriteDataEndPos == 16)
                                            i2cWriteDataEndPos = 0;

                                    }
                                    else if(GetInput(5) == 0 && GetInput(4) != 0){
                                        speedKnobState = 2;
                                        debug_print("speed knob backward. %d\n", i2cWriteDataEndPos);

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
                                        debug_print("speed knob floating.\n");
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
                                debug_print("Release Sensitive button.\n");

                                memset(tempCommand, 0x0, 16);
                                //sprintf(tempCommand, "1002,0");
                                sprintf(tempCommand, "1001,0");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        case 2:
                            if(sustainButton == 1){
                                sustainButton = 0;
                                debug_print("Release Sustain button.\n");

                                memset(tempCommand, 0x0, 16);
                                //sprintf(tempCommand, "1001,0");
                                sprintf(tempCommand, "1002,0");
                                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                                i2cWriteDataEndPos++;
                                if(i2cWriteDataEndPos == 16)
                                    i2cWriteDataEndPos = 0;
                            }
                            break;
                        case 3:
                            if(speedButton == 1){
                                speedButton = 0;
                                debug_print("Release Speed button.\n");

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
                                debug_print("Release Pause button.\n");

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
                                debug_print("Release Raise octave button.\n");

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
                                debug_print("Release Lower octave button.\n");

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
                                debug_print("Pedal Up.\n");

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
            debug_print("Press power button.\n");

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
            debug_print("Release power button.\n");

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
            debug_print("Pedal plugged in.\n");

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
            debug_print("Pedal plugged out.\n");

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
    /*
    spi1_open(SPI1_DEFAULT);
    
    spi1_writeByte(indicatorLights[1]);
    spi1_writeByte(indicatorLights[0]);
    
    spi1_close();
    */
    
    // change to bit bang
    IO_RD2_SetValue(false);
    for(int i = 0; i < 16; i++){
        if(indicatorLights[15-i])
            IO_RD0_SetValue(true);
        else
            IO_RD0_SetValue(false);
        IO_RC13_SetValue(true);
        IO_RC13_SetValue(false);

    }
    IO_RD2_SetValue(true);
    
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
    else if(lightNumber == 5){
        for(i = 8; i < 14; i++){
            indicatorLights[i] = 0;
        }
        
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
 * 5: constant revolving
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
            brightnessArray[i] = 0x10;
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
            if(timeLength < 0){ // keep revolving
                pwmLightEffectType = 5;
                pwmTimeLength = 2 * 1000; // revolve one round per 2 second
            }
            else{   // revolve only once
                pwmLightEffectType = 1;
                pwmTimeLength = timeLength * 1000;
            }
            pwmTimeLengthLeft = pwmTimeLength;
            debug_print("pwm light effect 1 with time %d ms\n", pwmTimeLengthLeft);
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
        SetPwmLightRingRevolving(-1);
        return;
    }

    if(pwmLightEffectType == 2 || pwmLightEffectType == 4)
        return;

    if(pwmLightEffectType == 1 || pwmLightEffectType == 5){

        if(elapsedFrameTime < pwmTimeLengthLeft)
            pwmTimeLengthLeft -= elapsedFrameTime;
        else{
            if(pwmLightEffectType == 1){
                pwmLightEffectType = 0;
                SetPwmLightRingRevolving(-1);
                return;
            }
            else{   // pwmLightEffectType == 5
                pwmTimeLengthLeft = pwmTimeLength;
            }
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

#include <math.h>

/*
 * 0 -> 1 -> 2 -> 0
 * 0: no state
 * 1: a1 reading
 * 2: a2 reading
 */
uint8_t adcState = 0;

int an9 = 0;
int an10 = 0;

/*
 * the value to compute actual volume
 */
int convertedVolume = 0;

/*
 * the value of slider is 0~100
 * the value of read value is 0 ~ 925
 * the conversion follows the equation a*ln(b*x)=y, x=0~100, y=0~925
 * a=200.86119788025397028864712499893, b=1
 * we can get x from x=exp(y/200.86119788025397028864712499893)
 * 
 * the value would bounce between 5, so make a ease of ((previous value) + (new value)) / 2
 */
void ProcessAdc(){
    
    switch(adcState){
        case 0:
            //if(timerCount << 2 > adcTimerCount + 3){    // trigger every 256 * 4 ticks, which is 0.0256s * 4
            //    adcTimerCount = timerCount << 2;
                ADC1_ChannelSelect(channel_AN9);
                ADC1_SoftwareTriggerEnable();
                adcState = 1;
            //}
            break;
            
        case 1:
            ADC1_SoftwareTriggerDisable();
            uint32_t tempAn9 = ADC1_ConversionResultGet(channel_AN9);
            
            convertedVolume = exp(((double)tempAn9) / 200.86119788);
            //debug_print("analog 9 %d %d\n", convertedVolume, tempAn9);
            
            
            if((convertedVolume > an9 + 5 || convertedVolume < an9 - 5) ||
                (convertedVolume == 0 && convertedVolume != an9)){  // cause if volume happens to be 5, it will never become 1
                char tempCommand[16] = {0};
                char value[4] = {0};
                an9 = (convertedVolume + an9) / 2;

                memset(tempCommand, 0x0, 16);

                sprintf(tempCommand, "1030,");
                //strncat(tempCommand, "1030,", 5);
                //if(convertedVolume == 1){
                //    an9 = 1;
                //    strncat(tempCommand, "0", 101 - 1);
                //}
                if(convertedVolume > 95){
                    an9 = convertedVolume;
                    strncat(tempCommand, "0", 1);
                }
                else{
                    sprintf(value, "%03d", 101 - an9);
                    strncat(tempCommand, value, 3);
                }
                
                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                i2cWriteDataEndPos++;
                if(i2cWriteDataEndPos == 16)
                    i2cWriteDataEndPos = 0;

                debug_print("%d %s %04d\n",convertedVolume , tempCommand, an9);
            }
            
#if 0
            if(false)
            if(tempAn9 * tempAn9 > an9 + 10000 || tempAn9 * tempAn9 < an9 - 10000){
                if(tempAn9 * tempAn9 > 10000 || an9 > 10000){
                    
                    char tempCommand[16] = {0};
                    char value[4] = {0};
                    uint8_t volume = tempAn9 * tempAn9 / 8500;

                    memset(tempCommand, 0x0, 16);
                    
                    strncat(tempCommand, "1030,", 5);
                    sprintf(value, "%03d", volume);
                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                    i2cWriteDataEndPos++;
                    if(i2cWriteDataEndPos == 16)
                        i2cWriteDataEndPos = 0;

                    //debug_print("%s %s %04d\n",value , tempCommand, (int)volume);
                    debug_print("analog 9 %d %d\n", tempAn9 * tempAn9, tempAn9);
                    an9 = tempAn9 * tempAn9;
                }
            }
#endif
            adcState = 2;
            break;
            
        case 2:
            
            
            ADC1_ChannelSelect(channel_AN10);
            ADC1_SoftwareTriggerEnable();
            adcState = 3;
            break;
        
        case 3:
            ADC1_SoftwareTriggerDisable();
            uint32_t tempAn10 = ADC1_ConversionResultGet(channel_AN10);
            
            
            convertedVolume = exp(((double)tempAn10) / 200.86119788);
            //debug_print("analog 10 %d %d\n", convertedVolume, tempAn10);
            
            if((convertedVolume > an10 + 5 || convertedVolume < an10 - 5) ||
                (convertedVolume == 0 && convertedVolume != an10)){  // cause if volume happens to be 5, it will never become 1
                char tempCommand[16] = {0};
                char value[4] = {0};
                an10 = (convertedVolume + an10) / 2;

                memset(tempCommand, 0x0, 16);
                sprintf(tempCommand, "1031,");
                //strncat(tempCommand, "1031,", 5);
                //if(convertedVolume == 1){
                //    an10 = 1;
                //    strncat(tempCommand, "0", 1);
                //}
                if(convertedVolume > 95){
                    an10 = convertedVolume;
                    strncat(tempCommand, "0", 1);
                }
                else{
                    sprintf(value, "%03d", 101 - an10);
                    strncat(tempCommand, value, 3);
                }
                
                memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                i2cWriteDataEndPos++;
                if(i2cWriteDataEndPos == 16)
                    i2cWriteDataEndPos = 0;

                debug_print("%d %s %04d\n",convertedVolume , tempCommand, an10);
            }
            
#if 0
            if(false)
            if(tempAn10 * tempAn10 > an10 + 10000 || tempAn10 * tempAn10 < an10 - 10000){
                if(tempAn10 * tempAn10 > 10000 || an10 > 10000){
                    char tempCommand[16] = {0};
                    char value[4] = {0};
                    uint8_t volume = tempAn10 * tempAn10 / 8500;

                    memset(tempCommand, 0x0, 16);
                    
                    strncat(tempCommand, "1031,", 5);
                    sprintf(value, "%03d", volume);
                    memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
                    i2cWriteDataEndPos++;
                    if(i2cWriteDataEndPos == 16)
                        i2cWriteDataEndPos = 0;

                    //debug_print("%s %s %04d\n",value , tempCommand, (int)volume);
                    debug_print("analog 10 %d %d\n", tempAn10 * tempAn10, tempAn10);
                    an10 = tempAn10 * tempAn10;
                }
            }
#endif
            adcState = 4;
            break;
            
        case 4:
            
            ADC1_ChannelSelect(channel_AN9);
            ADC1_SoftwareTriggerEnable();
            adcState = 1;
            break;
    }
    
    
    
    return;
    
    switch(adcState){
        case 0:
            //if(timerCount << 2 > adcTimerCount + 3){    // trigger every 256 * 4 ticks, which is 0.0256s * 4
            //    adcTimerCount = timerCount << 2;
                ADC1_ChannelSelect(channel_AN9);
                ADC1_SoftwareTriggerEnable();
                adcState = 1;
            //}
            break;
            
        case 1:
            ADC1_SoftwareTriggerDisable();
            uint32_t tempAn9 = ADC1_ConversionResultGet(channel_AN9);
            debug_print("analog 9 %d %d\n", tempAn9 * tempAn9, tempAn9);
            if(tempAn9 * tempAn9 > an9 + 10000 || tempAn9 * tempAn9 < an9 - 10000){
                if(tempAn9 * tempAn9 > 10000 || an9 > 10000){
                    debug_print("analog 9 %d %d\n", tempAn9 * tempAn9, tempAn9);
                    an9 = tempAn9 * tempAn9;
                }
            }
                
            ADC1_ChannelSelect(channel_AN10);
            ADC1_SoftwareTriggerEnable();
            adcState = 2;
            break;
        
        case 2:
            ADC1_SoftwareTriggerDisable();
            uint32_t tempAn10 = ADC1_ConversionResultGet(channel_AN10);
            //debug_print("analog 10 %d %d\n", tempAn10 * tempAn10, tempAn10);
            if(tempAn10 * tempAn10 > an10 + 10000 || tempAn10 * tempAn10 < an10 - 10000){
                if(tempAn10 * tempAn10 > 10000 || an10 > 10000){
                    debug_print("analog 10 %d %d\n", tempAn10 * tempAn10, tempAn10);
                    an10 = tempAn10 * tempAn10;
                }
            }
            
            ADC1_ChannelSelect(channel_AN9);
            ADC1_SoftwareTriggerEnable();
            adcState = 1;
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
        debug_print("get RV %s \n", message);
        pointer = strtok(NULL, ",");
        value1 = atof(pointer);
        if(value1 > 0)
            SetLightRingEffect(1, value1);
        else if(value1 == 0)
            //SetLightRingEffect(3, value1);	// pause revolving ring
            SetLightRingEffect(0, value1);	// turn off
        else
            //SetLightRingEffect(0, value1);	// turn off light ring
            SetLightRingEffect(1, value1);	// keep revolving
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

        debug_print("%s | %d \n", newCommand, timerCount);

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
    uint32_t loopCountDevide8 = 0;
    uint32_t loopCountDevide16 = 0;
    uint32_t loopCountDevide32 = 0;
    uint32_t loopCountDevide64 = 0;
    
    debug_print("start\n");
    
    while(0){
        ReadAllDebug();
        loopCount++;
    }
    /*
    SetIndicatorLights(0,1);
    SetIndicatorLights(1,1); // lower octave
    SetIndicatorLights(2,1); // speed 0
    SetIndicatorLights(3,1); // speed 1
    SetIndicatorLights(4,1); // speed 2
    SetIndicatorLights(5,1); // speed 3
    SetIndicatorLights(6,1); // speed 4
    SetIndicatorLights(7,1); // speed 5
    SetIndicatorLights(8,1); // speed 5
    */
    
    // repeatly revolving light ring
    SetLightRingEffect(1, -1);
    
    while(1){
        ReadKeyboard();
        ReadPedal();
        if(loopCount >> 3 > loopCountDevide8){  // 125hz
            loopCountDevide8 = loopCount >> 3;
            
            if(loopCount >> 4 > loopCountDevide16){ // 62.5hz
                loopCountDevide16 = loopCount >> 4;
                // ReadPanel
                ReadPanel();
                
                if(loopCount >> 5 > loopCountDevide32){ // 31.25hz
                    loopCountDevide32 = loopCount >> 5;
                    // I2C input
                    ProcessI2cMessage();
                    // PWM
                    UpdateLightRingEffect((timerCount - lastFrameTimerCount));
                    lastFrameTimerCount = timerCount;
                    // SPI
                    
                    if(loopCount >> 6 > loopCountDevide64){ // 15.625hz
                        loopCountDevide64 = loopCount >> 6;
                        ProcessAdc();
                        /*
                        SetIndicatorLights(loopCountDevide64 % 16, 1);
                        if(loopCountDevide64 % 16 > 0)
                            SetIndicatorLights(loopCountDevide64 % 16 - 1, 0);
                        else
                            SetIndicatorLights(15, 0);
                         */
                    }
                    
                }
            }
            else{
                ReadPedal();
            }
        }
        loopCount++;
    }
    
    while(1){
        if(timerCount > lastFrameTimerCount + 100 || timerCount < lastFrameTimerCount){
            ProcessAdc();
            lastFrameTimerCount = timerCount;
        }
    }
    
    while (1)
    {
        //debug_print("loop, %d\n", timerCount);
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

