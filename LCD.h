#include <avr/pgmspace.h>

//////////////////////////////////////////////////////////////////////////
//deklaracje funkcji do pisania po LCD
//////////////////////////////////////////////////////////////////////////

void LCDsetup(void); //wlaczenie i konfiguracja poczatkowa wyswietlacza
void LCDclr(void); //czyszczenei wyswietlacza przy pomocy firmowej funkcji
void LCDbklight(unsigned char stan);

//funkcie wyswietlajace ciag znakow odczytanych z pamieci RAM
void LCDc(char znak); //wyslanie znaku (char) na wyswietlacz
void LCDsn(char *ciagznakow, unsigned char n); //wyswietlenie ciagu n-znakow
void LCDs(char *ciagznakow);  //wyswietlenie ciagu znakow zakonczonych 0x00 (null)

void LCDxy(unsigned char x, unsigned char y); //ustawienie kursora w x kolumnie (0-15), y rzedzie (0, 1)
void LCDxys(unsigned char x, unsigned char y, char *ciagznakow); //wyswietlenie ciagu znakow zakonczonych 0x00 pod wspolzednymi x,y

//funkcja wyswietlajaca liczbe dziesietna
void LCDxyDec(unsigned char x, unsigned char y, int liczba);

//dodadkowe funkcje wyswietlajace ciagi znakow z obszaru pamieci kodu (FLASH)
void LCDsf(PGM_P ciagznakow); //wyswietlenie w miejscu kursora ciagu znakow zawartych we flashu zakonczonych 0x00 (null)
void LCDsnf(PGM_P ciagznakow, unsigned char n); //wyswietlenie w miejscu kursora ciagu n-znakow zawartego we flash
void LCDxysf(unsigned char x, unsigned char y, PGM_P ciagznakow); //wyswietlenie ciagu znakow zawartych we flashu zakonczonych 0x00 pod wspolzednymi x,y

void LCDdefchar5x8(uint8_t char_nr, PGM_P chardef); //funcja laduje generator znakow tablica 8x8 do ktorej wskaznik jest w chardef
