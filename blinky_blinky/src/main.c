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
			
#define PB9_MASK	0x0080;
#define PB14_MASK	0x4000;

#define TIM6_UIF	(TIM6->SR & (1 << 0))

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
		if(count == 0)
		{
			++count;
			GPIOB->ODR ^= PB9_MASK;
		}
		else if(count == 1)
		{
			--count;
			GPIOB->ODR ^= PB14_MASK;
			GPIOB->ODR ^= PB9_MASK;
		}
		while(!TIM6_UIF); // wait for counter to overflow and flag to be set
		TIM6->SR &= (0 << 0); // reset TIM6 flag
	}
}

void initOSC(void)
{
	RCC->CFGR = 0; // select HSI oscillator @ 16 MHz -- looks like the default was set to PLL (HSI x 4)
	for(int i = 0; i < 5000; i++); // allow oscillator to stabilize -- arbitrary delay
}

void initLEDs(void)
{
	// TODO: Figure out how to access individual bits, similar to MPLAB
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // TODO: investigate why this is required

	RCC->AHB1ENR |= 0x2; // enable clock on Port B // THIS IS IMPORTANT
	GPIOB->MODER = 0x10004000; // set PB7, PB14 as output
	GPIOB->OTYPER = 0x00000000; // set all pins as push-pull (this is default anyways)
	GPIOB->ODR = 0x00004080; // set PB7, PB14 high

	//	GPIO_InitTypeDef gpio;
	//
	//	GPIO_StructInit(&gpio);
	//	gpio.GPIO_Pin = GPIO_Pin_14;
	//	gpio.GPIO_Mode = GPIO_Mode_OUT;
	//	GPIO_Init(GPIOB, &gpio);
	//
	//	GPIO_SetBits(GPIOB, GPIO_Pin_14);
}

void initTIM6(void)
{
	// Counter should overflow every 2s
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // enable clock for TIM6; taken from bit definitions provided in stm32f4xx.h for register APB1ENR
	//RCC->APB1ENR |= (1 << 4); // equivalent to line above

	TIM6->PSC = 0xFFFF; // prescaler of 1:65536
	TIM6->ARR = 488; // period register -- clock seems to be 4 times faster than expected
	TIM6->CNT = 0; // default of 0 for the counter
	TIM6->CR1 |= (1 << 0); // turn on counter
}
