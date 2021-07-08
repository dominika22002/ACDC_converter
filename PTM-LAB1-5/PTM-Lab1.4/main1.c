// #include <avr/io.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h>
// #include <util/delay.h>
// #include <avr/sfr_defs.h>
// #include <math.h>
// #include <avr/interrupt.h>
// #include <avr/eeprom.h>
// #include <avr/iom32.h>
// #include "HD44780.h"

// #ifndef _BV
// #define _BV(bit) (1 << (bit))
// #endif

// #ifndef inb
// #define inb(addr) (addr)
// #endif

// #ifndef outb
// #define outb(addr, data) addr = (data)
// #endif

// #ifndef sbi
// #define sbi(reg, bit) reg |= (_BV(bit))
// #endif

// #ifndef cbi
// #define cbi(reg, bit) reg &= ~(_BV(bit))
// #endif

// #ifndef tbi
// #define tbi(reg, bit) reg ^= (_BV(bit))
// #endif

// /*
//  *  Gotowe zaimplementowane:
//  #define 	bit_is_set(sfr, bit)   (_SFR_BYTE(sfr) & _BV(bit))
//  #define 	bit_is_clear(sfr, bit)   (!(_SFR_BYTE(sfr) & _BV(bit)))
//  #define 	loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
//  #define 	loop_until_bit_is_clear(sfr, bit)   do { } while (bit_is_set(sfr, bit))

//  */

// // MIN/MAX/ABS macros
// #define MIN(a, b) ((a < b) ? (a) : (b))
// #define MAX(a, b) ((a > b) ? (a) : (b))
// #define ABS(x) ((x > 0) ? (x) : (-x))

// void ADC_init()
// {
// 	sbi(ADMUX, REFS0); // konfiguracja napięcia referencyjnego na AVCC
// 	cbi(ADMUX, REFS1); // konfiguracja podzielnika częstotliwości dla układu przetwornika. Mniejsza od 100KHz

// 	//	Czestotliwosc sygnalu taktujacego 62,5 kHz
// 	sbi(ADCSRA, ADPS0); //Tymi bitami definiujemy pożądaną relację 
// 	sbi(ADCSRA, ADPS1); //	 między częstotliwością zegara sytemowego XTAL, 
// 	sbi(ADCSRA, ADPS2); //	 a częstotliwością przebiegu taktującego przetwornik.

// 	sbi(ADCSRA, ADEN); //uruchomienie układu przetwornika

// 	//	konfiguracja/wybór kanału/pinu na którym będzie dokonywany pomiar
// 	cbi(ADMUX, MUX0); //Bity wyboru wejścia analogowego. 
// 	cbi(ADMUX, MUX1); //   Wejście wybrane kombinacją tych bitów
// 	cbi(ADMUX, MUX2); //   jest dołączone do przetwornika
// 	cbi(ADMUX, MUX3);
// }

// int uint16_tADC_10bit()
// {
// 	sbi(ADCSRA, ADSC); //uruchomienie pojedynczego pomiaru 
// 	while (bit_is_set(PINA, ADSC))
// 	{
// 	}
// 	return ADC;
// }

// int ADC_10bit()
// {
// 	char line[20];
// 	LCD_Clear();
// 	LCD_GoTo(0, 0);

// 	int A_D = uint16_tADC_10bit(); //odczytana wartość z rejestru pomiarowego przetwornika A/D

// 	sprintf(line, "%d", A_D); 
// 	LCD_WriteText(line);
// 	return A_D;
// }


// int main()
// {
// 	while (1)
// 	{
// 		_delay_ms(10);
// 		ADC_10bit();
// 		_delay_ms(10);
// 	}
// }
