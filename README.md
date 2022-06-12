# Project-mikro
Projekt zaliczeniowy na przedmiot z programowania mikrokontrolerów w języku C.
Komponenty:

-arduino Uno (Atmega 328p),

-wyświetlacz LCD 2x16,

-dioda RGB,

-buzzer,

-joystick.

Na wyświetlaczu stworzono kilku-stopniowe menu przewijalne, które pozwala na sterowanie diodą RGB oraz buzzerem. 
Poruszanie po menu odbywa się z użyciem joysticka (góra, dół, wciśnięcie).
Opcje dla diody:

-włącz/wyłącz,

-zmiana koloru,

-zmiana jasności 0-100% dla każdego koloru osobno.

Buzzer wykorzystano do stworzenia metronomu. W menu można sterować częstotliwością jaką pika, 
co równocześnie steruje prędkością migania diody.
