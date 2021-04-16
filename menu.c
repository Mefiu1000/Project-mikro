/*
 * menu.c
 *
 * Created: 2014-11-11 11:39:05
 *  Author: Marcin
 */ 
#include <avr/io.h>
#include "uno.h"
#include "eval_atmega328.h"
#include <util/delay.h>
#include "menu.h"
#include "LCD.h"
#include "single_bit_macros.h"
#include <stdio.h>
#include "ADC.h"
//////////////////////////////////////////////////////////////////////////
// PROTOTYPY FUNKCJI WTWO�YWANYCH PO KLIKNIeCIU W POZYCJe MENU
//////////////////////////////////////////////////////////////////////////
//TUTAJ  MENU


void Menu_func2_1(void); //Buzzer ON
void Menu_func2_2(void); //Buzzer OFF
void Menu_func2_3(void); // freq

void Menu_func2_3_1(void); // Frequency UP
void Menu_func2_3_2(void); // Frequency DOWN

void Menu_func1_1(void); //LED ON
void Menu_func1_2(void); //LED OFF
void Menu_func1_4(void); //BRIGHTNESS

void Menu_func1_3_1(void); //LED RED
void Menu_func1_3_2(void); //LED GREEN
void Menu_func1_3_3(void); //LED BLUE

void Menu_func1_4_1_1(void); //LED RED Brightness increase
void Menu_func1_4_1_2(void); //LED RED Brightness decrease

void Menu_func1_4_2_1(void); //LED GREEN Brightness increase
void Menu_func1_4_2_2(void); //LED GREEN Brightness decrease

void Menu_func1_4_3_1(void); //LED BLUE Brightness increase
void Menu_func1_4_3_2(void); //LED BLUE Brightness decrease
//////////////////////////////////////////////////////////////////////////
//DEKLARACJE ZMIENNYCH DO OBS�UGI MENU

uint8_t Stan_Diody = 0;			//Czy dioda LED swieci
volatile uint8_t RED = 0;		//Czy dioda LED swieci kolorem czerwonym
volatile uint8_t GREEN = 0;		//Czy dioda LED swieci kolorem zielonym
volatile uint8_t BLUE = 0;		//Czy dioda LED swieci kolorem niebieskim
volatile uint8_t PWM_RED = 153; //60%
volatile uint8_t PWM_GREEN = 153;//60%
volatile uint8_t PWM_BLUE = 153;//60%
volatile int16_t f = 1;			// Czestotliwosc, po wlaczeniu 1Hz
extern int16_t ADC_Value;
//////////////////////////////////////////////////////////////////////////

typedef void (*menuItemFuncPtr)(); //deklaracja nowego typu danych wskaznika do funkcji 
									//wykonywanej po kliknieciu w pozycje menu

struct _menuitem //struktura pozycji menu
{
	const char *text; //wskaznik do �a�cucha znak�w z nazw� pozycji menu
	menuItemFuncPtr menuItemFunc; //wska�nik do funkcji obs�ugi wybranego menu
	const struct _menuitem *parent; //wska�nik na pozycj� manu rodzica
	const struct _menuitem *submenu; //wska�nik na podmenu
	const struct _menuitem *next; //wska�nik na nast�pn� pozycj� menu
};

//�a�cuchy znak�w w pami�ci FLASH z nazwami pozycji menu
const char PROGMEM nazwa1[] = "LED"; 
const char PROGMEM nazwa2[] = "BUZZER"; 
const char PROGMEM nazwa3[] = "Menu 3";
const char PROGMEM nazwa_ON[] = "ON";
const char PROGMEM nazwa_OFF[] = "OFF";
const char PROGMEM nazwa1_3[] = "COLOR";
const char PROGMEM nazwa1_4[] = "BRIGHTNESS";
const char PROGMEM nazwa1_3_1[] = "RED";
const char PROGMEM nazwa1_3_2[] = "GREEN";
const char PROGMEM nazwa1_3_3[] = "BLUE";
const char PROGMEM nazwa2_3[] = "FREQUENCY"; //Frequency buzzer
const char PROGMEM nazwaPowrot[] = {0x7F, 'B', 'a', 'c', 'k', 0};
const char PROGMEM nazwaUP[] = "+";
const char PROGMEM nazwaDOWN[] = "-";
//deklaracje kolejnych pozycji menu wraz z ich inicjalizacj�
//deklaracje zapowiadaj�ce dla menu1, menuA1 i menu B1 musz� by� znane przed kolejnymi pozycjami
const struct _menuitem menu1; 
const struct _menuitem menu2;
const struct _menuitem menu3; 
const struct _menuitem menu1_1;   
const struct _menuitem menu2_1;
const struct _menuitem menu1_3_1;
const struct _menuitem menu1_4_1;
const struct _menuitem menu2_3_1;
const struct _menuitem menu1_4_1_1;
const struct _menuitem menu1_4_2_1;
const struct _menuitem menu1_4_3_1;
//deklaracje kolejnych pozycji menu jako listy (poczynaj�c od nadalszej pozycji menu)
// {nazwa menu, wska�nik funkcji wywo�ywanej po klikni�ciu, wska�nik do rodzica, wska�nik do podmenu, wska�nik do nast�pnej pozycji}

//podmenu Buzzer
const struct _menuitem menu2_4 = {nazwaPowrot,	&Menu_Back,			&menu2_1,		0,				0 };
const struct _menuitem menu2_3 = {nazwa2_3,		&Menu_func2_3,		&menu2_1,		0,		&menu2_4 };
const struct _menuitem menu2_2 = {nazwa_OFF,	&Menu_func2_2,		&menu2_1,		0,				&menu2_3 };
const struct _menuitem menu2_1 = {nazwa_ON,		&Menu_func2_1,		&menu1,			0,				&menu2_2 };

//podmenu LED/color	
const struct _menuitem menu1_3_4 = {nazwaPowrot,	&Menu_Back,			&menu1_3_1,			0,			0};
const struct _menuitem menu1_3_3 = {nazwa1_3_3,		&Menu_func1_3_3,	&menu1_3_1,			0,			&menu1_3_4 };
const struct _menuitem menu1_3_2 = {nazwa1_3_2,		&Menu_func1_3_2,	&menu1_3_1,			0,			&menu1_3_3 };
const struct _menuitem menu1_3_1 = {nazwa1_3_1,		&Menu_func1_3_1,	&menu1_1,		    0,			&menu1_3_2 };

//podmenu LED/Brightness
const struct _menuitem menu1_4_4 = {nazwaPowrot,	&Menu_Back,			&menu1_4_1,			0,						0};
const struct _menuitem menu1_4_3 = {nazwa1_3_3,		0,					&menu1_4_1,			&menu1_4_3_1,			&menu1_4_4 };
const struct _menuitem menu1_4_2 = {nazwa1_3_2,		0,					&menu1_4_1,			&menu1_4_2_1,			&menu1_4_3 };
const struct _menuitem menu1_4_1 = {nazwa1_3_1,		0,					&menu1_1,		    &menu1_4_1_1,			&menu1_4_2 };

//podmenu LED/Brightness/RED
const struct _menuitem menu1_4_1_3 = {nazwaPowrot,		&Menu_Back,			&menu1_4_1_1,		0,			0};
const struct _menuitem menu1_4_1_2 = {nazwaDOWN,		&Menu_func1_4_1_2,	&menu1_4_1_1,		0,			&menu1_4_1_3 };
const struct _menuitem menu1_4_1_1 = {nazwaUP,			&Menu_func1_4_1_1,	&menu1_4_1,		    0,			&menu1_4_1_2 };

//podmenu LED/Brightness/GREEN
const struct _menuitem menu1_4_2_3 = {nazwaPowrot,		&Menu_Back,			&menu1_4_2_1,		0,			0};
const struct _menuitem menu1_4_2_2 = {nazwaDOWN,		&Menu_func1_4_2_2,	&menu1_4_2_1,		0,			&menu1_4_2_3 };
const struct _menuitem menu1_4_2_1 = {nazwaUP,			&Menu_func1_4_2_1,	&menu1_4_1,		    0,			&menu1_4_2_2 };
	
//podmenu LED/Brightness/BLUE
const struct _menuitem menu1_4_3_3 = {nazwaPowrot,		&Menu_Back,			&menu1_4_3_1,		0,			0};
const struct _menuitem menu1_4_3_2 = {nazwaDOWN,		&Menu_func1_4_3_2,	&menu1_4_3_1,		0,			&menu1_4_3_3 };
const struct _menuitem menu1_4_3_1 = {nazwaUP,			&Menu_func1_4_3_1,	&menu1_4_1,		    0,			&menu1_4_3_2 };
			
//podmenu LED
const struct _menuitem menu1_5 = {nazwaPowrot,		&Menu_Back,			&menu1_1,			0,					0};
const struct _menuitem menu1_4 = {nazwa1_4,			0,					&menu1_1,			&menu1_4_1,			&menu1_5 };
const struct _menuitem menu1_3 = {nazwa1_3,			0,					&menu1_1,			&menu1_3_1,			&menu1_4 };
const struct _menuitem menu1_2 = {nazwa_OFF,		&Menu_func1_2,		&menu1_1,			0,					&menu1_3 };
const struct _menuitem menu1_1 = {nazwa_ON,			&Menu_func1_1,		&menu1,				0,					&menu1_2 };

//menu g��wne
const struct _menuitem menu2 = {nazwa2,		0,		&menu1,		&menu2_1,		0};
const struct _menuitem menu1 = {nazwa1,		0,		0,			&menu1_1,		&menu2 };

static const struct _menuitem *currMenuPtr = &menu1; //wska�nik na pierwsz� pozycj� w aktualnym menu
static int8_t menuIndex; //aktualnie wybrany element menu (wskazany na ekranie)
static int8_t menuFirstPos; //numer pozycji menu wy�wietlanej w pierwszym wierszu wy�wietlacza LCD

//////////////////////////////////////////////////////////////////////////
//DEFINICJE FUNKCJI OBS�UGI MENU
//////////////////////////////////////////////////////////////////////////

void Menu_Back(void)
{	//przej�cie z podmenu do menu wy�szego poziomu
	const struct _menuitem *tempMenuPtr = currMenuPtr->parent;
	menuFirstPos = 0;
	menuIndex = 0;
	currMenuPtr = tempMenuPtr;
}

void Menu_Click()
{	//wybiera funkcj� adekwatn� do wskazanego menu lub otwiera podmenu
	const struct _menuitem *tempMenuItem = GetMenuItem(menuIndex);
	const struct _menuitem *subMenu = tempMenuItem->submenu;
	menuItemFuncPtr funPtr = tempMenuItem->menuItemFunc;
	if(funPtr) //je�li wska�nik do funkcji nie jest zerowy to j� wywo�aj
		(*funPtr)();
	if(subMenu) //je�li wska�nik do submenu nie jest zerowy to przejdz do podmenu
	{
		currMenuPtr = subMenu;
		menuIndex = 0;
		menuFirstPos = 0;
	}
	Menu_Show();
}

const struct _menuitem *GetMenuItem(uint8_t index)
{	//funkcja znajduje i zwraca adres elementu menu o podanym indeksie
	const struct _menuitem *tempMenuItem = currMenuPtr;
	while((tempMenuItem) && (index)>0) //dopuki adres aktualnego menu nie jest zerem i indeks jest wi�kszy od zera
	{	//przeskakuj po kolejnych pozycjach menu dekrementuj�c indeks
		//p�tla zatrzyma si� na adresie o wksazanym indeksie
		tempMenuItem = tempMenuItem->next;
		index--;
	}
	return tempMenuItem;
}

void Menu_SelectPrev(void)
{	//funkcja przechodzi do porzedniej pozycji menu
	if(menuIndex > 0)
	{
		menuIndex--;
		if(menuIndex < menuFirstPos)//je�li wybrana pozycja powy�ej pierwszej linii, przesu� te� pierwsz� lini�
			menuFirstPos--;
	}
	else
	{	//przesuni�cie z pierwszej pozycji menu na ostatni� (zap�tlenie menu)
		const struct _menuitem *tempMenuItem = currMenuPtr;
		while(tempMenuItem)
		{	//wyszukanie ostatniej pozycji w aktualnym menu
			tempMenuItem = tempMenuItem->next;
			if(tempMenuItem)
				menuIndex++;
		}
	}
	menuFirstPos = menuIndex - 1; //w pierwszej linii jest pozycja o jeden mniejsza od aktualnie wybranej
	if(menuFirstPos < 0)
	{
		menuFirstPos = 0;
	}
	Menu_Show();
}

void Menu_SelectNext(void)
{	//funkcja przechodzi do nast�pnej pozycji w menu
	if(GetMenuItem(menuIndex + 1) != 0) //je�li nast�pna pozycja istnieje (ma niezerowy wska�nik)
	{
		menuIndex++;
		if((menuIndex - menuFirstPos) >= 2) 
			menuFirstPos++; //po przesuni�ciu na pozycje poza wyswietlaczem, przesu� tak�e pozycj� wy�wietlan� w pierwszej linii
	}
	else
	{	//gdy dojgdziesz do konca menu powracaj do pocz�tku (zap�tlenie)
		menuIndex = 0;
		menuFirstPos = 0;
	}
	Menu_Show();
}

void Menu_Show(void)
{	//funkcja wy�wietla na LCD pozycje menu i znak aktualnie wybranej pozyci
	const struct _menuitem *tempMenuItem = GetMenuItem(menuFirstPos); //pobranie adresu pierwszej pozycji menu
	for(uint8_t i=0; i<LCD_ROWS; i++)
	{	//pozycje wy�wietlane s� w dw�ch liniach
		uint8_t charcnt = 1; //licznik znak�w wy�wietlonych w jednej linii
		LCDxy(0,i);
		if((i+menuFirstPos) == menuIndex)
			LCDc('>');//w pierwszej kolumnie wybranej pozycji menu wstaw znak >
		else
			LCDc(' ');//gdy pozycja nie jest wybrana zamiast znaku > wstaw spacj�
		if(tempMenuItem->text)
		{	//je�li istnieje wska�nik do nazwy menu to w�lij j� na LCD
			LCDsf(tempMenuItem->text);
			charcnt += strlen_P(tempMenuItem->text); //i powi�ksz licznik wy�wietlonych znak�w w jednej linii
		}
		if(tempMenuItem->submenu)
		{	//je�li istnieje wska�nik do podmenu
			LCDc(0x7E); //narysuj obok nazwy pozycji menu symbol strza�ki w prawo
			charcnt++;
		}
		for( ; charcnt<LCD_COLLS; charcnt++)
			LCDc(' '); //wyczyszczenie reszty wyswietlanej linii spacjami aby skasowa� ko�c�wki nazw poprzednich pozycji
		tempMenuItem = tempMenuItem->next; //przesu� wska�nik na nast�pn� pozycj� w menu dla kolejnego obiegu p�tli
	}
}
//funkcje menu g��wnego


//funkcje podmenu 1-stopnia
void Menu_func1_1(void)		// LED ON ,domyslnie color RED
{
	if((!GET(S2)) && (Stan_Diody == 0))
		{
		 	_delay_ms(80);
			 Stan_Diody = 1; RED = 1; GREEN = 0; BLUE = 0;
		 	CLR(PORT,LED_RED);
		 	SET(PORT,LED_GREEN);SET(PORT,LED_BLUE);
		}
}

void Menu_func1_2(void)		// LED OFF
{
	if(!GET(S2))
	{
		_delay_ms(80);
		Stan_Diody = 0; RED = 0; GREEN = 0; BLUE = 0;
		SET(PORT,LED_RED);SET(PORT,LED_GREEN);SET(PORT,LED_BLUE);
	}
}

void Menu_func2_1(void)				// Buzzer ON
{
	
	if(!GET(S2))
	{
		_delay_ms(80);
		TCCR1B |=  (1<<CS12);							// preskaler 256, buzzer ON
		Stan_Diody = 0; RED = 0; GREEN = 0; BLUE = 0;	// wy��czenie diody je�li wcze�niej �wieci�a
		SET(PORT,LED_GREEN);SET(PORT,LED_BLUE);
	}
}

void Menu_func2_2(void)				// Buzzer OFF
{
	if(!GET(S2))
	{
		_delay_ms(80);
		TCCR1B &= 0x18 ;			// preskaler off, buzzer OFF 0001 1000
		SET(PORT,LED_RED);			// zgaszenie diody
	}

}

void Menu_func2_3(void)				// Buzzer freq	
{
	LCDclr(); 
	_delay_ms(500);
	char buf[3];
	uint8_t przejscie = 0;
	sprintf(buf, "%d", f);
	LCDxys(0,0, buf);
	
	while(1)
	{
		ADC_Value = ADC_Read(1);		// Pomiar z ADC1
		
		if((ADC_Value > 800) && (przejscie == 0))
		{
			f++;
			OCR1A = 16000000/(256*f)-1;
			sprintf(buf, "%d", f);
			LCDxys(0,0, buf);
			przejscie = 1;
		}
		if((ADC_Value < 200) && (przejscie == 0))
		{
			if (f<=1)
				f=1;
			else
			{
				if(f<=10)
					LCDxys(1,0," "); // wstawia pusty znak na miejsce 0 z 10
				f--;
			}
			OCR1A = 16000000/(256*f)-1;
			sprintf(buf, "%d", f);
			LCDxys(0,0, buf);
			przejscie = 1;
		}
		if((ADC_Value> 440) && (ADC_Value < 600)) przejscie = 0;
		
		if(!GET(S2))
		{
			_delay_ms(20);
			if(!GET(S2))
				break;
		}
			
	}
	
		
}

//funkcje podmenu 2-stopnia

void Menu_func1_3_1(void)			// color RED
{
	if((!GET(S2)) && (Stan_Diody == 1))
	{
		_delay_ms(80);
		RED = 1; GREEN = 0; BLUE = 0;
		CLR(PORT,LED_RED);
		SET(PORT,LED_GREEN);SET(PORT,LED_BLUE);
	}
}

void Menu_func1_3_2(void)			// color GREEN
{
	if((!GET(S2)) && (Stan_Diody == 1))
	{
		_delay_ms(80);
		RED = 0; GREEN = 1; BLUE = 0;
		CLR(PORT,LED_GREEN);
		SET(PORT,LED_BLUE);SET(PORT,LED_RED);
	}
}

void Menu_func1_3_3(void)			// color BLUE
{
	if((!GET(S2)) && (Stan_Diody == 1))
	{
		_delay_ms(80);
		RED = 0; GREEN = 0; BLUE = 1;
		CLR(PORT,LED_BLUE);
		SET(PORT,LED_GREEN);SET(PORT,LED_RED);
	}
}

 

//funkcje podmenu 3 stopnia

void Menu_func1_4_1_1()				// Brightness RED++
{
	if(!GET(S2))
	{
		_delay_ms(80);
		if(PWM_RED == 255);
		else PWM_RED+=51;			// +20%
	}
}

void Menu_func1_4_1_2()				// Brightness RED--
{
	if(!GET(S2))
	{
		_delay_ms(80);
		if(PWM_RED == 0);
		else PWM_RED-=51;			// -20%
	}
}

void Menu_func1_4_2_1()				// Brightness GREEN++
{
	if(!GET(S2))
	{
		_delay_ms(80);
		if(PWM_GREEN == 255);
		else PWM_GREEN+=51;			// +20%
	}
}

void Menu_func1_4_2_2()				// Brightness GREEN--
{
	if(!GET(S2))
	{
		_delay_ms(80);
		if(PWM_GREEN == 0);
		else PWM_GREEN-=51;			// -20%
	}
}

void Menu_func1_4_3_1()				// Brightness BLUE++
{
	if(!GET(S2))
	{
		_delay_ms(80);
		if(PWM_BLUE == 255);
		else PWM_BLUE+=51;			// +20%
	}
}

void Menu_func1_4_3_2()				// Brightness BLUE--
{
	if(!GET(S2))
	{
		_delay_ms(80);
		if(PWM_BLUE == 0);
		else PWM_BLUE-=51;			// -20%
	}
}




