/*************************************************************************
 *  © 2016 Microchip Technology Inc.
 *
 *  Project Name:   PIC32MM I2C Bit Bang Library
 *  FileName:       i2c_slave.h
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *  Description: This header contains I/O definitions/selection used for
 *  the SLAVE I2C interface. Also this file includes prototypes of
 *  SLAVE I2C functions.
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

#ifndef _I2C_SLAVE_
#define _I2C_SLAVE_

#include <xc.h>

////////////////////////////////////////////////////////////////////////////////
// This parameter sets the bit position of I/O used for SCL signal.
#define SCL_MASK        (1<<8)
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the TRIS CLEAR register of I/O used for SCL signal.
// VERIFY THAT IN YOUR APPLICATION THE SCL PIN IS CONFIGURED AS
// A DIGITAL INPUT IN ANSELx REGISTER.
#define SCL_TRIS_CLR    TRISBCLR
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the TRIS SET register of I/O used for SCL signal.
#define SCL_TRIS_SET    TRISBSET
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the ODC SET register of I/O used for SCL signal.
#define SCL_ODC_SET     ODCBSET
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the LAT CLEAR register of I/O used for SCL signal.
#define SCL_LAT_CLR     LATBCLR    
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the LAT SET register of I/O used for SCL signal.
#define SCL_LAT_SET     LATBSET        
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the PORT register of I/O used for SCL signal.
#define SCL_PORT        PORTB            

////////////////////////////////////////////////////////////////////////////////
// This parameter sets the bit position of I/O used for SDA signal.
#define SDA_MASK        (1<<9)
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the TRIS CLEAR register of I/O used for SDA signal.
// VERIFY THAT IN YOUR APPLICATION THE SCL PIN IS CONFIGURED AS
// A DIGITAL INPUT IN ANSELx REGISTER.
#define SDA_TRIS_CLR    TRISBCLR
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the TRIS SET register of I/O used for SDA signal.
#define SDA_TRIS_SET    TRISBSET
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the ODC SET register of I/O used for SDA signal.
#define SDA_ODC_SET     ODCBSET
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the LAT CLEAR register of I/O used for SDA signal.
#define SDA_LAT_CLR     LATBCLR    
////////////////////////////////////////////////////////////////////////////////
// This parameter sets the LAT SET register of I/O used for SDA signal.
#define SDA_LAT_SET     LATBSET        
// This parameter sets the PORT register of I/O used for SDA signal.
#define SDA_PORT        PORTB

////////////////////////////////////////////////////////////////////////////////
// Add/uncomment the definition of this parameter to disable clock stretching.
// #define I2C_DISABLE_CLOCK_STRETCHING

////////////////////////////////////////////////////////////////////////////////
// void I2CS_Init()
// Description: This function initializes SDA and SCL I/Os.
// Parameters: None.
// Returned data: None.
extern void I2CS_Init();

////////////////////////////////////////////////////////////////////////////////
// void I2CS_Task()
// Description: This function is an engine to process signals on SDA and SCL I/Os.
// If some I2C event will be detected this function will pass control to
// the corresponding CALLBACK FUNCTION implemented in the user?s code.
// The following callback functions are called:
//    I2C START => I2CS_Start()
//    I2C STOP  => I2CS_Stop()
//    BYTE RECEVED FROM MASTER => I2CS_Read(?)
//    REQUEST TO TRANSMIT A BYTE TO MASTER => I2CS_Write(?)
// The I2CS_Task() function must be executed periodically. It can be done by:
//    1.Change Notification interrupts on both SCL and SDA I/Os (the interrupts
//      must detect positive and negative edges/transitions)
//    2. calling it in main idle loop
//    3. timer interrupt
// Parameters: None.
// Returned data: None.
extern void I2CS_Task();

////////////////////////////////////////////////////////////////////////////////
// void I2CM_Start()
// Description: This is a CALL BACK function controlled by I2CS_Task() function.
// If it is implemented in the application it will be called each time when
// I2C start signal is detected.
// Parameters: None.
// Returned data: None.
extern void I2CS_Start();

////////////////////////////////////////////////////////////////////////////////
// void I2CM_Stop()
// Description: This is a CALL BACK function controlled by I2CS_Task() function.
// If it is implemented in the application it will be called each time when
// I2C stop signal is detected.
// Parameters: None.
// Returned data: None.
extern void I2CS_Stop();

////////////////////////////////////////////////////////////////////////////////
// long I2CS_Read(unsigned char data)
// Description: This is a CALL BACK function controlled by I2CS_Task() function.
// If it is implemented in the application it will be called each time when
// 8-bit data are received from I2C master.
// Parameters:
// unsigned char data ? data received from I2C master.
// Returned data:
// Return/Pass the Acknowledgment (bit #0) and Write Mode (bit #1) flags
// to the library. If the data received must be acknowledged then clear bit #0.
// For NACK return one in bit #0. If for the next transaction the I2C slave must
// transmit data to master then return/pass one in bit #1. If for the next
// transaction the I2C slave must still receive the data from master
// then clear bit #1. 
extern long I2CS_Read(unsigned char data);

////////////////////////////////////////////////////////////////////////////////
// unsigned char I2CS_Write(long prev_ack)
// Description: This is a CALL BACK function controlled by I2CS_Task() function.
// If it is implemented in the application it will be called each time when
// I2C master will request 8-bit data from slave.
// Parameters:
// long prev_ack ? acknowledgment for the previous transaction. In most cases
// if the master answered with NACK (=1) before, the new data are not required
// and master will generate a stop event soon.
// Returned data:
// Return/Pass the 8-bit data to be transmitted to I2C master.
extern unsigned char I2CS_Write(long prev_ack);

#endif
