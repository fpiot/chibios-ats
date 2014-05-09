
#include "ch.h"
#include "hal.h"

#ifndef _ATMEGA_TIMERS_H_
#define _ATMEGA_TIMERS_H_
uint8_t findBestPrescaler(uint16_t frequency, uint16_t *ratio ,uint8_t *clock_source,uint8_t n);

extern uint16_t ratio_base[];
extern uint8_t clock_source_base[];
extern uint16_t ratio_extended[];
extern uint8_t clock_source_extended[];

#define PRESCALER_SIZE_BASE 5
#define PRESCALER_SIZE_EXTENDED 7

#endif