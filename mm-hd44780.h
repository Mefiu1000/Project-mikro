
#ifndef MM_HD44780_H
#define MM_HD44780_H

#define LCD_COLLS 16
#define LCD_ROWS 2
//////////////////////////////////////////////////////////////////////////
//makrodefinicje rozkazów
//////////////////////////////////////////////////////////////////////////

//czysci wyswietlacz
#define HD44780_CLR 0x01

//ustawia kursor na pocz¹tu DDRAM
#define HD44780_HOME 0x02

/*
 Ustawienia trybu wprowadzania znakow. 
 inc = 1 - autoinkrementacja adresu w DDRAM (jeœli 0 to autodekrementacja)
 shift = 1 - w³¹czenie przesuwania ekranu do aktualnie wprowadzonego znaku
*/
#define INCREMENT_AC 1
#define DECREMENT_AC 0
#define SHIFT_TO_ACTUAL 1
#define NO_SHIFT_TO_ACTUAL 0

#define HD44780_ENTMODE(inc, shift) \
	(0x04 | ((inc) ? 0x02: 0) | ((shift) ? 1: 0))

/*
 disp[lay] on/off, cursor on/off, cursor blink[ing] on/off.
*/
#define ON 1
#define OFF 0

#define HD44780_DISPCTL(disp, cursor, blink) \
	(0x08 | ((disp) ? 0x04: 0) | ((cursor) ? 0x02: 0) | ((blink) ? 1: 0))

/*
 shift = 1, - przesuwa ekran ,
 shift = 0, - przesuwa kursor,
 right = 1  - kierunek w prawo,
 fight = 0  - kierunek w lewo,
*/
#define SHIFT_DISPLAY 1
#define SHIFT_COURSOR 0
#define SHIFT_RIGHT 1
#define SHIFT_LEFT 0

#define HD44780_SHIFT(shift, right) \
	(0x10 | ((shift) ? 0x08: 0) | ((right) ? 0x04: 0))

/*
  ustawia komunikacje z LCD
  if8bit = 1 - 8-bitowy interfejs (jeœli 0 to 4-bitowy), 
  twoline = 1 - 2-liniowy wyœwietlacz (jeœli 0 to 1-liniowy),
  font5x10 = 1 - krój czcionek 5x10 dot font (jeœli 0 to 5x8).
*/
#define INTERFACE_8_BIT 1
#define INTERFACE_4_BIT 0
#define TWO_LINE_DISPLAY 1
#define ONE_LINE_DISPLAY 0
#define FONT_5x10 1
#define FONT_5x8  0

#define HD44780_FNSET(if8bit, twoline, font5x10) \
	(0x20 | ((if8bit) ? 0x10: 0) | ((twoline) ? 0x08: 0) | ((font5x10) ? 0x04: 0))
	
//ustawia nastêpny adres w generatorze adresów na addr. 
#define HD44780_CGADDR(addr) (0x40 | ((addr) & 0x3f))

//ustawia nastêpny adres w DDRAM na addr.
#define HD44780_DDADDR(addr) (0x80 | ((addr) & 0x7f))

//////////////////////////////////////////////////////////////////////////
//deklaracje funkcji
//////////////////////////////////////////////////////////////////////////
void MM_HD44780_dana(unsigned char d);
void MM_HD44780_rozkaz(unsigned char d);
void MM_HD44780_setup(void);
void MM_HD44780_setup2(void);

#endif
