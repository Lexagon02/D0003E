#include "tinythreads.h"
#include "tinythreads.c"
#include <stdbool.h>

void writeChar(char ch, int pos); // from lab 1

bool is_prime(long i); // from lab 1

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

void writeChar(char ch, uint8_t pos) {
	if (!(ch >= 48 && ch<=57)) {
		return;
	}
	typedef struct {
		uint8_t nibbles[4];
		uint16_t fontValue;
		volatile uint8_t *ptr; 
	} 
	regz;
	regz registers;
	
	registers.fontValue=sourceCodeCharacter[9-(57-ch)], registers.ptr=(uint8_t*)0xEC + pos/2;
	for(uint8_t i=0; i< 4; i++)
	{
		*(registers.nibbles+i) = registers.fontValue & 0xf, registers.fontValue >>=4;
		if(!(pos%2)) 
		{ 
			*registers.ptr &= ~(0xf), *registers.ptr |= *(registers.nibbles+i); 
		}
		else
		{ 
			*registers.ptr &= ~(0xf0), *registers.ptr |= (*(registers.nibbles+i) << 4); 
		}
		registers.ptr+=5;
	}
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

void printAt(long num, int pos) {
    int pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
    pp++;
    writeChar( num % 10 + '0', pp);
}

void computePrimes(int pos) {
    long n;

    for(n = 1; ; n++) {
        if (is_prime(n)) {
            printAt(n, pos);
        }
    }
}

int main() {
    spawn(computePrimes, 0);
    computePrimes(3);
}
