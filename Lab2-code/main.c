#include "tinythreads.h"
#include <stdbool.h>
#include <stdio.h>
//#include "lab1.c"
#include <avr/interrupt.h>
#define _PROTECTED_WRITE(register, value);

void LCD_Init(void); // declaration from lab1
void USART1_init(void); //declaration from lab1
int16_t sourceCodeCharacter; //declaration from lab1
void writeChar(char ch, uint8_t pos); // declaration from lab 1
bool is_prime(long i); // declaration from lab 1


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
            //yield();
        }
    }
}

int main() {
    USART1_init();
    LCD_Init();
    spawn(computePrimes, 0);
    computePrimes(3);
}
