/*
* Timer 4 library, written for the ICROP monitor project, for AISER
* by Marcus Vinícius Martins Meneses
*
* dependencies:  none
* this library implements a datetime "rtc" for the atmega328pb core
* also it implements a scheduler for up to 10 tasks. note that tasks are cleared after execution
*/


#include <Arduino.h>
#include <avr/interrupt.h>


void (*fcptr)(void);

 
ISR (TIMER4_OVF_vect) {

	fcptr();
	TCNT4 = 63974; 
}
     

Timer4::Timer4(){}

Timer4::Timer4(TimeInfo * Timeholder){

fcptr = tick();
TCNT4 = 63974;
TCCR4A = 0x00;
TCCR4B = (1<<CS40) | (1<<CS41);
TIMSK4 = (1<<TOIE4);
sei();

}



bool Timer4::updateTime(uint32_t timestamp2){

timevar.timestamp = timestamp;

}

bool Timer4::tick(void){

timevar.timestamp++;

}


bool Timer4::parsetime(){


}
