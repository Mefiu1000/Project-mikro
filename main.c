/*
 * projekt_zajecia.c
 *
 * Created: 20.12.2020 13:33:48
 * Author : Sebastian
 */ 

#include <avr/io.h>
#include "LCD.h"
#include "uno.h"
#include "eval_atmega328.h"
#include "single_bit_macros.h"
#include "ADC.h"
#include "menu.h"
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#define BUTTONS 3

extern volatile uint8_t RED;			// Czy dioda LED swieci kolorem czerwonym
extern volatile uint8_t GREEN;			// Czy dioda LED swieci kolorem zielonym
extern volatile uint8_t BLUE;			// Czy dioda LED swieci kolorem niebieskim
extern volatile uint8_t PWM_RED;		// Jasnosc 60% na start
extern volatile uint8_t PWM_GREEN;		// Jasnosc 60% na start
extern volatile uint8_t PWM_BLUE;		// Jasnosc 60% na start

char buffer[20];
int16_t ADC_Value;						// Przechowuje wartosc pomiaru z pinu ADC
uint8_t przejscie = 0;					// Odpowiada za to by przeskakiwac tylko o 1 pozycje menu
extern volatile int16_t f;
bool button_state_act[BUTTONS];
bool button_state_last[BUTTONS];
enum {bs1, bs2, bs3, bs4};

int main(void)
{
	
	//konfiguracja diody RGB, przycisku i buzzera
	SET(DDR,LED_RED);SET(DDR,LED_GREEN);SET(DDR,LED_BLUE);		// Wyjscia dla diody RGB
	SET(PORT,LED_RED);SET(PORT,LED_GREEN);SET(PORT,LED_BLUE);	// Wylaczenie Diody RGB ze wspolna anoda
	SET(DDR, Buzz);												// Wyjscie buzzer
	CLR(DDR, S2);												// Wejscie przycisk
	SET(PORT, S2);												// Pull up dla przycisku
	
	//konfiguracja timera0
	TCCR0A |= (1<<WGM01);					// Tryb CTC
	TCCR0B |= (1<<CS00);					// Preskaler =  1
	TIMSK0 |= (1<<OCIE0A);					// Zezwolenie na przerwanie compare match
	OCR0A = 255;							// Dodatkowy podzielnik = 255, bo dopoki czestotliwosc duze to nie widac migania diody
	sei();									// Odblokowanie globalne przerwania
	
	//konfiguracja timera1
	TCCR1A |= (1<<WGM10) | (1<<WGM11);
	TCCR1B |= (1<<WGM12) | (1<<WGM13); 		// Tryb Fast PWM, preskaler off, domyslnie timer wylaczony
	TIMSK1 |= (1<<OCIE1A);					// Zezwolenie na przerwanie compare match
	OCR1A = 16000000/(256*f)-1;				// Ustawianie czestotliwosci
	//konfiguracja adc
	ADC_Init();
	
	//konfiguracja lcd						
	LCDsetup();
	LCDbklight(1);
	LCDxys(0, 0, "Sebastian S.");
	LCDxys(0, 1, "W�odzimierz S.");
	_delay_ms(1000);
	LCDclr();
	Menu_Show();
	
	while(1)
	{
		ADC_Value = ADC_Read(1);
		
		if((ADC_Value < 200) && (przejscie == 0))
		{
			
			_delay_ms(20);
			
			if((ADC_Value < 200) && (przejscie == 0))
			{
				button_state_act[bs1] = true;
				if(button_state_act[bs1] != button_state_last[bs1])
				{
					button_state_last[bs1] = true;
					Menu_SelectNext();
					przejscie = 1;
				}
			}
		}
		else
			{
				button_state_act[bs1] = button_state_last[bs1] = false;
			}
			

		if((ADC_Value > 800) && (przejscie == 0))
		{
			_delay_ms(20);
			if((ADC_Value > 800) && (przejscie == 0))
			{
				button_state_act[bs2] = true;
				if(button_state_act[bs2] != button_state_last[bs2])
				{
					button_state_last[bs2] = true;
					Menu_SelectPrev();
					przejscie = 1;
				}
			}
			}else{
			button_state_act[bs2] = button_state_last[bs2] = false;
		}
		if(!GET(S2))
		{
			_delay_ms(20);
			if(!GET(S2))
			{
				button_state_act[bs3] = true;
				if(button_state_act[bs3] != button_state_last[bs3])
				{
					button_state_last[bs3] = true;
					Menu_Click();
					
				}
			}
			}else{
			button_state_act[bs3] = button_state_last[bs3] = false;
		}
		
		if((ADC_Value> 440) && (ADC_Value < 600)) przejscie = 0;
	}
}

//obs�uga przerwania CM timera0 
ISR(TIMER0_COMPA_vect)
{
	static uint8_t cnt;		// licznik PWM, static bo odrazu = 0 i tylko dostepna w przerwaniu, zainicjalizowana przy starcie programu
	if(RED == 1)			// Czy wlaczony kolor czerwony
		{
			if(cnt >= PWM_RED) SET(PORT,LED_RED); else CLR(PORT,LED_RED);
		}
	if(GREEN == 1)
		{					// Czy wlaczony kolor  zielony
			if(cnt >= PWM_GREEN) SET(PORT,LED_GREEN); else CLR(PORT,LED_GREEN);
		}
	if(BLUE == 1)			// Czy wlaczony kolor niebieski
	{
		if(cnt >= PWM_BLUE) SET(PORT,LED_BLUE); else CLR(PORT,LED_BLUE);
	}
	cnt++;					// Zwiekszenie licznika o 1
}

ISR(TIMER1_COMPA_vect)
{
	TGL(PORT,Buzz);
	TGL(PORT,LED_RED);
}
