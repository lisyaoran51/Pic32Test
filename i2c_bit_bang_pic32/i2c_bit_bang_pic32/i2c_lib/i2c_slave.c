/*************************************************************************
 *  © 2016 Microchip Technology Inc.
 *
 *  Project Name:   PIC32MM I2C Bit Bang Library
 *  FileName:       i2c_slave.c
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *  Description: This file contains I2C bit bang library functions.
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

volatile long datain = 0;
volatile long dataout = 0;
volatile long mask = 0;
volatile long read_flag = 0;
volatile long ack = 0;
volatile long sda = SDA_MASK;
volatile long scl = SCL_MASK;

unsigned char __attribute__((weak)) I2CS_Write(long prev_ack){
    return 0x55; // transmit 0x55 by default
}

long __attribute__((weak)) I2CS_Read(unsigned char data){
    return 0; // acknowledge by default
}

void __attribute__((weak)) I2CS_Start(){
}

void __attribute__((weak)) I2CS_Stop(){
}

void I2CS_Init(){
    SCL_LAT_CLR = SCL_MASK;
    SCL_ODC_SET = SCL_MASK;
    SCL_TRIS_SET = SCL_MASK;        
    SDA_ODC_SET = SDA_MASK;
    SDA_TRIS_SET = SDA_MASK;    
}

void I2CS_RX_SCL_0to1(){
    if(mask == 0x0000){
        return;    
    }
    if(sda != 0){
        datain |= mask;
    }
}
void I2CS_RX_SCL_1to0(){
    
    if(mask == 0x0000){        
        datain = 0;
        mask = 0x0080;
        if(read_flag == 1)
        {
            dataout = I2CS_Write(ack);
            if(dataout&mask){
                SDA_LAT_SET = SDA_MASK;
            }else{
                SDA_LAT_CLR = SDA_MASK;            
            }
            return;
        }
        SDA_TRIS_SET = SDA_MASK; // release SDA from acknowledgment        
        return;
    }            
    if(mask == 0x0001){ // 8th bit
        ack = I2CS_Read(datain);
        if(ack&2)
        {
            read_flag = 1;
        }
        if(ack&1){
            SDA_LAT_SET = SDA_MASK;
        }else{
            SDA_LAT_CLR = SDA_MASK;            
        }
        SDA_TRIS_CLR = SDA_MASK; // acknowledgment
    }
    mask >>= 1;              
}

void I2CS_TX_SCL_0to1(){   
    if(mask == 0x0000){        
        ack = sda;
    }
}

void I2CS_TX_SCL_1to0(){
    if(mask == 0x0000){
        mask = 0x0080;
        if(read_flag == 1)
        {
           
            dataout = I2CS_Write(ack);
            if(dataout&mask){
                SDA_LAT_SET = SDA_MASK;
            }else{
                SDA_LAT_CLR = SDA_MASK;            
            }
            SDA_TRIS_CLR  = SDA_MASK; // switch to send data            
            return;
        }
        return;    
    }
    
    if(mask == 0x0001){ // 8th bit
        SDA_TRIS_SET = SDA_MASK; // switch to input for acknowledgment
        return;
    }    
    mask >>= 1;    
    if(dataout&mask){
        SDA_LAT_SET = SDA_MASK;
    }else{
        SDA_LAT_CLR = SDA_MASK;            
    }
}

void I2CS_Task(){
    
    if(scl^(SCL_PORT&SCL_MASK)){
        // SCL was changed
        sda = (SDA_PORT&SDA_MASK);
        if(read_flag){
            if(scl){
                #ifndef I2C_DISABLE_CLOCK_STRETCHING                        
                SCL_TRIS_CLR = SCL_MASK; // hold SCL
                #endif                 
                I2CS_TX_SCL_1to0();
                #ifndef I2C_DISABLE_CLOCK_STRETCHING                        
                SCL_TRIS_SET = SCL_MASK; // release SCL                    
                #endif                                
            }else{
                I2CS_TX_SCL_0to1();                                    
            }            
        }else{
            if(scl){
                #ifndef I2C_DISABLE_CLOCK_STRETCHING                        
                SCL_TRIS_CLR = SCL_MASK; // hold SCL
                #endif                                 
                I2CS_RX_SCL_1to0();            
                #ifndef I2C_DISABLE_CLOCK_STRETCHING                        
                SCL_TRIS_SET = SCL_MASK; // release SCL                    
                #endif                
            }else{
                I2CS_RX_SCL_0to1();
            }                        
        }
        scl ^= SCL_MASK; // update SCL
        return;
    }   
    if(scl)
    {
        if(sda^(SDA_PORT&SDA_MASK)){
            // SDA was changed
            datain = 0;
            mask = 0;
            read_flag = 0;
            ack = 0;                            
            if(sda){
                I2CS_Start();
            }else{            
                I2CS_Stop();                
            }
            sda ^= SDA_MASK; // update SDA
        }       
    }    
}
