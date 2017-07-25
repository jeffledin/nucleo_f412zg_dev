/*
 * inits.c
 *
 *  Created on: Jul 24, 2017
 *      Author: ledin
 */

#include "stm32f4xx.h"
#include "defines.h"
#include <string.h>

void initOSC(void)
{
	RCC->CFGR = 0; // select HSI oscillator @ 16 MHz -- looks like the default was set to PLL (HSI x 4)
	for(int i = 0; i < 5000; i++); // allow oscillator to stabilize -- arbitrary delay
}

void initLEDs(void)
{
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // TODO: investigate this syntax

	RCC->AHB1ENR |= 0x2; // enable clock on Port B // THIS IS IMPORTANT
	GPIOB->MODER = 0x10004001; // set PB0, PB7, PB14 as output
	GPIOB->OTYPER = 0x00000000; // set all pins as push-pull (this is default anyways)
	GPIOB->ODR = 0x00004081; // set PB0, PB7, PB14 high
}

void initTIM6(void)
{
	// Counter should overflow every 1s
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // enable clock for TIM6; taken from bit definitions provided in stm32f4xx.h for register APB1ENR
	//RCC->APB1ENR |= (1 << 4); // equivalent to line above

	TIM6->PSC = 0xFFFF; // prescaler of 1:65536
	TIM6->ARR = 244; // period register
	TIM6->CNT = 0; // default of 0 for the counter
	TIM6->DIER |= (1 << 0); // enable interrupts
	TIM6->SR &= (0 << 0); // turn off interrupt flag
	TIM6->CR1 |= (1 << 0); // turn on counter
	NVIC_EnableIRQ(TIM6_IRQn); // enable the interrupt in the NVIC
}

void initUSART3(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // enable clock for GPIOD
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // enable clock for USART3

	/* Lower-level approach */
	GPIOD->MODER &= ~(0b11 << 16); // clear bits we want to set
	GPIOD->MODER |= (0b10 << 16); // PD8 is an alternative function pin
	GPIOD->AFR[1] |= GPIO_AF_USART3; // mux PD8 with USART3 peripheral

	USART3->BRR |= (104 << 4); // Baudrate = 9600 (see datasheet for calculations)
	USART3->CR1 |= USART_CR1_TE; // enable transmitter
	USART3->CR1 |= USART_CR1_RE; // enable receiver
	USART3->CR1 |= USART_CR1_UE; // enable USART

	/* Higher-level approach */
//	GPIO_InitTypeDef gpioConfig;
//	gpioConfig.GPIO_Pin = GPIO_Pin_8;
//	gpioConfig.GPIO_Mode = GPIO_Mode_AF;
//	gpioConfig.GPIO_Speed = GPIO_High_Speed;
//	gpioConfig.GPIO_OType = GPIO_OType_PP; // push pull
//	GPIOD->AFR[1] |= GPIO_AF_USART3; // configure PD8 as USART3 pin
//
//	GPIO_Init(GPIOD, &gpioConfig);
//
//	USART_InitTypeDef usartConfig;
//	usartConfig.USART_BaudRate = 9600;
//	usartConfig.USART_WordLength = USART_WordLength_8b;
//	usartConfig.USART_StopBits = USART_StopBits_1;
//	usartConfig.USART_Parity = USART_Parity_No;
//	usartConfig.USART_Mode = USART_Mode_Tx;
//	usartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_Cmd(USART3, ENABLE);
//	USART_Init(USART3, &usartConfig);
}
