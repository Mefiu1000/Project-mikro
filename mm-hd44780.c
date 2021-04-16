// najprostszy sterownik wyœwietlacza HD44780
// 2010-10-06	v1.1	poprawiono wszelkie nazwy z hd4480 na hd44780 ("czeski" b³¹d, zjedzono jedn¹ literkê)
// 2010-09-06	v1.0	
// sterownik potrzebuje nastepujace linie:
// LCD_D4,LCD_D5,LCD_D6,LCD_D7 - 4 bity danych (wspolpraca 4 bitowa)
// LCD_EN - linia enable signal
// LCD_RS - linia register select (wybierajace dane badz rozkazy)
// LCD_BKLIGHT - linia zalaczajaca podswietlanie
// nie stosujemy linii LCD_RW - poniewaz nie bedziemy niczego odczytywac z wyswietlacza, dla MM-EVAL1 jest stale w stanie 0, 
// czyli jedynie zapisujemy do wyswietlacza

#define MM_HD44780_VER11		

#include <avr/io.h>         
#include "single_bit_macros.h"
//#include "config.h"
#include "uno.h"
#include "eval_atmega328.h"
#include <util/delay.h>     
#include "mm-hd44780.h"

//////////////////////////////////////////////////////////////////////////
// Definicje funkcji
//////////////////////////////////////////////////////////////////////////

// procedura wysyla 4 bity na wyswietlacz
void MM_HD44780_4b(unsigned char d)
{
	// ustawiam LCD_EN na 1
	SET(PORT, LCD_EN);

	// poniewaz moze sie zdarzyc, ze kazda linia wyswietlacza bedzie podlaczana do 
	// roznych bitow w roznych portach
	// testuje stan kazdego bitu i zapalam badz kasuje odpowiednia linie

	if (d & 0x01)	SET(PORT,LCD_D4);	else CLR(PORT,LCD_D4);
	if (d & 0x02)	SET(PORT,LCD_D5);	else CLR(PORT,LCD_D5);
	if (d & 0x04)	SET(PORT,LCD_D6);	else CLR(PORT,LCD_D6);
	if (d & 0x08)	SET(PORT,LCD_D7);	else CLR(PORT,LCD_D7);

	// najmniejszy czas Tw ynosi wg dok 230ns, dlatego 1us wystarczy z nawiazka
	_delay_us(1);
	// ustawiam LCD_EN na 0, opadajace zbocze LCD_EN powoduje zapis danych do wyswietlacza
    CLR(PORT, LCD_EN);
	// caly cykl zapisu powinien wg dok. trac min 500ns, stad 1us wystarczy z nawiazka
	_delay_us(1);
}

void MM_HD44780_bajt(unsigned char d)
{
	MM_HD44780_4b(d>>4);
	MM_HD44780_4b(d);
}	

// procedura wysyla rozkaz do LCD
void MM_HD44780_rozkaz(unsigned char d)
{
	// ustawiam 0 na linii register select, oznacza to wyslanie rozkazu do wyswietlacza
	CLR(PORT,LCD_RS);
	MM_HD44780_bajt(d);
	// tutaj oczekiwanie az wyswietlacz zrobi swoje
	_delay_us(100);
}

// procedura wysyla dane do LCD
void MM_HD44780_dana(unsigned char d)
{
	// ustawiam 1 na linii register select, oznacza to wyslanie danych do wyswietlacza
	SET(PORT,LCD_RS);
	MM_HD44780_bajt(d);
	// tutaj oczekiwanie az wyswietlacz zrobi swoje
	_delay_us(100);
}

const unsigned char ls[] ={ 0x03, 0x03, 0x03, 0x02,    	// zalacz 4 bitowy interfejs
                            0x02, 0x08,          		// 4 bity jeszcze raz i ile linii
                            0x00, 0x0c,                 // disp on, cursor on, blink on
                            0x00, 0x06,              	//tryb dzialania i icrement i shift                                      
                            0x00, 0x01};				//clear display   														

void MM_HD44780_setup(void)
{
    unsigned char i;
	// ustawiam porty procesora jako wyjscia
	SET (DDR, LCD_D4);
	SET (DDR, LCD_D5);
	SET (DDR, LCD_D6);
	SET (DDR, LCD_D7);	
	SET (DDR, LCD_EN);
	SET (DDR, LCD_RS);	
	SET (DDR, LCD_BKLIGHT);	

	// zeruje linie RS - wysylane beda rozkazy
	CLR(PORT,LCD_RS);
    _delay_ms(5);
    for (i=0; i<12; i++)
    {
        MM_HD44780_4b(ls[i]);
        _delay_ms(10);
    }
	// ustawiam linie RS - wysylane beda dane        
	SET(PORT,LCD_RS);
	_delay_ms(100);
}

void MM_HD44780_setup2(void)
{
	uint8_t i;
	// ustawiam porty procesora jako wyjscia
	SET (DDR, LCD_D4);
	SET (DDR, LCD_D5);
	SET (DDR, LCD_D6);
	SET (DDR, LCD_D7);
	SET (DDR, LCD_EN);
	SET (DDR, LCD_RS);
	SET (DDR, LCD_BKLIGHT);

	CLR(PORT,LCD_RS);// RS=0 - wysylane beda rozkazy
	_delay_ms(15);
	for (i=0; i<3; i++)
	{
		MM_HD44780_4b(0x03);
		_delay_ms(5);
	}
	MM_HD44780_4b(0x02);
	_delay_us(40);
	MM_HD44780_rozkaz(HD44780_FNSET(INTERFACE_4_BIT, TWO_LINE_DISPLAY, FONT_5x8)); //4-bitowy interfejs, 2-linie, znaki 5x8
	_delay_us(40);	
	MM_HD44780_rozkaz(HD44780_DISPCTL(ON, OFF, OFF)); //LCD -on, cursor -off, blink -off
	_delay_us(40);
	MM_HD44780_rozkaz(HD44780_ENTMODE(INCREMENT_AC, NO_SHIFT_TO_ACTUAL)); //increment AC, bez przesuwania ekranu
	_delay_us(40);
	MM_HD44780_rozkaz(HD44780_CLR); //czyszczenie ekranu
	_delay_us(40);
	MM_HD44780_rozkaz(HD44780_HOME); //czyszczenie ekranu
	_delay_ms(2);
	
	SET(PORT,LCD_RS);// RS=1 - wysylane beda dane
	_delay_ms(100);
}