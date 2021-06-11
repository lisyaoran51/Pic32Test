/*************************************************************************
 *  © 2016 Microchip Technology Inc.
 *
 *  Project Name:   PIC32MM I2C Bit Bang Library
 *  FileName:       i2c_master.h
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *  Description: This header contains I/O definitions/selection and
 *  timing/clock speed settings used for the MASTER I2C interface.
 *  Also this file includes prototypes of MASTER I2C functions.
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

#ifndef _I2C_MASTER_
#define _I2C_MASTER_

#include <xc.h>

#ifdef __LANGUAGE_ASSEMBLY__

/******************************************************************************
 * This parameter is a quarter of I2C clock period in instruction cycles.
 * It defines timing for I2C interface. I2C MAY NOT WORK IF THIS PARAMETER IS
 * WRONG (CLOCK IS TOO FAST). */
    .equ I2C_CLOCK_DELAY, 100
      
/******************************************************************************
 * This parameter sets the bit position of I/O used for SCL signal. */
    .equ SCL_MASK, 1<<8
/******************************************************************************            
 * This parameter sets the TRIS CLEAR register of I/O used for SCL signal.
 * VERIFY THAT IN YOUR APPLICATION THE SCL PIN IS CONFIGURED AS
 * A DIGITAL INPUT IN ANSEL REGISTER. */
    .equ SCL_TRIS_CLR, TRISBCLR
/******************************************************************************            
 * This parameter sets the ODC SET register of I/O used for SCL signal. */
    .equ SCL_ODC_SET,  ODCBSET
/******************************************************************************            
 * This parameter sets the LAT CLEAR register of I/O used for SCL signal. */            
    .equ SCL_LAT_CLR,  LATBCLR    
/******************************************************************************            
 * This parameter sets the LAT SET register of I/O used for SCL signal. */                        
    .equ SCL_LAT_SET,  LATBSET    
/******************************************************************************            
 * This parameter sets the PORT register of I/O used for SCL signal. */                        
    .equ SCL_PORT,     PORTB            

/******************************************************************************
 * This parameter sets the bit position of I/O used for SDA signal. */            
    .equ SDA_MASK, 1<<9
/******************************************************************************            
 * This parameter sets the TRIS CLEAR register of I/O used for SDA signal.
 * VERIFY THAT IN YOUR APPLICATION THE SCL PIN IS CONFIGURED AS
 * A DIGITAL INPUT IN ANSEL REGISTER. */            
    .equ SDA_TRIS_CLR, TRISBCLR
/******************************************************************************            
 * This parameter sets the TRIS SET register of I/O used for SDA signal. */           
    .equ SDA_TRIS_SET, TRISBSET
/******************************************************************************            
 * This parameter sets the ODC SET register of I/O used for SDA signal. */                       
    .equ SDA_ODC_SET,  ODCBSET
/******************************************************************************            
 * This parameter sets the LAT CLEAR register of I/O used for SDA signal. */                                   
    .equ SDA_LAT_CLR,  LATBCLR    
/******************************************************************************            
 * This parameter sets the LAT SET register of I/O used for SDA signal. */                                               
    .equ SDA_LAT_SET,  LATBSET
/******************************************************************************            
 * This parameter sets the PORT register of I/O used for SDA signal. */                                               
    .equ SDA_PORT,     PORTB

#else

////////////////////////////////////////////////////////////////////////////////            
// void I2CM_Init()
// Description: This function initializes SDA and SCL I/Os.
// Parameters: None.
// Returned data: None.            
extern void I2CM_Init();
    
////////////////////////////////////////////////////////////////////////////////
// long I2CM_Start()
// Description: This function generates an I2C start signal.
// Parameters: None.
// Returned data: The function returns non-zero value if the bus collision is detected.    
extern long I2CM_Start();

////////////////////////////////////////////////////////////////////////////////
// long I2CM_Stop()
// Description: This function generates I2C stop signal.
// Parameters: None.
// Returned data:
// The function returns non-zero value if the bus collision is detected.
extern long I2CM_Stop();

////////////////////////////////////////////////////////////////////////////////
// long I2CM_Write(unsigned char data)
// Description: This function transmits 8-bit data to slave.
// Parameters:
// unsigned char data ? data to be transmitted
// Returned data:
// This function returns acknowledgment from slave (0 means ACK and 1 means NACK).
extern long I2CM_Write(unsigned char data);

////////////////////////////////////////////////////////////////////////////////
// unsigned char I2CM_Read(long ack)
// Description: This function reads 8-bit data from slave.
// Parameters:
// long ack ? acknowledgment to be sent to slave
// Returned data:
// This function returns 8-bit data read.
extern unsigned char I2CM_Read(long ack);

#endif

#endif