// realizacja najprostszych funkcji obslugi wyswietlacza 2x16 
// 2014-10-30	v2.1	zmieniono nazwy funkcji piszacych, dodano akrodefinicje rozkazow
// 2011-01-25	v2.0 	obsluga danych w pamieci FLASH, dodano procedury Lsf, Lsnf, Lxysf
// 2010-09-22	v1.0	

#define LCD_VER21	

#include <avr/io.h>  
#include <stdlib.h>         
#include <avr/pgmspace.h>
#include "single_bit_macros.h"
//#include "config.h"
#include "uno.h"
#include "eval_atmega328.h"
#include "mm-hd44780.h"
#include <util/delay.h>

//////////////////////////////////////////////////////////////////////////
//definicje funkcji
//////////////////////////////////////////////////////////////////////////

void LCDsetup(void)
{
	//MM_HD44780_setup();
	MM_HD44780_setup2();
}

// funkcja czysci wyswietlacz przy pomocy firmowej funkcji
void LCDclr(void)
{
	// przesylamy rozkaz czyszczenia wyswietlacza
	MM_HD44780_rozkaz(HD44780_CLR);
	// odczekujemy 2 milisekundy
	_delay_ms(2);
}

// wyslanie litery (char) na wyswietlacz
void LCDc(char znak)
{
	// w czasie normalnej pracy wyswietlacza
	// wyslanie do niego danej powoduje pojawienie
	// sie tego znaku na wyswietlaczu
	// w miejscu, w ktorym znajduje sie kursor
	MM_HD44780_dana(znak);
}

// funkcja ustawia kursor wyswietlacza
void LCDxy(unsigned char x, unsigned char y)
{	
	// sprawczam czy nie wychodzi poza ekran
	if (x>15) x=15;
	if (y>1) y=1;

	// jezeli y=1, dodaje offset 0x40 poniewaz od adresu 0x40 swojej pamieci wyswietlacz wyswietla druga linie
	x+=y*0x40;
	MM_HD44780_rozkaz(HD44780_DDADDR(x));
}


// wyswietlenie ciagu n-znakow
void LCDsn(const char *ciagznakow, unsigned char n)
{
	unsigned char k;
	// odliczam klejne znaki
	for (k=0; k<n; k++)
	{	
		// wysylam do wyswietlacza znak wkazywany przez *ciagznakow i zwiekszam wskaznik o 1
		LCDc(*ciagznakow++);
	}
}

// wyswietlenie ciagu znakow zakonczonych 0x00 null
void LCDs(const char *ciagznakow)
{
    char q;
	// tak dlugo wysylam znak wskazywany przez *ciagznakow i zwiekszam wskaznik o 1 az znak wynosi 0x00
	while ((q=*ciagznakow++)!=0x00)
    {
		LCDc(q);
	}
}

// wyswietlenie ciagu znakow zakonczonych 0x00 pod wspolzednymi x,y
void LCDxys(unsigned char x, unsigned char y, const char *ciagznakow)
{
	LCDxy(x,y);
	LCDs(ciagznakow);
}

void LCDbklight(unsigned char stan)
{
	//if (stan&0x01) SET(PORT,LCD_BKLIGHT); else CLR(PORT,LCD_BKLIGHT);

}

// wyswietlenie ciagu znakow zakonczonych zawartych we flashu 0x00 null zawartych we flashu
void LCDsf(PGM_P ciagznakow)
{
    char q;
	// tak dlugo wysylam znak wskazywany przez *ciagznakow i zwiekszam wskaznik o 1 az znak wynosi 0x00
	while ((q=pgm_read_byte(&(*ciagznakow++)))!=0x00)
    {
		LCDc(q);
	}

}

// wyswietlenie ciagu znakow zawartych we flashu zakonczonych 0x00 pod wspolzednymi x,y
void LCDxysf(unsigned char x, unsigned char y, PGM_P ciagznakow)
{
	LCDxy(x,y);
	LCDsf(ciagznakow);
}

// wyswietlenie ciagu n-znakow zawartego we flash
void LCDsnf(PGM_P ciagznakow, unsigned char n)
{
	unsigned char k;
	// odliczam klejne znaki
	for (k=0; k<n; k++)
	{	
		// wysylam do wyswietlacza znak wkazywany przez *ciagznakow i zwiekszam wskaznik o 1
		LCDc(pgm_read_byte(&(*ciagznakow++)));
	}
}

//wyswietlenie liczby dziesietnej w miejscu o podanych wsporzednych x,y
void LCDxyDec(unsigned char x, unsigned char y, int liczba)
{
	char buf[6];
	itoa(liczba, buf, 10);
	LCDxys(x, y, buf);
}

//funkcja zapisuje zdefiniowane przez uzytkownika znaki do generatora zanakow wyswietlacza
//znaki musza byc zapisane w tablicy umieszczonej w pamieci programu
// do kotrej wskaznik jest podany jako chardef
void LCDdefchar5x8(uint8_t char_nr, PGM_P chardef)
{
	MM_HD44780_rozkaz(HD44780_CGADDR(char_nr*8));
	for(uint8_t j=0; j<8; j++)
	{
		MM_HD44780_dana(pgm_read_byte(&chardef[j]));
		_delay_ms(2);
		
	}
}