#include <avr/io.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/portpins.h>
#include <util/delay_basic.h>

#define _PROTECTED_WRITE(register, value);


void LCD_Init(void)
{

	LCDCRB = (1<<LCDCS) | (1<<LCDMUX1)| (1<<LCDMUX0) |(1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);

	LCDFRR = (0<<LCDPS2) | (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
	
	LCDCCR = (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0);
	
	LCDCRA =  (1<<LCDEN); // LCDCRA |= 0x80 ----> ENABLE DISPLAY!

	
}

void USART1_init(void)
{
	_PROTECTED_WRITE(CLKPR, 0x80);
	_PROTECTED_WRITE(CLKPR, 0x00);
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
}

bool is_prime(long i){
	int c;
	for (c = 2; c <= i - 1; c++)
	{
		if (i%c == 0)
		return false;
	}
	return true;
}

