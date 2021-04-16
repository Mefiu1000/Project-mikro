/*
 * ADC.h
 *
 * Created: 26.12.2020 14:16:34
 *  Author: Sebastian
 */ 


#ifndef ADC_H_
#define ADC_H_
#define F_CPU	16000000UL				//zewnetrzny rezonator
#include <avr/io.h>						
#include <util/delay.h>
#include "single_bit_macros.h"
#include "uno.h"
#include "eval_atmega328.h"

void ADC_Init();
int16_t ADC_Read(char);



#endif /* ADC_H_ */