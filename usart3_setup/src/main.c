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
#include "bitBangedUSART.h"
#include <string.h>

void transmitByte(unsigned char data);
void transmitBuffer(unsigned char *buffer, unsigned int bufferSize);
unsigned char receiveByte(void);

int main(void)
{
	unsigned char buffer[] = "Hello from STM32F412ZG!\n";
	initOSC();
	initLEDs();
	//initUSART3();
	initBitBangedUSART(4800);
	initTIM6();

	while(1)
	{
		//transmitBuffer(buffer, strlen(buffer));
		transmitBitBangedBuffer(buffer, strlen(buffer));
		for(unsigned int i = 0; i < 100000; i++);
	}
}

void transmitBuffer(unsigned char *buffer, unsigned int bufferSize)
{
	for(unsigned char i = 0; i < bufferSize; i++)
	{
		transmitByte(buffer[i]);
	}
}

void transmitByte(unsigned char data)
{
	while(!USART3_TXE); // wait for flag to be set, denoting that DR is empty
	USART3->DR = data;
	GPIOB->ODR ^= (1 << 0); // toggle green LED to signal a write
}

unsigned char receiveByte(void)
{
	unsigned char receiveChar;
	while (!(USART3->SR & USART_SR_RXNE));
	receiveChar = ((int)(USART3->DR & 0xFF));
	return receiveChar;
}
