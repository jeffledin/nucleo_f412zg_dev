
#include "stm32f4xx.h"
#include "defines.h"
#include "inits.h"
#include <string.h>

#define RNG_BASE 0x50060800
#define RNG_CR	 (*(unsigned int *)RNG_BASE)
#define RNG_SR	 (*(unsigned int *)(RNG_BASE + 0x04))
#define RNG_DR	 (*(unsigned int *)(RNG_BASE + 0x08))
#define DRDY	 0x00000001
			
void initRNG(void);
void transmitByte(unsigned char data);
void transmitBuffer(unsigned char *buffer, unsigned int bufferSize);
unsigned char receiveByte(void);
void generatePassword(unsigned int numChars);

int main(void)
{
	unsigned char message[] = "Generating random password...\n";
	RCC->CFGR = 0; // select HSI oscillator @ 16 MHz -- looks like the default was set to PLL (HSI x 4)
	for(int i = 0; i < 5000; i++); // allow oscillator to stabilize -- arbitrary delay
	initUSART3();
	initRNG();

	transmitBuffer(message, strlen(message));
	generatePassword(16);

	while(1)
	{

	};
}

void initRNG(void)
{
	RCC->AHB2ENR |= (1 << 6);
	RNG_CR |= (1 << 2); // enable RNG
	RNG_CR &= ~(1 << 3); // disable interrupts
}

void generatePassword(unsigned int numChars)
{
	unsigned int randomNumber;

	for(int i = 0; i < numChars; i++)
	{
		while(!(RNG_SR & DRDY)); // wait for data ready flag to be set
		if(((RNG_SR & (1 << 5)) == 0) && ((RNG_SR & (1 << 6)) == 0)) // are both error bits clear?
		{
			randomNumber = RNG_DR; // read data
		}
		randomNumber %= 94;
		transmitByte(randomNumber + 33);
	}
	transmitByte('\n');
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
