/*************************************************************************
 *  © 2016 Microchip Technology Inc.
 *
 *  Project Name:   PIC32MM I2C Bit Bang Library
 *  FileName:       i2c_master.S
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *  Description: This assembler source file contains MASTER I2C functions implementations.
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

#include "i2c_master.h"
 
    .global I2CM_Init        
    .global I2CM_Start    
    .global I2CM_Stop        
    .global I2CM_Write        
    .global I2CM_Read          
   
    .macro DELAY num
    .if (\num < 10)
    nop
    .if (\num - 1)
        DELAY (\num - 1)
    .endif
    .else
    ori t7, zero, \num/2
1:  bnez t7, 1b
    addi t7, t7, -1
    .endif
    .endm

   .section .text, code
   .set noreorder
   .set nomips16

################################################################################
################################################################################   
   
I2CM_Init:    

    lui  t0, %hi(SCL_PORT) # upper 16 bit of IO address
    ori  t1, zero, SCL_MASK        
    ori  t2, zero, SDA_MASK        
    sw   t1,%lo(SCL_ODC_SET)(t0)
    sw   t1,%lo(SCL_LAT_SET)(t0)
    sw   t2,%lo(SDA_ODC_SET)(t0)
    sw   t2,%lo(SDA_LAT_SET)(t0)    
    sw   t1,%lo(SCL_TRIS_CLR)(t0)
    sw   t2,%lo(SDA_TRIS_CLR)(t0)    
    
    jr   ra
    nop   
   
################################################################################
################################################################################ 
   
I2CM_Start:    
    lui  t0, %hi(SCL_PORT) # upper 16 bit of IO address
    ori  t1, zero, SCL_MASK        
    ori  t2, zero, SDA_MASK    
   
    # SDA to high
    sw   t2,%lo(SDA_LAT_SET)(t0)    

    DELAY 2*I2C_CLOCK_DELAY
    
    # SCL to high 
    sw   t1,%lo(SCL_LAT_SET)(t0)
    
    DELAY 2*I2C_CLOCK_DELAY
    
    # check bus status    
    lw   t3,%lo(SCL_PORT)(t0) 
    and  t3, t1, t3
    beqz t3, i2cm_start_bus_collision
    nop        
    lw   t3,%lo(SDA_PORT)(t0) 
    and  t3, t2, t3
    beqz t3, i2cm_start_bus_collision
    nop

    # SDA to low
    sw   t2,%lo(SDA_LAT_CLR)(t0)

    DELAY 2*I2C_CLOCK_DELAY
    
    # SCL to low
    sw   t1,%lo(SCL_LAT_CLR)(t0)    
    
    DELAY 2*I2C_CLOCK_DELAY    
    
    # successful
    or   v0, zero, zero
    jr   ra
    nop
    
i2cm_start_bus_collision:
    # failed
    ori  v0, zero, 1
    jr   ra
    nop

################################################################################
################################################################################
    
I2CM_Stop:    
    lui  t0, %hi(SCL_PORT) # upper 16 bit of IO address
    ori  t1, zero, SCL_MASK        
    ori  t2, zero, SDA_MASK    

    # SCL to low 
    sw   t1,%lo(SCL_LAT_CLR)(t0)
    
    DELAY 2*I2C_CLOCK_DELAY    
    
    # SDA to low
    sw   t2,%lo(SDA_LAT_CLR)(t0)
    
    DELAY 2*I2C_CLOCK_DELAY    
    
    # SCL to high
    sw   t1,%lo(SCL_LAT_SET)(t0)    
    
    DELAY 2*I2C_CLOCK_DELAY    
    
    # SDA to high
    sw   t2,%lo(SDA_LAT_SET)(t0)
        
    # check bus status    
    lw   t3,%lo(SCL_PORT)(t0) 
    and  t3, t1, t3
    beqz t3, i2cm_stop_bus_collision
    nop        
    lw   t3,%lo(SDA_PORT)(t0) 
    and  t3, t2, t3
    beqz t3, i2cm_stop_bus_collision
    nop
    
    DELAY 2*I2C_CLOCK_DELAY        
    
    # successful
    or   v0, zero, zero
    jr   ra
    nop
    
i2cm_stop_bus_collision:
    # failed
    ori  v0, zero, 1
    jr   ra
    nop

################################################################################
################################################################################
    
I2CM_Write:    
    lui  t0, %hi(SCL_PORT) # upper 16 bit of IO address
    ori  t1, zero, SCL_MASK        
    ori  t2, zero, SDA_MASK          
    # SCL to low 
    sw   t1,%lo(SCL_LAT_CLR)(t0)    
    
    ori  t3, zero, 7 # bits counter counts 8 bits from 7 to 0
    
i2cm_write_bit:      
    # data bit to SDA    
    andi t4, a0, 0x0080
    beqz t4, i2cm_write_zero
    sll  a0, a0, 1
i2cm_write_one:    
    sw   t2,%lo(SDA_LAT_SET)(t0)    
    b    i2cm_write_clock_pulse
    nop
i2cm_write_zero:        
    sw   t2,%lo(SDA_LAT_CLR)(t0)

i2cm_write_clock_pulse:    
    DELAY I2C_CLOCK_DELAY
    sw   t1,%lo(SCL_LAT_SET)(t0)
    nop
    nop
i2cm_wtite_clock_stretching:
    lw   t4,%lo(SCL_PORT)(t0) 
    and  t4, t1, t4
    beqz t4, i2cm_wtite_clock_stretching

    DELAY 2*I2C_CLOCK_DELAY
    sw   t1,%lo(SCL_LAT_CLR)(t0)
    DELAY I2C_CLOCK_DELAY

    bnez t3, i2cm_write_bit
    addi t3, t3, -1
                    
    # SDA to input
    sw   t2,%lo(SDA_TRIS_SET)(t0)    
    DELAY I2C_CLOCK_DELAY
    sw   t1,%lo(SCL_LAT_SET)(t0)
    nop
    nop
i2cm_wtite_ack_clock_stretching:
    lw   t4,%lo(SCL_PORT)(t0) 
    and  t4, t1, t4
    beqz t4, i2cm_wtite_ack_clock_stretching

    DELAY 2*I2C_CLOCK_DELAY    
    lw   t3,%lo(SDA_PORT)(t0) 
    and  t3, t2, t3    
    sw   t2,%lo(SDA_TRIS_CLR)(t0) # SDA to output
    sw   t1,%lo(SCL_LAT_CLR)(t0)
    DELAY I2C_CLOCK_DELAY
        
    beqz t3, i2cm_ack
    or   v0, zero, zero    
        
i2cm_nack:
    ori  v0, zero, 1
    jr   ra
    nop
    
i2cm_ack:
    jr   ra
    nop
      
################################################################################
################################################################################
    
I2CM_Read:    
    lui  t0, %hi(SCL_PORT) # upper 16 bit of IO address
    ori  t1, zero, SCL_MASK        
    ori  t2, zero, SDA_MASK          
    # SCL to low 
    sw   t1,%lo(SCL_LAT_CLR)(t0) 
    # SDA to input
    sw   t2,%lo(SDA_TRIS_SET)(t0)    
    
    or   v0, zero, zero
    ori  t3, zero, 7 # bits counter counts 8 bits from 7 to 0
    
i2cm_read_bit:
    
    DELAY I2C_CLOCK_DELAY
    sw   t1,%lo(SCL_LAT_SET)(t0)
    nop
    nop
i2cm_read_clock_stretching:
    lw   t4,%lo(SCL_PORT)(t0) 
    and  t4, t1, t4
    beqz t4, i2cm_read_clock_stretching        
    
    DELAY 2*I2C_CLOCK_DELAY
    
    lw   t4,%lo(SDA_PORT)(t0) 
    and  t4, t2, t4
    beqz t4, i2cm_read_zero
    sll  v0, v0, 1
i2cm_read_one:
    ori  v0, v0, 1
i2cm_read_zero:    
    
    sw   t1,%lo(SCL_LAT_CLR)(t0)
    DELAY I2C_CLOCK_DELAY

    bnez t3, i2cm_read_bit
    addi t3, t3, -1
    
    # SDA to output
    sw   t2,%lo(SDA_TRIS_CLR)(t0)        
        
    beqz a0, i2cm_read_acknowlegement_zero 
    nop
    sw   t2,%lo(SDA_LAT_SET)(t0)     
    b    i2cm_read_acknowlegement_clock
    nop
i2cm_read_acknowlegement_zero:
    sw   t2,%lo(SDA_LAT_CLR)(t0) 
    
i2cm_read_acknowlegement_clock:
    DELAY I2C_CLOCK_DELAY
    sw   t1,%lo(SCL_LAT_SET)(t0)
    DELAY 2*I2C_CLOCK_DELAY        
    sw   t1,%lo(SCL_LAT_CLR)(t0)    
    DELAY I2C_CLOCK_DELAY
    
    jr   ra
    nop
  