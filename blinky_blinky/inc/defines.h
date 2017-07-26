/*
 * defines.h
 *
 *  Created on: Jul 22, 2017
 *      Author: ledin
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#define PB7_MASK	0x0080;
#define PB14_MASK	0x4000;
#define PB14 14
#define PB9 7
#define BLUE_LED 7
#define RED_LED 14


#define TIM6_UIF	(TIM6->SR & (1 << 0))

#endif /* DEFINES_H_ */
