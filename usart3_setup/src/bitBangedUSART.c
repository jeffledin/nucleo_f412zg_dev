/*
 * bitBangedUSART.c
 *
 *  Created on: Jul 24, 2017
 *      Author: ledin
 */

#include "stm32f4xx.h"
#include "defines.h"
#include <string.h>

void initBitBangedUSART(unsigned int baudrate) // works well with baudrates at or below 9600 bps
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // enable clock for GPIOD

	initTIM7(baudrate);

	// init TX pin
	GPIO_InitTypeDef gpioConfig;
	gpioConfig.GPIO_Pin = GPIO_Pin_8;
	gpioConfig.GPIO_Mode = GPIO_Mode_OUT;
	gpioConfig.GPIO_Speed = GPIO_High_Speed;
	gpioConfig.GPIO_OType = GPIO_OType_PP; // push pull

	GPIO_Init(GPIOD, &gpioConfig);

	GPIOD->ODR |= (1 << 8); // set pin high for idle bus
}

void transmitBitBangedByte(unsigned char data)
{
	sendStartBit();

	// send out byte LSb first
	for(unsigned int dataByteMask = 0x01; dataByteMask <= 0x80; dataByteMask <<= 1) // scan through data byte
	{
		if(dataByteMask & data) // send a 1
		{
			GPIOD->ODR |= (1 << 8); // set pin high
		}
		else // send a 0
		{
			GPIOD->ODR &= ~(1 << 8); // set pin low
		}
		waitTIM7();
	}

	sendStopBit();
}

void transmitBitBangedBuffer(unsigned char *buffer, unsigned int bufferSize)
{
	for(unsigned int i = 0; i < bufferSize; i++)
	{
		transmitBitBangedByte(buffer[i]);
	}
}

void sendStartBit(void)
{
	GPIOD->ODR = ~(1 << 8); // set pin low
	waitTIM7();
}

void sendStopBit(void)
{
	GPIOD->ODR = (1 << 8); // set pin high
	waitTIM7();
}

void waitTIM7(void)
{
	TIM7->CR1 |= (1 << 0); // turn on counter
	while(!TIM7_UIF);
	TIM7->SR &= ~(1 << 0); // turn off interrupt flag
	TIM7->CR1 &= ~(1 << 0); // turn off counter
	TIM7->CNT = 0; // reset counter
}

void initTIM7(unsigned int baudrate)
{
	unsigned int periodRegister = (1 / (double) baudrate) / (1 / (double) OSC_FREQ);

	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; // enable clock for TIM7

	TIM7->PSC = 0; // prescaler of 1 for maximum resolution
	TIM7->ARR = periodRegister;
	TIM7->CNT = 0; // default of 0 for the counter
}

