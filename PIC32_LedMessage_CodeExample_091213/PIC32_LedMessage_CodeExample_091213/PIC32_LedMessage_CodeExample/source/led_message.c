/*********************************************************************
 *
 *     PIC32MX Message Display via 8 LEDs
 *
 *********************************************************************
 * FileName:        led_message.c
 * Dependencies:    plib.h
 *
 * Processor:       PIC32MX
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the ï¿½Companyï¿½) for its PIC32MX Microcontroller is intended
 * and supplied to you, the Companyï¿½s customer, for use solely and
 * exclusively on Microchip Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN ï¿½AS ISï¿½ CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *********************************************************************
 * $Id: led_message.c 9560 2008-07-04 10:10:53Z deshmukha $
 *********************************************************************
 *
 * Thanks to Lucio DiJasio for letting us use this example.
 *
 * Platform: Explorer-16 with PIC32MX PIM
 *
 * Description:
 *      Hold the board vertically from the PICtail connector side
 *      and wave the board back-and-forth to see the message "HELLO"
 *      on LEDs
 ********************************************************************/
#include <plib.h>

#if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__) || (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 80 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
#define SYS_FREQ (80000000L)
#endif

// Define timing constants
#define SHORT_DELAY (500*8)
#define LONG_DELAY  (2000*8)

// Declare and initialize an array with the message bitmap
char bitmap[30] = {
    0xff,	// H
    0x08,
    0x08,
    0xff,
    0,
    0,
    0xff,	// E
    0x89,
    0x89,
    0x81,
    0,
    0,
    0xff,	// L
    0x80,
    0x80,
    0x80,
    0,
    0,
    0xff,	// L
    0x80,
    0x80,
    0x80,
    0,
    0,
    0x7e,	// O
    0x81,
    0x81,
    0x7e,
    0,
    0
    };


int main(void)
{
    int i;  // i will serve as the index

    // Explorer-16 LEDs are on lower 8-bits of PORTA and to use all LEDs, JTAG port must be disabled.
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    // Clear PORTA bits so there are no unexpected flashes when setting
    // them to output in the next step
    mPORTAClearBits(BIT_7 | BIT_6 | BIT_5 | BIT_4 | \
                                    BIT_3 | BIT_2 | BIT_1 | BIT_0);

    // Make all lower 8-bits of PORTA as output
    mPORTASetPinsDigitalOut( BIT_7 | BIT_6 | BIT_5 | BIT_4 | \
                                             BIT_3 | BIT_2 | BIT_1 | BIT_0);
    // Start timer1, Fpb/256, max period
    OpenTimer1(T1_ON | T1_PS_1_256 | T1_SOURCE_INT, 0xFFFF);

    // The main loop
    while( 1)
    {
        // Display loop, hand moving to the right
        for( i=0; i<30; i++)
        {	
            PORTA = bitmap[i]; // Update the LEDs

            // Short pause
            WriteTimer1(0);
            while ( TMR1 < SHORT_DELAY);
        } 

        // Long pause, hand moving back to the left
        PORTA = 0; // Turn off LEDS
        WriteTimer1(0);
        while ( TMR1 < LONG_DELAY);
    } 
} 
