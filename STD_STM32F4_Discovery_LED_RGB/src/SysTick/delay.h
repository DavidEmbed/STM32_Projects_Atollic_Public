/*
 * delay.h
 *
 *  Created on: Feb 5, 2012
 *      Author: Marcel
 *
 *      Remember to modify the SysTick_Handler in stm32f4xx.h, writing
 *      inside Delay_TimingDecrement(); and include at top this header.
 *
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"


static __IO uint32_t timingDelay;
static __IO uint32_t delay_ticks_1us;
static __IO uint32_t delay_ticks_1ms;

void Delay_Init(void);
void Delay_TimingDecrement(void);
void Delay_ms(uint32_t nms);
void Delay_us(uint32_t nus);

#endif /* DELAY_H_ */
