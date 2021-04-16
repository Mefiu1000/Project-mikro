/*
 * ADC.c
 *
 * Created: 26.12.2020 14:13:49
 *  Author: Sebastian
 */ 
#include "ADC.h"

void ADC_Init()
{
	CLR(DDR,ADC0);CLR(DDR,ADC1);								// Wejscia
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // Wlaczenie ADC , preskaler 128
	ADMUX |= (1<<REFS0);										// Wybor napiecia odniesienia w postaci VCC
}

int16_t ADC_Read(char channel)
{
	int16_t ADC_value;
	
	ADMUX = (1<<REFS0) | (channel & 0x07);		// Ustawienie kanalu ADC w rejestrze ADMUX (0x07 = 0111), channel  u nas ma wartosc 1 (od Y joysticka) ale moze miec np. 0 (od X joysticka)
	ADCSRA |= (1<<ADSC);						// Start pomiaru
	while((ADCSRA &(1<<ADIF))== 0);				// Sprawdzanie stanu flagi przerwania ADIF
	ADCSRA |= (1<<ADIF);						// Wyczyszczenie flagi przerwania 
	ADC_value = ADCW;							// Odczytanie wyniku z rejestrow ADCH i ADCL w postaci 16-bitowej
	return ADC_value;							// Zwrocenie wyniku
}