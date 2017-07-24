/**
  ******************************************************************************
  * @file    main.c
  * @author  Jeff Ledin
  * @version V1.0
  * @date    22-July-2017
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "defines.h"

void initOSC(void);
void initLEDs(void);
void initTIM6(void);

int main(void)
{
	unsigned char count = 0;

	initOSC();
	initLEDs();
	initTIM6();

	while(1)
	{
	};
}

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

// TODO: Play around with timer interrupts
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
