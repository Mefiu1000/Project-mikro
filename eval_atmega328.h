/*
 * eval_atmega328.h
 *
 * Created: 20.12.2020 14:22:14
 *  Author: Sebastian
 */ 


#ifndef EVAL_ATMEGA328_H_
#define EVAL_ATMEGA328_H_

// wyswietlacz LCD 2x16

#define LCD_D4		_PIN00
#define LCD_D5		_PIN01
#define LCD_D6		_PIN02
#define LCD_D7		_PIN03
#define LCD_EN		_PIN04
#define	LCD_RS		_PIN05
#define LCD_BKLIGHT	_PIN06

//Buzzer

#define Buzz _PIN16

// Przyciski

#define S2 _PIN07 

// Dioda RGB

#define LED_RED _PIN19
#define LED_GREEN _PIN18
#define LED_BLUE _PIN17

// Kanaly ADC/Joystick

#define ADC0 _PIN14
#define ADC1 _PIN15
#endif /* EVAL_ATMEGA328_H_ */