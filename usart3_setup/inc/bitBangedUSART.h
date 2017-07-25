/*
 * bitBangedUSART.h
 *
 *  Created on: Jul 24, 2017
 *      Author: ledin
 */

#ifndef BITBANGEDUSART_H_
#define BITBANGEDUSART_H_

void initTIM7(unsigned int baudrate);
void initBitBangedUSART(unsigned int baudrate);
void transmitBitBangedByte(unsigned char data);
void transmitBitBangedBuffer(unsigned char *buffer, unsigned int bufferSize);
void sendStartBit(void);
void sendStopBit(void);
void waitTIM7(void);

#endif /* BITBANGEDUSART_H_ */
