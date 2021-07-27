/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * $Revision: 7 $
 * $Date: 15/07/13 1:27p $
 * @brief    Show how to set GPIO pin mode and use pin data input/output control.
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define PLLCON_SETTING      CLK_PLLCON_50MHz_HXT
#define PLL_CLOCK           50000000

#define VK49KEY
//#define VK24KEY_X2

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO varaibles                                                                              						 */
/*---------------------------------------------------------------------------------------------------------*/

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
		
		/*
		 * speed knob and section knob cannot detect at the same time
		 */
		uint8_t outputToPass = 14;
		
		/*
		 * speed knob state is changed at last run
		 */
		uint8_t hasLastSpeedKnobState = 0;


/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototypes                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

void AdcContScanModeTest(void);


/*---------------------------------------------------------------------------------------------------------*/
/* ADC                                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
double sliderDataAccumulation[2] = {0};
int sliderDataAverage[2] = {0};
uint8_t adcScanCount = 0;
#define MaxAdcScanTimes 10.0

/*---------------------------------------------------------------------------------------------------------*/
/* indicator lights                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

uint8_t indicatorLights[16] = {0};

#define LatchPin P14
#define DataPin P15
#define ClockPin P17


/*---------------------------------------------------------------------------------------------------------*/
/* PWM lights                                                                                              */
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




/*---------------------------------------------------------------------------------------------------------*/
/* I2C Start                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

uint8_t i2cWriteData[16][16];		// mcu write pi
uint8_t i2cReadData[16][16];		// mcu read pi

volatile uint8_t i2cWriteDataStartPos = 0;
volatile uint8_t i2cWriteDataEndPos = 0;

volatile uint8_t i2cReadDataStartPos = 0;
volatile uint8_t i2cReadDataEndPos = 0;

uint8_t i2cTempWriteBuffer[16] = {0};
uint8_t i2cTempReadBuffer[16] = {0};

volatile uint8_t writeDataLen = 0;
volatile uint8_t readDataLen = 0;


typedef void (*I2C_FUNC)(uint32_t u32Status);

static I2C_FUNC s_I2C0HandlerFn = NULL;

/*---------------------------------------------------------------------------------------------------------*/
/* I2C End                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* Timer function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/

/*
 * unit: ms
 */
volatile uint32_t timerCount = 0;

/*
 * last while loop run time
 */
uint32_t lastRunTimerCount = 0;

/*
 * last light display time
 */
uint32_t lastFrameTimerCount = 0;

/*
 * last adc scan time
 */
uint32_t lastAdcScanTimerCount = 0;

/*
 * debounce time
 */
uint32_t debounceTimerCount = 0;

/*
 * read panel record time in ms
 */
uint32_t readPanelTimerCount = 0;

/**
 * @brief       Timer0 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The Timer0 default IRQ, declared in startup_M051Series.s.
 */
void TMR0_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER0) == 1)
    {
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER0);

        timerCount+=2;
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Timer End                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal RC 22.1184MHz clock */
    CLK->PWRCON |= CLK_PWRCON_OSC22M_EN_Msk;

    /* Waiting for Internal RC clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_OSC22M_STB_Msk));

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLK_S_Msk)) | CLK_CLKSEL0_HCLK_S_HIRC;
    CLK->CLKDIV = (CLK->CLKDIV & (~CLK_CLKDIV_HCLK_N_Msk)) | CLK_CLKDIV_HCLK(1);
		
// -------PWM-------
		CLK->CLKDIV &= ~CLK_CLKDIV_HCLK_N_Msk;
// -------PWM-------end
	
    /* Set PLL to Power down mode and HW will also clear PLL_STB bit in CLKSTATUS register */
    CLK->PLLCON |= CLK_PLLCON_PD_Msk;    
    
    /* Enable external XTAL 12MHz clock */
    CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk;
		
// -------PWM-------
		CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk | CLK_PWRCON_OSC22M_EN_Msk;
// -------PWM-------end

// -------Timer-------
		/* Enable external 12 MHz XTAL, IRC10K */
    CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk | CLK_PWRCON_OSC10K_EN_Msk;
// -------Timer-------end
		
    /* Waiting for external XTAL clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_XTL12M_STB_Msk));
		
		

    /* Set core clock as PLL_CLOCK from PLL */
    CLK->PLLCON = PLLCON_SETTING;
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk));
		
// -------PWM-------
		while(!(CLK->CLKSTATUS & (CLK_CLKSTATUS_PLL_STB_Msk | CLK_CLKSTATUS_XTL12M_STB_Msk | CLK_CLKSTATUS_OSC22M_STB_Msk)));
// -------PWM-------end
		
		
// -------Timer-------
    /* Waiting for clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk));
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_XTL12M_STB_Msk));
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_IRC10K_STB_Msk));
// -------Timer-------end
		
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLK_S_Msk)) | CLK_CLKSEL0_HCLK_S_PLL;

// -------PWM-------
		/* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    CLK->CLKSEL0 = CLK_CLKSEL0_STCLK_S_HCLK_DIV2 | CLK_CLKSEL0_HCLK_S_PLL;

    /* Enable IP clock */
    CLK->APBCLK = CLK_APBCLK_UART0_EN_Msk | CLK_APBCLK_PWM01_EN_Msk | CLK_APBCLK_PWM23_EN_Msk | CLK_APBCLK_PWM45_EN_Msk | CLK_APBCLK_PWM67_EN_Msk;
    /* IP clock source */
    CLK->CLKSEL1 = CLK_CLKSEL1_UART_S_PLL;
    /* IP clock source */
    CLK->CLKSEL1 = CLK_CLKSEL1_PWM01_S_HXT | CLK_CLKSEL1_PWM23_S_HXT;
		CLK->CLKSEL2 = CLK_CLKSEL2_PWM45_S_HXT | CLK_CLKSEL2_PWM67_S_HXT;

    /* Reset PWMA channel0~channel3 */
    SYS->IPRSTC2 = SYS_IPRSTC2_PWM03_RST_Msk;
    SYS->IPRSTC2 = 0;
		
		SYS->IPRSTC2 = SYS_IPRSTC2_PWM47_RST_Msk;
    SYS->IPRSTC2 = 0;
// -------PWM-------end

// -------Timer-------
		/* Enable peripheral clock */
    CLK->APBCLK |= CLK_APBCLK_TMR0_EN_Msk;
		
		/* Peripheral clock source */
    CLK->CLKSEL1 |= CLK_CLKSEL1_UART_S_PLL | CLK_CLKSEL1_TMR0_S_LIRC;
		
// -------Timer-------end

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate();
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For CLK_SysTickDelay()

    /* Enable UART module clock */
    CLK->APBCLK |= CLK_APBCLK_UART0_EN_Msk;

// -------ADC-------
		/* Enable ADC module clock */
    CLK->APBCLK |= CLK_APBCLK_ADC_EN_Msk ;
// -------ADC-------end

// -------I2C-------
		/* Enable I2C module clock */
    CLK->APBCLK |= CLK_APBCLK_I2C0_EN_Msk ;
// -------I2C-------end


    /* Select UART module clock source */
    CLK->CLKSEL1 = (CLK->CLKSEL1 & (~CLK_CLKSEL1_UART_S_Msk)) | CLK_CLKSEL1_UART_S_PLL;

// -------ADC-------
    /* Select ADC module clock source */
    CLK->CLKSEL1 &= ~CLK_CLKSEL1_ADC_S_Msk ;
    CLK->CLKSEL1 |= CLK_CLKSEL1_ADC_S_HIRC ;
		
		/* ADC clock source is 22.1184MHz, set divider to 7, ADC clock is 22.1184/7 MHz */
    CLK->CLKDIV  = (CLK->CLKDIV & ~CLK_CLKDIV_ADC_N_Msk) | (((7) - 1) << CLK_CLKDIV_ADC_N_Pos);
// -------ADC-------end

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set P3 multi-function pins for UART0 RXD and TXD */
    SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk);
    SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0);
		
// -------PWM-------
		/* Set P2 multi-function pins for PWMB Channel0~3  */
    SYS->P2_MFP &= ~(SYS_MFP_P20_Msk | SYS_MFP_P21_Msk | SYS_MFP_P22_Msk | SYS_MFP_P23_Msk | SYS_MFP_P24_Msk | SYS_MFP_P25_Msk | SYS_MFP_P26_Msk | SYS_MFP_P27_Msk);
    SYS->P2_MFP |= SYS_MFP_P20_PWM0 | SYS_MFP_P21_PWM1 | SYS_MFP_P22_PWM2 | SYS_MFP_P23_PWM3 | SYS_MFP_P24_PWM4 | SYS_MFP_P25_PWM5 | SYS_MFP_P26_PWM6 | SYS_MFP_P27_PWM7;
// -------PWM-------end
		
// -------ADC-------
		/* Configure the P1.0 - P1.3 ADC analog input pins */
    SYS->P1_MFP &= ~(SYS_MFP_P10_Msk | SYS_MFP_P11_Msk | SYS_MFP_P12_Msk); //| SYS_MFP_P13_Msk);
    SYS->P1_MFP |= SYS_MFP_P10_AIN0 | SYS_MFP_P11_AIN1 | SYS_MFP_P12_AIN2; //| SYS_MFP_P13_AIN3 ;
// -------ADC-------end
		
// -------I2C-------
		/* Configure the SDA0 & SCL0 of I2C0 pins */
    SYS->P3_MFP &= ~(SYS_MFP_P34_Msk | SYS_MFP_P35_Msk);
    SYS->P3_MFP |= (SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0);
// -------I2C-------end
}

void I2C0_SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal RC 22.1184MHz clock */
    CLK->PWRCON |= CLK_PWRCON_OSC22M_EN_Msk;

    /* Waiting for Internal RC clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_OSC22M_STB_Msk));

    /* Switch HCLK clock source to Internal RC */
    CLK->CLKSEL0 &= ~CLK_CLKSEL0_HCLK_S_Msk;
    CLK->CLKSEL0 |= CLK_CLKSEL0_HCLK_S_HIRC;

    /* Enable external XTAL 12MHz clock */
    CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk;

    /* Waiting for external XTAL clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_XTL12M_STB_Msk));

    /* Set core clock as PLL_CLOCK from PLL */
    CLK->PLLCON = PLLCON_SETTING;
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk));
    CLK->CLKSEL0 &= (~CLK_CLKSEL0_HCLK_S_Msk);
    CLK->CLKSEL0 |= CLK_CLKSEL0_HCLK_S_PLL;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate();
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For SYS_SysTickDelay()

    /* Enable UART module clock */
    CLK->APBCLK |= CLK_APBCLK_UART0_EN_Msk | CLK_APBCLK_I2C0_EN_Msk;

    /* Select UART module clock source */
    CLK->CLKSEL1 &= ~CLK_CLKSEL1_UART_S_Msk;
    CLK->CLKSEL1 |= CLK_CLKSEL1_UART_S_HXT;

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set P3 multi-function pins for UART0 RXD and TXD */
    SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk);
    SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0);

    /* Configure the SDA0 & SCL0 of I2C0 pins */
    SYS->P3_MFP &= ~(SYS_MFP_P34_Msk | SYS_MFP_P35_Msk);
    SYS->P3_MFP |= (SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0);
}

void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 */
    SYS->IPRSTC2 |=  SYS_IPRSTC2_UART0_RST_Msk;
    SYS->IPRSTC2 &= ~SYS_IPRSTC2_UART0_RST_Msk;

    /* Configure UART0 and set UART0 Baudrate */
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(PLL_CLOCK, 115200);
    UART0->LCR = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;
}

void setPwm(){
	
		printf("setPWM\n");
	
		P2->PMD = (P2->PMD & (~GPIO_PMD_PMD0_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD0_Pos);
    P2->PMD = (P2->PMD & (~GPIO_PMD_PMD1_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD1_Pos);
    P2->PMD = (P2->PMD & (~GPIO_PMD_PMD2_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD2_Pos);
    P2->PMD = (P2->PMD & (~GPIO_PMD_PMD3_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD3_Pos);
    P2->PMD = (P2->PMD & (~GPIO_PMD_PMD4_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD4_Pos);
    P2->PMD = (P2->PMD & (~GPIO_PMD_PMD5_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD5_Pos);
    P2->PMD = (P2->PMD & (~GPIO_PMD_PMD6_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD6_Pos);
    P2->PMD = (P2->PMD & (~GPIO_PMD_PMD7_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD7_Pos);
	
		/*Set Pwm mode*/
		PWMA->PCR |= PWM_PCR_CH0MOD_Msk;
		PWMA->PCR |= PWM_PCR_CH1MOD_Msk;
		PWMA->PCR |= PWM_PCR_CH2MOD_Msk;
		PWMA->PCR |= PWM_PCR_CH3MOD_Msk;
	
		PWMB->PCR |= PWM_PCR_CH0MOD_Msk;
		PWMB->PCR |= PWM_PCR_CH1MOD_Msk;
		PWMB->PCR |= PWM_PCR_CH2MOD_Msk;
		PWMB->PCR |= PWM_PCR_CH3MOD_Msk;

		/*Set PWM Timer clock prescaler*/
		PWM_SET_PRESCALER(PWMA, PWM_CH0, 0x01); // Divided by 2
		PWM_SET_PRESCALER(PWMA, PWM_CH1, 0x01); // Divided by 2
		PWM_SET_PRESCALER(PWMA, PWM_CH2, 0x01); // Divided by 2
		PWM_SET_PRESCALER(PWMA, PWM_CH3, 0x01); // Divided by 2
	
		PWM_SET_PRESCALER(PWMB, PWM_CH0, 0x02); // Divided by 2
		PWM_SET_PRESCALER(PWMB, PWM_CH1, 0x02); // Divided by 2
		PWM_SET_PRESCALER(PWMB, PWM_CH2, 0x02); // Divided by 2
		PWM_SET_PRESCALER(PWMB, PWM_CH3, 0x02); // Divided by 2
	
	
		/*Set PWM Timer clock divider select*/
		PWM_SET_DIVIDER(PWMA, PWM_CH0, PWM_CLK_DIV_16);
		PWM_SET_DIVIDER(PWMA, PWM_CH1, PWM_CLK_DIV_16);
		PWM_SET_DIVIDER(PWMA, PWM_CH2, PWM_CLK_DIV_16);
		PWM_SET_DIVIDER(PWMA, PWM_CH3, PWM_CLK_DIV_16);

		PWM_SET_DIVIDER(PWMB, PWM_CH0, PWM_CLK_DIV_16);
		PWM_SET_DIVIDER(PWMB, PWM_CH1, PWM_CLK_DIV_16);
		PWM_SET_DIVIDER(PWMB, PWM_CH2, PWM_CLK_DIV_16);
		PWM_SET_DIVIDER(PWMB, PWM_CH3, PWM_CLK_DIV_16);
		
		/*Set PWM Timer duty*/
		//PWMA->CMR0 = g_au16ScaleCmr[(u8Item - '1')];
		PWMA->CMR0 = 0x0;
		PWMA->CMR1 = 0x0;
		PWMA->CMR2 = 0x0;
		PWMA->CMR3 = 0x0;
		
		PWMB->CMR0 = 0x0;
		PWMB->CMR1 = 0x0;
		PWMB->CMR2 = 0x0;
		PWMB->CMR3 = 0x0;
	
		/*Set PWM Timer period*/
		//PWMA->CNR0 = g_au16ScaleCnr[(u8Item - '1')];
		PWMA->CNR0 = 0x1000;
		PWMA->CNR1 = 0x1000;
		PWMA->CNR2 = 0x1000;
		PWMA->CNR3 = 0x1000;
											
		PWMB->CNR0 = 0x1000;
		PWMB->CNR1 = 0x1000;
		PWMB->CNR2 = 0x1000;
		PWMB->CNR3 = 0x1000;

		/* Enable PWM Output pin */
		PWMA->POE |= PWM_POE_PWM0_Msk;
		PWMA->POE |= PWM_POE_PWM1_Msk;
		PWMA->POE |= PWM_POE_PWM2_Msk;
		PWMA->POE |= PWM_POE_PWM3_Msk;
		
		PWMB->POE |= PWM_POE_PWM0_Msk;
		PWMB->POE |= PWM_POE_PWM1_Msk;
		PWMB->POE |= PWM_POE_PWM2_Msk;
		PWMB->POE |= PWM_POE_PWM3_Msk;

		/* Enable Timer period Interrupt */
		//PWMA->PIER |= PWM_PIER_PWMIE0_Msk;
		//PWMA->PIER |= PWM_PIER_PWMIE1_Msk;
		//PWMA->PIER |= PWM_PIER_PWMIE2_Msk;
		//PWMA->PIER |= PWM_PIER_PWMIE3_Msk;

		/* Enable PWMB NVIC */
		//NVIC_EnableIRQ((IRQn_Type)(PWMA_IRQn));

		/* Enable PWM Timer */
		PWMA->PCR |= PWM_PCR_CH0EN_Msk;
		PWMA->PCR |= PWM_PCR_CH1EN_Msk;
		PWMA->PCR |= PWM_PCR_CH2EN_Msk;
		PWMA->PCR |= PWM_PCR_CH3EN_Msk;
		
		PWMB->PCR |= PWM_PCR_CH0EN_Msk;
		PWMB->PCR |= PWM_PCR_CH1EN_Msk;
		PWMB->PCR |= PWM_PCR_CH2EN_Msk;
		PWMB->PCR |= PWM_PCR_CH3EN_Msk;

		//while(g_u8PWMCount);

		/*--------------------------------------------------------------------------------------*/
		/* Stop PWM Timer (Recommended procedure method 2)                                      */
		/* Set PWM Timer counter as 0, When interrupt request happen, disable PWM Timer         */
		/* Set PWM Timer counter as 0 in Call back function                                     */
		/*--------------------------------------------------------------------------------------*/

		/* Disable PWMA NVIC */
		//NVIC_DisableIRQ((IRQn_Type)(PWMA_IRQn));

		/* Wait until PWMA channel 0 Timer Stop */
		//while(PWMA->PDR0 != 0);

		/* Disable the PWM Timer */
		//PWMA->PCR &= ~PWM_PCR_CH0EN_Msk;

		/* Disable PWM Output pin */
		//PWMA->POE &= ~PWM_POE_PWM0_Msk;
	
}

void setGpio(){
	
		/* Configure P1.2 as Output mode and P4.1 as Input mode then close it */
    //P1->PMD = (P1->PMD & (~GPIO_PMD_PMD2_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD2_Pos);
    //P4->PMD = (P4->PMD & (~GPIO_PMD_PMD1_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD1_Pos);
		
		// read panel and keyboard
		P0->PMD = (P0->PMD & (~GPIO_PMD_PMD0_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD0_Pos);
		P0->PMD = (P0->PMD & (~GPIO_PMD_PMD1_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD1_Pos);
		P0->PMD = (P0->PMD & (~GPIO_PMD_PMD2_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD2_Pos);
		P0->PMD = (P0->PMD & (~GPIO_PMD_PMD3_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD3_Pos);
		P0->PMD = (P0->PMD & (~GPIO_PMD_PMD4_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD4_Pos);
		P0->PMD = (P0->PMD & (~GPIO_PMD_PMD5_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD5_Pos);
		P0->PMD = (P0->PMD & (~GPIO_PMD_PMD6_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD6_Pos);
		P0->PMD = (P0->PMD & (~GPIO_PMD_PMD7_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD7_Pos);
		
		P3->PMD = (P3->PMD & (~GPIO_PMD_PMD2_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD2_Pos);
		P3->PMD = (P3->PMD & (~GPIO_PMD_PMD3_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD3_Pos);
		P3->PMD = (P3->PMD & (~GPIO_PMD_PMD6_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD6_Pos);
		P3->PMD = (P3->PMD & (~GPIO_PMD_PMD7_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD7_Pos);
		
		// indicator lights shift register
		P1->PMD = (P1->PMD & (~GPIO_PMD_PMD4_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD4_Pos);
		P1->PMD = (P1->PMD & (~GPIO_PMD_PMD5_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD5_Pos);
		P1->PMD = (P1->PMD & (~GPIO_PMD_PMD7_Msk)) | (GPIO_PMD_OUTPUT << GPIO_PMD_PMD7_Pos);
		
		// power and pedal
		P4->PMD = (P4->PMD & (~GPIO_PMD_PMD0_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD0_Pos);
		P1->PMD = (P1->PMD & (~GPIO_PMD_PMD3_Msk)) | (GPIO_PMD_INPUT << GPIO_PMD_PMD3_Pos);
	
}


/*---------------------------------------------------------------------------------------------------------*/
/*  I2C0 IRQ Handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void I2C0_IRQHandler(void)
{
    uint32_t u32Status;

    u32Status = I2C0->I2CSTATUS;

    if(I2C0->I2CTOC & I2C_I2CTOC_TIF_Msk)
    {
        /* Clear I2C0 Timeout Flag */
        I2C0->I2CTOC |= I2C_I2CTOC_TIF_Msk;
    }
    else
    {
        if(s_I2C0HandlerFn != NULL)
            s_I2C0HandlerFn(u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C TRx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_SlaveTRx(uint32_t u32Status)
{  	
		uint32_t i;
	
    if(u32Status == 0x60)                       /* Own SLA+W has been receive; ACK has been return */
    {
        //g_u8SlvDataLen = 0;
        I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_AA);
    }
    else if(u32Status == 0x80)                 /* Previously address with own SLA address
                                                   Data has been received; ACK has been returned*/
    {
				i2cTempReadBuffer[readDataLen] = (unsigned char)(I2C0->I2CDAT);
			
				readDataLen++;
			
				if((unsigned char) I2C_GET_DATA(I2C0) == 0)//'\0')	/* end of receive */
        {
					
					memcpy(i2cReadData[i2cReadDataEndPos], i2cTempReadBuffer, 16);
					memset(i2cTempReadBuffer, 0, 16);
					
					i2cReadDataEndPos++;
					if(i2cReadDataEndPos == 16)
						i2cReadDataEndPos = 0;
					
					// overflow so start pos move 1 forward
					if(i2cReadDataEndPos == i2cReadDataStartPos){
						i2cReadDataStartPos++;
						if(i2cReadDataStartPos == 16)
							i2cReadDataStartPos = 0;
					}
					
					readDataLen = 0;
				}
				
				
        I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_AA);
    }
    else if(u32Status == 0xA8)                  /* Own SLA+R has been receive; ACK has been return */
    {
				if(i2cWriteDataStartPos == i2cWriteDataEndPos){
					I2C0->I2CDAT = 0x0;
					//i2cWriteDataStartPos = 0;
					
					memset(i2cTempWriteBuffer, 0, 16);
					
				}
				else{
					//printf("Read!\n");
					memcpy(i2cTempWriteBuffer, i2cWriteData[i2cWriteDataStartPos], 16);
					
					I2C0->I2CDAT = 0x80;
					
					i2cWriteDataStartPos++;
					if(i2cWriteDataStartPos == 16)
						i2cWriteDataStartPos = 0;
					
				}
				
				I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_AA);
    }
		else if(u32Status == 0xB8)                  /* from datasheet to know 0xb8 is how to use TRM_ML51_Series_EN_Rev1.02.pdf */
    {
				if(writeDataLen == 16){
					I2C0->I2CDAT = 0x0;
					//I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI);
				}
				else{
					I2C0->I2CDAT = i2cTempWriteBuffer[writeDataLen++];
				}
			
				I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_AA);
    }
    else if(u32Status == 0xC0)                 /* Data byte or last data in I2CDAT has been transmitted
                                                   Not ACK has been received */
    {
				if(writeDataLen == 16){
					I2C0->I2CDAT = 0x0;
				}
				else{
					I2C0->I2CDAT = i2cTempWriteBuffer[writeDataLen++];
				}
				writeDataLen = 0;
        I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_AA);
    }
    else if(u32Status == 0x88)                 /* Previously addressed with own SLA address; NOT ACK has
                                                   been returned */
    {
        //g_u8SlvDataLen = 0;
        I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_AA);
    }
    else if(u32Status == 0xA0)                 /* A STOP or repeated START has been received while still
                                                   addressed as Slave/Receiver*/
    {
        //g_u8SlvDataLen = 0;
        I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_AA);
    }
		
    else
    {
        /* TO DO */
        printf("Status 0x%x is NOT processed\n", u32Status);
				I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_AA);
    }
}

void I2C0_Init(void)
{
    /* Reset I2C0 */
    SYS->IPRSTC2 |=  SYS_IPRSTC2_I2C0_RST_Msk;
    SYS->IPRSTC2 &= ~SYS_IPRSTC2_I2C0_RST_Msk;

    /* Enable I2C0 Controller */
    I2C0->I2CON |= I2C_I2CON_ENS1_Msk;

    /* I2C0 clock divider, I2C Bus Clock = PCLK / (4*125) = 100kHz */
    I2C0->I2CLK = 125 - 1;

    /* Get I2C0 Bus Clock */
    printf("I2C clock %d Hz\n", (SystemCoreClock / (((I2C0->I2CLK) + 1) << 2)));

    /* Set I2C0 4 Slave Addresses */
    /* Slave Address : 0x15 */
    I2C0->I2CADDR0 = (I2C0->I2CADDR0 & ~I2C_I2CADDR_I2CADDR_Msk) | (0x15 << I2C_I2CADDR_I2CADDR_Pos);
    /* Slave Address : 0x35 */
    I2C0->I2CADDR1 = (I2C0->I2CADDR1 & ~I2C_I2CADDR_I2CADDR_Msk) | (0x35 << I2C_I2CADDR_I2CADDR_Pos);
    /* Slave Address : 0x55 */
    I2C0->I2CADDR2 = (I2C0->I2CADDR2 & ~I2C_I2CADDR_I2CADDR_Msk) | (0x55 << I2C_I2CADDR_I2CADDR_Pos);
    /* Slave Address : 0x75 */
    I2C0->I2CADDR3 = (I2C0->I2CADDR3 & ~I2C_I2CADDR_I2CADDR_Msk) | (0x75 << I2C_I2CADDR_I2CADDR_Pos);

    /* Set I2C0 4 Slave Addresses Mask Bits*/
    /* Slave Address Mask Bits: 0x01 */
    I2C0->I2CADM0 = (I2C0->I2CADM0 & ~I2C_I2CADM_I2CADM_Msk) | (0x01 << I2C_I2CADM_I2CADM_Pos);
    /* Slave Address Mask Bits: 0x04 */
    I2C0->I2CADM1 = (I2C0->I2CADM1 & ~I2C_I2CADM_I2CADM_Msk) | (0x04 << I2C_I2CADM_I2CADM_Pos);
    /* Slave Address Mask Bits: 0x01 */
    I2C0->I2CADM2 = (I2C0->I2CADM2 & ~I2C_I2CADM_I2CADM_Msk) | (0x01 << I2C_I2CADM_I2CADM_Pos);
    /* Slave Address Mask Bits: 0x04 */
    I2C0->I2CADM3 = (I2C0->I2CADM3 & ~I2C_I2CADM_I2CADM_Msk) | (0x04 << I2C_I2CADM_I2CADM_Pos);

    /* Enable I2C0 interrupt and set corresponding NVIC bit */
    I2C0->I2CON |= I2C_I2CON_EI_Msk;
    NVIC_EnableIRQ(I2C0_IRQn);
}

void I2C0_Close(void)
{
    /* Disable I2C0 interrupt and clear corresponding NVIC bit */
    I2C0->I2CON &= ~I2C_I2CON_EI_Msk;
    NVIC_DisableIRQ(I2C0_IRQn);

    /* Disable I2C0 and close I2C0 clock */
    I2C0->I2CON &= ~I2C_I2CON_ENS1_Msk;
    CLK->APBCLK &= ~CLK_APBCLK_I2C0_EN_Msk;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: AdcContScanModeTest                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*   ADC continuous scan mode test.                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void AdcContScanModeTest()
{
    uint32_t u32ChannelCount;
    int32_t  i32ConversionData;
		double conversedData;
		char tempCommand[16];
		char value[4];

    //printf("\n\nConversion rate: %d samples/second\n", ADC_GetConversionRate());
    //printf("\n");
    //printf("+----------------------------------------------------------------------+\n");
    //printf("|                 ADC continuous scan mode sample code                 |\n");
    //printf("+----------------------------------------------------------------------+\n");

    //printf("\nIn this test, software will get 2 cycles of conversion result from the specified channels.\n");

		/* Set the ADC operation mode as continuous scan, input mode as single-end and enable the ADC converter */
		ADC->ADCR = (ADC_ADCR_ADMD_CONTINUOUS | ADC_ADCR_DIFFEN_SINGLE_END | ADC_ADCR_ADEN_CONVERTER_ENABLE);
		/* Enable analog input channel 0, 1, 2 , without 3, which is 0111 */
		ADC->ADCHER |= ((ADC->ADCHER & ~ADC_ADCHER_CHEN_Msk) | (0x7));

		/* clear the A/D interrupt flag for safe */
		ADC->ADSR = ADC_ADSR_ADF_Msk;

		/* start A/D conversion */
		ADC->ADCR |= ADC_ADCR_ADST_Msk;
		
		///* Wait conversion done */
		//while(!((ADC->ADSR & ADC_ADSR_ADF_Msk) >> ADC_ADSR_ADF_Pos));
		//
		///* Clear the ADC interrupt flag */
		//ADC->ADSR = ADC_ADSR_ADF_Msk;
		//
		//for(u32ChannelCount = 0; u32ChannelCount < 2; u32ChannelCount++)
		//{
		//		i32ConversionData = (ADC->ADDR[(u32ChannelCount)] & ADC_ADDR_RSLT_Msk) >> ADC_ADDR_RSLT_Pos;
		//		
		//		conversedData = log((double)(3710 - i32ConversionData)) / log(1.04375021467);//(max3720)
		//		printf("%.3f ", conversedData);
		//}

		/* Wait conversion done */
		while(!((ADC->ADSR & ADC_ADSR_ADF_Msk) >> ADC_ADSR_ADF_Pos));

		/* Stop A/D conversion */
		ADC->ADCR &= ~ADC_ADCR_ADST_Msk;
		
		for(u32ChannelCount = 0; u32ChannelCount < 2; u32ChannelCount++)
		{
				i32ConversionData = (ADC->ADDR[(u32ChannelCount)] & ADC_ADDR_RSLT_Msk) >> ADC_ADDR_RSLT_Pos;
				
				conversedData = log((double)(3710 - i32ConversionData)) / log(1.04375021467);//(max3720)
				sliderDataAccumulation[u32ChannelCount] += conversedData;
				//printf("%.3f ", conversedData);
		}
		//printf("\n ");
		
		/* Clear the ADC interrupt flag */
		ADC->ADSR = ADC_ADSR_ADF_Msk;

    adcScanCount++;
		if(adcScanCount >= MaxAdcScanTimes){
				if((int)(sliderDataAccumulation[0] / MaxAdcScanTimes) > sliderDataAverage[0] + 1 || 
					 (int)(sliderDataAccumulation[0] / MaxAdcScanTimes) < sliderDataAverage[0] - 1){
						sliderDataAverage[0] = (int)(sliderDataAccumulation[0] / MaxAdcScanTimes);
						printf("slider 1:%d\n", sliderDataAverage[0]);
						 
						memset(tempCommand, 0x0, 16);
						sprintf(tempCommand, "1030,");
						sprintf(value, "%03d", sliderDataAverage[0] - 90 < 0 ? 0 : sliderDataAverage[0] - 90);
						strncat(tempCommand, value, 3);
						memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
						i2cWriteDataEndPos++;
						if(i2cWriteDataEndPos == 16)
								i2cWriteDataEndPos = 0;
				}
				if((int)(sliderDataAccumulation[1] / MaxAdcScanTimes) > sliderDataAverage[1] + 1 ||
					 (int)(sliderDataAccumulation[1] / MaxAdcScanTimes) < sliderDataAverage[1] - 1){
						sliderDataAverage[1] = (int)(sliderDataAccumulation[1] / MaxAdcScanTimes);
						printf("slider 2:%d\n", sliderDataAverage[1]);
						
						memset(tempCommand, 0x0, 16);
						sprintf(tempCommand, "1031,");
						sprintf(value, "%03d", sliderDataAverage[1] - 90 < 0 ? 0 : sliderDataAverage[1] - 90);
						strncat(tempCommand, value, 3);
						memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
						i2cWriteDataEndPos++;
						if(i2cWriteDataEndPos == 16)
								i2cWriteDataEndPos = 0;
				}
				adcScanCount = 0;
				sliderDataAccumulation[0] = 0;
				sliderDataAccumulation[1] = 0;
		}
    
}

void setI2c(){
	
		uint8_t i, j;
		/* Init I2C0 */
    I2C0_Init();

    /* I2C enter no address SLV mode */
    I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_AA);
		
		printf("+-------------------------------------------------------+\n");
    printf("|  M05xx I2C Driver Sample Code(Slave) for access Slave |\n");
    printf("|                                                       |\n");
    printf("| I2C Master (I2C0) <---> I2C Slave(I2C0)               |\n");
    printf("+-------------------------------------------------------+\n");

    printf("Configure I2C0 as a slave.\n");
    printf("The I/O connection for I2C0:\n");
    printf("I2C0_SDA(P3.4), I2C0_SCL(P3.5)\n");
		
		/* I2C function to Slave receive/transmit data */
    s_I2C0HandlerFn = I2C_SlaveTRx;
		
		for(i = 0; i < 16; i++){
			for(j = 0; j < 16; j++){
				i2cWriteData[i][j] = 0;
				i2cReadData[i][j] = 0;
			}
		}
}

void setTimer(){
		printf("+-------------------------------------------------+\n");
    printf("|    Timer0 Power-down and Wake-up Sample Code    |\n");
    printf("+-------------------------------------------------+\n\n");

    printf("# Timer Settings:\n");
    printf("  Timer0: Clock source is LIRC(10 kHz); Toggle-output mode and frequency is 2 Hz; Enable interrupt and wake-up.\n");
    printf("# System will enter to Power-down mode while Timer0 interrupt count is reaching 3;\n");
    printf("  And be waken-up while Timer0 interrupt count is reaching 4.\n\n");
	
		/* Open Timer0 frequency to 2 Hz in toggle-output mode */
		TIMER0->TCMPR = __LIRC / 10000UL;	// 10khz
		//TIMER0->TCMPR = __LIRC / 5000UL;	// 10khz ,don't know why the speed is two times slower
		TIMER0->TCSR = TIMER_TOGGLE_MODE | TIMER_TCSR_IE_Msk | TIMER_TCSR_WAKE_EN_Msk;
		TIMER0->TCSR = TIMER_TCSR_IE_Msk | TIMER_PERIODIC_MODE;
	
		/* Enable Timer0 NVIC */
    NVIC_EnableIRQ(TMR0_IRQn);

    /* Start Timer0 counting */
    TIMER_Start(TIMER0);
}

void SetPin(uint8_t port, uint8_t pin, uint8_t value){
	GPIO_PIN_ADDR(port, pin) = value;
}

uint32_t GetPin(uint8_t port, uint8_t pin){
	return GPIO_PIN_ADDR(port, pin);
}

void ReadPanel(){
	
		/*
		*	P0.0~P0.7 is KB in
		* P3.2 KB out A
		* P3.3 KB out B
		* P3.6 KB out C
		* P3.7 KB out Enable
		*/
	
		uint8_t i, j, k, l;
	
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
			
		for(i = 0; i < 16; i++){//16; i++){
			
			if(i == outputToPass){
					if(outputToPass == 14){
							i++;
							outputToPass = 15;
					}
					else if(outputToPass == 15){
							outputToPass = 14;
							continue;
					}
			}
			
			if(i & 0x8)
				SetPin(3, 7, 1);
			else
				SetPin(3, 7, 0);
			
			if(i & 0x4)
				SetPin(3, 6, 1);
			else
				SetPin(3, 6, 0);
			
			if(i & 0x2)
				SetPin(3, 3, 1);
			else
				SetPin(3, 3, 0);
			
			if(i & 0x1)
				SetPin(3, 2, 1);
			else
				SetPin(3, 2, 0);
			
			for(j = 0; j < 8; j++){
				//if(i > 13 && (j == 4 || j == 5)){
				//	for(k = 0; k < 255; k++){
				//		for(l = 0; l < 16; l++){
				//				printf(" ");
				//		}
				//	}
				//}
				//
				////printf(" ");
				//if(GetPin(0, j) == 0 && i > 13 && (j == 4 || j == 5)){
				//		printf("get input i:%d j:%d\n", i, j);
				//		//printf("\n");
				//}
				
				if(i == 8 || i ==10 || i == 12){	// press first pad
					
						pressedNum = (i-8)/2*8+j;
						if(GetPin(0, j) == 0){
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
						else if(pressedKey[pressedNum] == 2 || // means the second pad was pressed
										pressedKey[pressedNum] == 3){  // means after release second pad
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
						if(GetPin(0, j) == 0){
								pressedNum = (i-8)/2*8+j;
								if(pressedKey[pressedNum] == 1){
										//printf("read input [%d] %d %d at %ds\n", pressedNum, i, j, timerCount);
										//velocity = tan(((double)(200 - (timerCount - pressedKeyTime[pressedNum])))/200.0*3.14159265358979323846/2.0)*2;
										velocity = (timerCount - pressedKeyTime[pressedNum]) / 2;
										//printf("press [%d] %d %d with speed %ds, velocity %f \n", pressedNum, i, j, timerCount - pressedKeyTime[pressedNum], 
										//		velocity);
										pressedKey[pressedNum] = 2;
									
										memset(tempCommand, 0x0, 16);
										sprintf(value, "%03d", 127 + 23 - pressedNum);
										strncpy(tempCommand, value, 3);
										strncat(tempCommand, ",", 1);
										// 127 = 1.02476732964 exp(200-t) big power no difference
										// sprintf(value, "%03d", (int)pow(1.02476732964, (200 - (timerCount - pressedKeyTime[pressedNum])) * 1));
										// 127 = 1.01230792234 exp((200-t)*2) 
										// sprintf(value, "%03d", (int)pow(1.01230792234, (200 - (timerCount - pressedKeyTime[pressedNum])) * 2));
										// 127 = 1.0081854132 exp((200-t)*3) 
										// sprintf(value, "%03d", (int)pow(1.0081854132, (200 - (timerCount - pressedKeyTime[pressedNum])) * 3));
										// 127 = 1.00613514119 exp((200-t)*4) 
										// sprintf(value, "%03d", (int)pow(1.00613514119, (200 - (timerCount - pressedKeyTime[pressedNum])) * 4));
										// 127 = 1.00490510912 exp((200-t)*5) 
										// sprintf(value, "%03d", (int)pow(1.00490510912, (200 - (timerCount - pressedKeyTime[pressedNum])) * 5));
										// 127 = 1.0024495544 exp((200-t)*10) 
										// sprintf(value, "%03d", (int)pow(1.0024495544, (200 - (timerCount - pressedKeyTime[pressedNum])) * 10));
										// 127 = 1.00122402808 exp((200-t)*20) 
										// sprintf(value, "%03d", (int)pow(1.00122402808, (200 - (timerCount - pressedKeyTime[pressedNum])) * 20));
										// 127 = 1.00012357123 exp((200-t)^2) 
										// sprintf(value, "%03d", (int)pow(1.00012357123, pow(200 - (timerCount - pressedKeyTime[pressedNum]), 2)));
										// 127 = tan((200-t)/200*(pi/2)*2)
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
#ifdef VK49KEY
				/*			i		j					i		j
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
										//velocity = tan(((double)(200 - (timerCount - pressedKeyTime[pressedNum])))/200.0*3.14159265358979323846/2.0)*2;
										velocity = (timerCount - pressedKeyTime[pressedNum]) / 2;
										//printf("press [%d] %d %d with speed %ds, velocity %f \n", pressedNum, i, j, timerCount - pressedKeyTime[pressedNum], 
										//		velocity);
										pressedKey[pressedNum] = 2;
									
										memset(tempCommand, 0x0, 16);
										sprintf(value, "%03d", 127 + pressedNum);
										strncpy(tempCommand, value, 3);
										strncat(tempCommand, ",", 1);
										// 127 = 1.02476732964 exp(200-t) big power no difference
										// sprintf(value, "%03d", (int)pow(1.02476732964, (200 - (timerCount - pressedKeyTime[pressedNum])) * 1));
										// 127 = 1.01230792234 exp((200-t)*2) 
										// sprintf(value, "%03d", (int)pow(1.01230792234, (200 - (timerCount - pressedKeyTime[pressedNum])) * 2));
										// 127 = 1.0081854132 exp((200-t)*3) 
										// sprintf(value, "%03d", (int)pow(1.0081854132, (200 - (timerCount - pressedKeyTime[pressedNum])) * 3));
										// 127 = 1.00613514119 exp((200-t)*4) 
										// sprintf(value, "%03d", (int)pow(1.00613514119, (200 - (timerCount - pressedKeyTime[pressedNum])) * 4));
										// 127 = 1.00490510912 exp((200-t)*5) 
										// sprintf(value, "%03d", (int)pow(1.00490510912, (200 - (timerCount - pressedKeyTime[pressedNum])) * 5));
										// 127 = 1.0024495544 exp((200-t)*10) 
										// sprintf(value, "%03d", (int)pow(1.0024495544, (200 - (timerCount - pressedKeyTime[pressedNum])) * 10));
										// 127 = 1.00122402808 exp((200-t)*20) 
										// sprintf(value, "%03d", (int)pow(1.00122402808, (200 - (timerCount - pressedKeyTime[pressedNum])) * 20));
										// 127 = 1.00012357123 exp((200-t)^2) 
										// sprintf(value, "%03d", (int)pow(1.00012357123, pow(200 - (timerCount - pressedKeyTime[pressedNum]), 2)));
										// 127 = tan((200-t)/200*(pi/2)*2)
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
					//continue;
					//if(timerCount / 10 > readPanelTimerCount || timerCount / 10 < readPanelTimerCount){
					//		readPanelTimerCount = timerCount / 10;
					//}
					//else{
					//		continue;
					//}
					
					if(GetPin(0, j) == 0){
							if(i ==14 && (j == 4 || j == 5)){
								
									switch(sectionKnobState){
									case 0:
											if(GetPin(0, 4) == 0 && GetPin(0, 5) != 0){
													sectionKnobState = 1;
													printf("section knob forward. %d\n", i2cWriteDataEndPos);
												
													memset(tempCommand, 0x0, 16);
													sprintf(tempCommand, "1020,1");
													memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
													i2cWriteDataEndPos++;
													if(i2cWriteDataEndPos == 16)
															i2cWriteDataEndPos = 0;
												
											}
											else if(GetPin(0, 5) == 0 && GetPin(0, 4) != 0){
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
											if(GetPin(0, 4) == 0 && GetPin(0, 5) == 0){
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
															if(GetPin(0, 4) == 0 && GetPin(0, 5) != 0){
																	speedKnobState = 1;
																	printf("speed knob forward. %d\n", i2cWriteDataEndPos);
																
																	memset(tempCommand, 0x0, 16);
																	sprintf(tempCommand, "1020,1");
																	memcpy(i2cWriteData[i2cWriteDataEndPos], tempCommand, 16);
																	i2cWriteDataEndPos++;
																	if(i2cWriteDataEndPos == 16)
																			i2cWriteDataEndPos = 0;
																
															}
															else if(GetPin(0, 5) == 0 && GetPin(0, 4) != 0){
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
															if(GetPin(0, 4) == 0 && GetPin(0, 5) == 0){
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
		// if last run has no speed knob state, then enable speed knob input
		//if(hasThisSpeedKnobState == 0 && isAfterDebounce == 1){
		//		printf("this run NOT has speed knob state.\n");
		//		hasLastSpeedKnobState = 0;
		//}
		if(hasThisSpeedKnobState == 0 && debounceTimerCount + 500 < timerCount){
				//printf("this run NOT has speed knob state.\n");
				hasLastSpeedKnobState = 0;
		}
		else if(isAfterDebounce == 1){
				//printf("this run has speed knob state.\n");
				hasLastSpeedKnobState = 1;
		}
		
		if(GetPin(4, 0) == 1){
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
		
		if(GetPin(1, 3) == 1){
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
	
		// https://ddddiy.blogspot.com/2014/02/74hct595n.html
		for(i = 0; i < 16; i++){
				LatchPin = 0;
				for(j = 15; j < 16; j--){
						ClockPin = 0;
						
						if(indicatorLights[j] == 1)
							DataPin = 1;
						else
							DataPin = 0;
						
						ClockPin = 1;
				}
				LatchPin = 1;
			
		}
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
	
		// https://ddddiy.blogspot.com/2014/02/74hct595n.html
		for(i = 0; i < 16; i++){
				LatchPin = 0;
				for(j = 15; j < 16; j--){
						ClockPin = 0;
						
						if(indicatorLights[j] == 1)
							DataPin = 1;
						else
							DataPin = 0;
						
						ClockPin = 1;
				}
				LatchPin = 1;
			
		}
}

/*
* position: 0~8, -1:turn 0ff
 */
void SetPwmLightRing(int position){
	
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
					brightnessArray[i] = 0xFF0;
				else
					brightnessArray[i] = 0x100;
		}
		
		PWMA->CMR0 = brightnessArray[7];
		PWMA->CMR1 = brightnessArray[6];
		PWMA->CMR2 = brightnessArray[5];
		PWMA->CMR3 = brightnessArray[4];
								
		PWMB->CMR0 = brightnessArray[3];
		PWMB->CMR1 = brightnessArray[2];
		PWMB->CMR2 = brightnessArray[1];
		PWMB->CMR3 = brightnessArray[0];
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
		
		PWMA->CMR0 = brightnessConverted;
		PWMA->CMR1 = brightnessConverted;
		PWMA->CMR2 = brightnessConverted;
		PWMA->CMR3 = brightnessConverted;
								 
		PWMB->CMR0 = brightnessConverted;
		PWMB->CMR1 = brightnessConverted;
		PWMB->CMR2 = brightnessConverted;
		PWMB->CMR3 = brightnessConverted;		
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
				SetPwmLightRing(0);
				return;
		}

		if(pwmLightEffectType == 2 || pwmLightEffectType == 4)
				return;
		
		if(pwmLightEffectType == 1){
			
				if(elapsedFrameTime < pwmTimeLengthLeft)
						pwmTimeLengthLeft -= elapsedFrameTime;
				else{
						pwmLightEffectType = 0;
						SetPwmLightRing(-1);
						return;
				}
				
				/* position = round(time left / total time * 8) */
				position = 8 - (int)(((float)pwmTimeLengthLeft) * 8.f / (float)pwmTimeLength + 1.0f);
				//printf("test float %f , %f , %f, %f\n", (float)pwmTimeLengthLeft ,((float)pwmTimeLengthLeft) * 8.f, ((float)pwmTimeLengthLeft) * 8.f / (float)pwmTimeLength, round(((float)pwmTimeLengthLeft) * 8.f / (float)pwmTimeLength));
				//printf("position %d, pwm time left %d\n", position, pwmTimeLengthLeft);
				if(position < 8){
						SetPwmLightRing(position);
				}
				else if(position == 8){
						SetPwmLightRing(7);
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
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    int32_t i32Err;
		int count = 0;
		int lightPos = 0;
		
		char arr[] = "LR,2.55";
		
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    //I2C0_SYS_Init();
		SYS_Init();
		
    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();
		/*
        To enable semihost, user must define "DEBUG_ENABLE_SEMIHOST" constant when build code with M051Series BSP.
        This sample code is used to show how to print message/getchar on IDE debug environment.
        It will echo all input character back on UART #1 of KEIL IDE.

        In KEIL MDK, user need to open "View->Serial Window->UART #1" windows in debug mode.
        In IAR Workbench, user need to open "View->Terminal I/O" in debug mode.

        NOTE1: HardFault_Handler handler is implemented in retarget.c.
        NOTE2: Semihost only works with Nuvoton NuLink ICE Dongle in debug mode.
        NOTE3: It does not print any message if Nuvoton NuLink ICE Dongle is not connected.
    */

	//while(1)printf("1\n");
	
    printf("\n\nCPU @ %d Hz\n", SystemCoreClock);
    printf("+-------------------------------------------------+\n");
    printf("|        All Panel Function Sample Code           |\n");
    printf("+-------------------------------------------------+\n\n");
		
		I2C0_Init();
		
		setPwm();
		
		setGpio();
		
		setI2c();

    setTimer();
		
		DecodeMessage(arr);
		
		
		/* --------------------main-----------------------*/
		
		// reset
		SetIndicatorLights(-1, 0);
		
		// fading light ring
		SetLightRingEffect(2, 3);
		
		while(1){
			
			//count++;
			//
			//if(timerCount % 10000 == 0){
			//	printf("time %d %d\n", timerCount / 10000, count);
			//	count = 0;
			//}
			
				
			ReadPanel();
			
			if(timerCount > lastAdcScanTimerCount + 100 || timerCount < lastAdcScanTimerCount){
					AdcContScanModeTest();
					lastAdcScanTimerCount = timerCount;
			}
			
			lastRunTimerCount = timerCount;
			if(timerCount == 0)
					debounceTimerCount = 0;
			
			/* 50 fps / update frame every 20 ms */
			if(timerCount > lastFrameTimerCount + 200/*200*/ || timerCount < lastFrameTimerCount){
					
				
					if(timerCount > lastFrameTimerCount)
							UpdateLightRingEffect((timerCount - lastFrameTimerCount) / 10);
					lastFrameTimerCount = timerCount;
				
					ProcessI2cMessage();
			}
		}
}

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
