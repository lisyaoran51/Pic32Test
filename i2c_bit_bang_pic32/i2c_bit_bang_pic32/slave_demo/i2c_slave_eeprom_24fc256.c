/*************************************************************************
 *  © 2016 Microchip Technology Inc.
 *
 *  Project Name:   PIC32MM I2C Bit Bang Library
 *  FileName:       i2c_slave_eeprom_24fc256.c
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *  Description: This file contains I2C slave call back functions called
 *               by I2C slave task to emulate 24FC256 EEPROM.
 *
 *************************************************************************/
/**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and
 * any derivatives created by any person or entity by or on your behalf,
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your reference in accompanying documentation. Microchip and its licensors
 * retain all ownership and intellectual property rights in the
 * accompanying software and in all derivatives hereto.
 *
 * This software and any accompanying information is for suggestion only.
 * It does not modify Microchip's standard warranty for its products. You
 * agree that you are solely responsible for testing the software and
 * determining its suitability. Microchip has no obligation to modify,
 * test, certify, or support the software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
 * APPLICATION.
 *
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY,
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE,
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE,
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW,
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
 * THESE TERMS.
 *************************************************************************
 *
 * written by Anton Alkhimenok 05/20/2016
 *
 *************************************************************************/

#include "i2c_slave.h"

// 24FC256 COMMUNICATION PROTOCOL STATES
typedef enum {
    STATE_DEV_ADDRESS, // device address will be received
    STATE_ADDRESS_HIGH_BYTE, // high byte of memory address will be recieved            
    STATE_ADDRESS_LOW_BYTE, // low byte of memory address will be recieved
    STATE_DATA_READ, // data byte will be read from master
    STATE_DATA_WRITE // data byte will be sent to master
} I2C_STATE;

// current state
I2C_STATE state = STATE_DEV_ADDRESS;

// 24FC256 device 7-bit address
#define EEPROM_DEV_ADDRESS     0x50  // from 24FC256 datasheet
#define EEPROM_SIZE            256   // 256 bytes

unsigned char eeprom_data[EEPROM_SIZE]; // memory storage
long          eeprom_address = 0; // current memory address

// This callback function is called every time when I2C start is detected
void I2CS_Start(){
    state = STATE_DEV_ADDRESS; // after start the device address byte will be transmitted
}

// This callback function is called every time when data from I2C master are received
long I2CS_Read(unsigned char data){

    switch(state){
            
        case STATE_DEV_ADDRESS:
            if((data >> 1) == EEPROM_DEV_ADDRESS){ // bits from #7 to #1 are device address
                if(data&1){ // if bit #0 is set (=1) it indicates that the next data go from slave to master
                    state = STATE_DATA_WRITE;                                        
                    return 2; // ACK to master (bit #0 = 0), master reads data on next transaction (bit #1 = 1) 
                }else{ // if bit #0 is cleared (=0) it indicates that the next data go from master to slave
                    state = STATE_ADDRESS_HIGH_BYTE;
                    return 0; // ACK to master                                
                }
            }
            return 1; // NACK to master if device address doesn't match EEPROM_DEV_ADDRESS
        case STATE_ADDRESS_HIGH_BYTE:
            state = STATE_ADDRESS_LOW_BYTE;
            eeprom_address = (data<<8);
            return 0; // ACK to master
        case STATE_ADDRESS_LOW_BYTE:
            state = STATE_DATA_READ;
            eeprom_address |= data;
            return 0; // ACK to master
        case STATE_DATA_READ:
            state = STATE_DEV_ADDRESS;
            if(eeprom_address >= EEPROM_SIZE){
                return 1;  // NACK to master, the memory address is wrong
            }            
            eeprom_data[eeprom_address] = data; // store the data received
            return 0; // ACK to master            
        default:
			state = STATE_DEV_ADDRESS;
            return 1; // NACK to master / unknown state
    }

    return 1; // NACK to master           
}

// This callback function is called every time when data must be sent to I2C master
unsigned char I2CS_Write(long prev_ack){
    if(eeprom_address >= EEPROM_SIZE)
    {
        return 0;
    }
    return eeprom_data[eeprom_address]; // send memory data to master
}