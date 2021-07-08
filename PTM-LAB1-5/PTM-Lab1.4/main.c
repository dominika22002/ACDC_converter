#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/iom32.h>
#include "HD44780.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#ifndef inb
#define inb(addr) (addr)
#endif

#ifndef outb
#define outb(addr, data) addr = (data)
#endif

#ifndef sbi
#define sbi(reg, bit) reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg, bit) reg &= ~(_BV(bit))
#endif

#ifndef tbi
#define tbi(reg, bit) reg ^= (_BV(bit))
#endif

/*
 *  Gotowe zaimplementowane:
 #define 	bit_is_set(sfr, bit)   (_SFR_BYTE(sfr) & _BV(bit))
 #define 	bit_is_clear(sfr, bit)   (!(_SFR_BYTE(sfr) & _BV(bit)))
 #define 	loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
 #define 	loop_until_bit_is_clear(sfr, bit)   do { } while (bit_is_set(sfr, bit))

 */

// MIN/MAX/ABS macros
#define MIN(a, b) ((a < b) ? (a) : (b))
#define MAX(a, b) ((a > b) ? (a) : (b))
#define ABS(x) ((x > 0) ? (x) : (-x))

void ADC_init()
{
	sbi(ADMUX, REFS0); // konfiguracja napiecia referencyjnego na AVCC
	cbi(ADMUX, REFS1); // konfiguracja podzielnika czestotliwosci dla ukladu przetwornika. Mniejsza od 100KHz

	//	Czestotliwosc sygnalu taktujacego 62,5 kHz
	sbi(ADCSRA, ADPS0); //Tymi bitami definiujemy pozadana relacje 
	sbi(ADCSRA, ADPS1); //	 miedzy czestotliwoscia zegara sytemowego XTAL,
	sbi(ADCSRA, ADPS2); //	 a czestotliwoscia przebiegu taktujacego przetwornik.

	sbi(ADCSRA, ADEN); //uruchomienie ukladu przetwornika

	//	konfiguracja/wybor kanalu/pinu na ktorym bedzie dokonywany pomiar
	cbi(ADMUX, MUX0); //Bity wyboru wejscia analogowego.
	cbi(ADMUX, MUX1); //   Wejscie wybrane kombinacja tych bitow
	cbi(ADMUX, MUX2); //   jest dolaczone do przetwornika
	cbi(ADMUX, MUX3);
}

int uint16_tADC_10bit()
{
	sbi(ADCSRA, ADSC); //uruchomienie pojedynczego pomiaru - ustawienie bitu
	while (bit_is_set(ADCSRA, ADSC)) //oczekiwanie na zakończenie pomiaru – oczekiwanie na wyzerowanie bitu
	{
	}
	return ADC; // wynik pomiaru
}

int ADC_10bit()
{
	// char line[20];
	//LCD_Clear();			//na 3 odkomentowac
	//LCD_GoTo(0, 0);			//na 3 odkomentowac

	int A_D = uint16_tADC_10bit(); //odczytana wartosc z rejestru pomiarowego przetwornika A/D

	//LCD_WriteText(line);			//na 3 odkomentowac
	return A_D; //zwraca wynik pomiaru z przetwornika
}
int ADC_measure(){
	//wzor na na przeliczenie wartosci zwracanej [V]: (ADC_10bit()*ref_val)/1024
	int max_val = 1022;//powinno byc 1024 ale lepiej dziala tak
	int ref_val = 5;	//napiecie referencyjne
	int ret = (int)(((double)ADC_10bit()*ref_val)*100/max_val);

	return ret; //zwraca wyliczona wartosc
}

int zadanie_3(){
	while (1)
	{
		ADC_10bit();
		_delay_ms(100);
	}
	return 0;
}


int set_b_number(int val, char* buff){
	if(val%100 > 10){
		sprintf(buff,"Pomiar1:%d.%dV",val/100,val%100);	 //		   				     : Pomiar1: 2.12
	}
	else{
		sprintf(buff,"Pomiar1:%d.0%dV",val/100,val%100); //rozni sie zerem w zapisie : Pomiar1: 2.03
	}
	return 0;
}

int zadanie_4(){
	char buff[20];
	while(1){
		int val = ADC_measure();
		set_b_number(val,buff);

		LCD_Clear();
		LCD_GoTo(0, 1);
		LCD_WriteText(buff);
		_delay_ms(100);
	}
	return 0;
}

// 1-wieksze
// 0-mniejsze
int komparator(int a, int b){ //zwraca 1 lub 0 - ta sama funkcja co w linii 143
	if(a > b){
		return 1;
	}
	return 0;
}

// bool komparator(int a, int b){
// 	return a>b;
// }

int zadanie_45(){

	//komparator jest to uklad sluzacy do porownywania dwuch liczb/napiec
	char buff_1[20];
	char buff_2[20];
	int val=0;

	while(1){
		val = ADC_measure(); //Przeliczenie wartosci zwracanej
		LCD_Clear();  //osbluga wyswietlacza
		LCD_GoTo(0, 0);

		if(komparator(val,250)){ 	//sprawdza czy napiecie jest wyzsze niz 2,5[V]
			sprintf(buff_2,"on"); 	//jesli jest, jest napis "on" na wyswietlaczu 
			sbi(PORTC,PC3); 		//i zapalona jest dioda
		}
		else{
			sprintf(buff_2,"off");  //jesli nie, napis zmienia sie na "off" 
			cbi(PORTC,PC3);			//i dioda gasnie
		}

		set_b_number(val,buff_1);   //sprawdzenie standardu wyswietlania liczby 
		LCD_WriteText(buff_1); 		// wyswietlenie Pomiar1:...
		LCD_GoTo(0, 1);				//zmiana linii wyswietlacza
		LCD_WriteText(buff_2);		// wyswietlenie on/off
		_delay_ms(100);				//odwiezanie 
	}

	return 0;
}

int main()
{
	DDRC = 0xFF;					//inicjacja portow c
	PORTC = 0;						//stan niski na porcie C

	ADC_init();						//inicjowanie przetwornika
	LCD_Initalize();				//inicjowanie wyswietlacza
	LCD_Home();						

	zadanie_45();					//zadanie na 4.5 :)
}
