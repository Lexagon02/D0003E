#include <avr/sfr_defs.h>
#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/portpins.h>
#include "tinythreads.h"
#include <stdint.h>
#include <stdbool.h>

#define NULL            0
#define DISABLE()       cli() //preventing interrupts
#define ENABLE()        sei() //enabling interrupts
#define STACKSIZE       80
#define NTHREADS        4
#define SETSTACK(buf,a) *((unsigned int *)(buf)+8) = (unsigned int)(a) + STACKSIZE - 4; \
                        *((unsigned int *)(buf)+9) = (unsigned int)(a) + STACKSIZE - 4

struct thread_block {
    void (*function)(int);   // code to run
    int arg;                 // argument to the above
    thread next;             // for use in linked lists
    jmp_buf context;         // machine state
    char stack[STACKSIZE];   // execution stack space
};

struct thread_block threads[NTHREADS];

struct thread_block initp;

thread freeQ   = threads;
thread readyQ  = NULL;
thread current = &initp;

ISR(PCINT1_vect){

    /* A PCINT1_vect interrupt can be generated by pressing the Butterfly
     * joystick in the downward direction, if the logical interrupt 
     * source PCINT15 is enabled in mask registers EIMSK and PCMSK1.
     * PCINT15 corresponds to a change on pin 7 of PORTB*/
    
    
    /*mask by looking for bits of port B if its equals 1 (ON)
     * This is the same as lab1 if (PINB & (1<<PB7)
     * where it test/checks if bit 7 on port b is 1 (joystick down)*/

    int16_t toggleBit7 = _BV(PB7); // the bit to check for toggle, initially 0	
    ENABLE(); // enable interrupt macro
    // PINB XOR bit value of PB7 eg., if it differs the XOR will change to true (1) 
    if (PINB ^ toggleBit7){   
        yield();
        //reti();
    }
    //PORTB ^= bitMaskPinToggle; //toggle the resistor state on and off

}

ISR(TIMER1_COMPA_vect){
    int16_t togglebitMatch = _BV(COM1A0);
    int_fast16_t togglebitClear = _BV(COM1A1);
    ENABLE();
    TCNT1 = 0x00; //clear the timer initially 
    TCCR1A |= _BV(CS12) | _BV(CS10); // sets the prescale to 1024
    TIMSK1 ^= _BV(1);
    TIFR1 ^= _BV(1);
    if (PINB ^ togglebitMatch{
        yield();
        TCRR1A ^= togglebitMatch;
        TCRR1A ^= togglebitClear; 
    }
}


volatile int16_t bitMaskPinToggle; //making global toggle variable volatile
int initialized = 0;

static void initialize(void) {
    /* XORING, toggles the bit in our case the 7th bit of PCMSK1 & EIMSK
    * registr at PCINT15 and PCIE1 = macro = 7.*/
    PCMSK1 ^= _BV(PCINT15);
    EIMSK ^= _BV(PCIE1);
    
    /* The PB7 pin can serve as an external interrupt source */
    bitMaskPinToggle = _BV(PB7); //(1<<PB7), pullup mask for changing state
    PORTB ^= bitMaskPinToggle; // toggle PB7
	DDRB |= (1<<DDB3)|(1<<DDB2)|(1<<DDB1)|(1<<DDB0); // alt: DDRB |= 0xFF;
    	 
    int i;
    for (i=0; i<NTHREADS-1; i++)
        threads[i].next = &threads[i+1];
    threads[NTHREADS-1].next = NULL;


    initialized = 1;
}


static void enqueue(thread p, thread *queue) {
    p->next = NULL;
    if (*queue == NULL) {
        *queue = p;
    } else {
        thread q = *queue;
        while (q->next)
            q = q->next;
        q->next = p;
    }
}

static thread dequeue(thread *queue) {
    thread p = *queue;
    if (*queue) {
        *queue = (*queue)->next;
    } else {
        // Empty queue, kernel panic!!!
        while (1) ;  // not much else to do...
    }
    return p;
}

static void dispatch(thread next) {
    if (setjmp(current->context) == 0) {
        current = next;
        longjmp(next->context,1);
    }
}

void spawn(void (* function)(int), int arg) {
    thread newp;

    DISABLE();
    if (!initialized) initialize();

    newp = dequeue(&freeQ);
    newp->function = function;
    newp->arg = arg;
    newp->next = NULL;
    if (setjmp(newp->context) == 1) {
        ENABLE();
        current->function(current->arg);
        DISABLE();
        enqueue(current, &freeQ);
        dispatch(dequeue(&readyQ));
    }
    SETSTACK(&newp->context, &newp->stack);

    enqueue(newp, &readyQ);
    ENABLE();
}

void yield(void) {
    enqueue(current, &readyQ);
    dispatch(dequeue(&readyQ));
    ENABLE();
}

void lock(mutex *m) {

}

void unlock(mutex *m) {

}
