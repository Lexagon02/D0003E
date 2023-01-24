/*
 * Lab1.c
 *
 * Created: 2023-01-19 22:34:55
 * Author : leorb
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/portpins.h>
#include <util/delay_basic.h>

#define _PROTECTED_WRITE(register, value);


void USART1_init(void)
{
	_PROTECTED_WRITE(CLKPR, 0x80);
	_PROTECTED_WRITE(CLKPR, 0x00);
}

void LCD_Init(void)
    {

    LCDCRB = (1<<LCDCS) | (1<<LCDMUX1)| (1<<LCDMUX0) |(1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);

    LCDFRR = (0<<LCDPS2) | (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
    
    LCDCCR = (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0);
    
    LCDCRA =  (1<<LCDEN); // LCDCRA |= 0x80 ----> ENABLE DISPLAY!
}

int16_t sourceCodeCharacter[10]={
        0x1551, //0
        0x0118, //1
        0x1e11, //2
        0x1b11, //3
        0x0b50, //4
        0x1b41, //5
        0x1f41, //6
        0x4009, //7
        0x1f51, //8
        0x1b51 //9
};

void writeChar(char ch, uint8_t pos)
{   if (!(ch >= 48 && ch<=57))
	{
		return;
	}
	typedef struct{
		uint8_t nibbles[4];
		uint16_t fontValue;
	volatile uint8_t *ptr; } regz;
	regz registers;
	
	registers.fontValue=sourceCodeCharacter[9-(57-ch)], registers.ptr=(uint8_t*)0xEC + pos/2;
	for(uint8_t i=0; i< 4; i++){
		*(registers.nibbles+i) = registers.fontValue & 0xf, registers.fontValue >>=4;
		if(!(pos%2)) { *registers.ptr &= ~(0xf), *registers.ptr |= *(registers.nibbles+i); }
		else{ *registers.ptr &= ~(0xf0), *registers.ptr |= (*(registers.nibbles+i) << 4); }
		registers.ptr+=5;
	}
} // struct
void clearLCD(void){
	LCDDR0 =0x0;
	LCDDR5=0x0;
	LCDDR10=0x0;
	LCDDR15=0x0;

	LCDDR1=0x0;
	LCDDR6=0x0;
	LCDDR11=0x0;
	LCDDR16=0x0;
	
	LCDDR2 =0x0;
	LCDDR7=0x0;
	LCDDR12=0x0;
	LCDDR17=0x0;

	LCDDR3=0x0;
	LCDDR8=0x0;
	LCDDR13=0x0;
	LCDDR18=0x0;
}

void writeLong(long i){
	
	long value_copy = i;
	unsigned int size_needed = 1; // For the null character.
	if (value_copy < 0) size_needed++; // Only needed for signed types.
	do {
	size_needed++; // Add 1 per digit.
	value_copy /= 10;
	} while (value_copy != 0);

	char str[size_needed];
	sprintf(str, "%ld", i);
	output(str, size_needed-1);	
}

int is_prime(long i){
   int c;
   for (c = 2; c <= i - 1; c++)
   {
      if (i%c == 0)
     return 0;
   }
   if (c == i)
      return 1;
} 

void output(char arr[], int length)
{
	int index = 0;
	int lastIndex = length;
	int pos = 0;
	index = length-6;
	while(index <= lastIndex)
	{
		char ch = arr[index];
		writeChar(ch, pos);
		index++;
		pos++;
	}
}
void primes(void){
	for(long i=3; i<10000; i++){
		if(is_prime(i) == 1){
			writeLong(i);
			_delay_ms(5000);
			clearLCD();
		}
	}
}

void LCD_blinkinit(void){
	
}

void LCD_blink(void){
	LCD_blinkinit();
	for(int i = 0; i<100; i++){
		LCDDR3 = 0x1;
		LCDDR8 = 0x1;
		LCDDR13 = 0x1;
		LCDDR18 = 0x1;
		_delay_ms(5000); 
		LCDDR3 = 0x0;
		LCDDR8 = 0x0;
		LCDDR13 = 0x0;
		LCDDR18 = 0x0;
		_delay_ms(5000); 
	}
}	

int main(void)
{
	USART1_init();
	LCD_Init();
		char arr[10] = "0123455555";
		int length = sizeof(arr)/sizeof(arr[0]);
		//output(arr, length);
		//_delay_ms(10000); //delay 0.5 sec
		clearLCD();
		//writeLong(12345678);
		//_delay_ms(50000); //delay 0.5 sec
		primes();
		LCD_blink();	
	
return 0;
}