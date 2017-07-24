/*
 * isr.c
 *
 *  Created on: Jul 22, 2017
 *      Author: ledin
 */

#include "stm32f4xx.h"
#include "defines.h"

#define TOGGLE_ALL_LEDS 0
#define TOGGLE_BLUE_LED 1
#define TURN_OFF_ALL_LEDS 2

void TIM6_IRQHandler()
{
	unsigned static char state = TOGGLE_ALL_LEDS;

	TIM6->SR &= ~(1 << 0); // turn off interrupt flag

	switch(state)
	{
		case TOGGLE_ALL_LEDS:
			GPIOB->ODR ^= ((1 << RED_LED) | (1 << BLUE_LED));
			state = TOGGLE_BLUE_LED;
			break;
		case TOGGLE_BLUE_LED:
			GPIOB->ODR ^= (1 << BLUE_LED);
			state = TOGGLE_ALL_LEDS;
			break;
		case TURN_OFF_ALL_LEDS:
			GPIOB->ODR = 0;
			state = TOGGLE_ALL_LEDS;
			break;
		default:
			break;
	}
}
