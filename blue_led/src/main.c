/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
			
#define PB9_MASK	0x0080;
#define PB14_MASK	0x4000;

int main(void)
{
	// TODO: Figure out how to access individual bits, similar to MPLAB
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // TODO: investigate why this is required

	RCC->AHB1ENR |= 0x2; // enable clock on Port B // THIS IS IMPORTANT
	GPIOB->MODER = 0x10004000; // set PB7, PB14 as output
	GPIOB->OTYPER = 0x00000000; // set all pins as push-pull (this is default anyways)
	//GPIOB->BSRRL = 0x4000; // set PB14 high
	GPIOB->ODR = 0x00004080; // set PB7, PB14 high
//	GPIO_InitTypeDef gpio;
//
//	GPIO_StructInit(&gpio);
//	gpio.GPIO_Pin = GPIO_Pin_14;
//	gpio.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_Init(GPIOB, &gpio);
//
//	GPIO_SetBits(GPIOB, GPIO_Pin_14);

	unsigned char count = 0;

	while(1)
	{
		if(count == 0)
		{
			++count;
			GPIOB->ODR ^= PB14_MASK;
		}
		else if(count == 1)
		{
			--count;
			GPIOB->ODR ^= PB14_MASK;
			GPIOB->ODR ^= PB9_MASK;
		}
		for(unsigned int i = 0; i < 2000000; i++);
	}
}
