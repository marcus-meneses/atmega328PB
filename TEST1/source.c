

//composite packets are up to 512 bytes in length, transmitted at frames of at most 64 bytes
//composite packets structure are as follows:
/*

FFFE -> start packet character
SOURCE[14]
DESTINATION[14]
FROM[14]
TO[14]
FFFD -> frame break character
PTYPE[1]
XTRA[6]
DATA[N]
FFFD -> frame break character
DATA[N]
FFFF -> end packet character
*/
//

#include <config/config.h>


#include <avr/pgmspace.h>
#include <string.h>
#include <i2c/i2c.h>
#include <spi/spi.h>
#include <sram/sram.h>
#include <uart/uart.h>
#include <serial/serial.h>

//#include "iocompat.h"       /* Note [1] */

uint8_t text[] = "this is a standard c string";
 


void setup(void){
Serial0_begin(57600);
Serial1_begin(57600);
mem_write(100, 'M');
//uart0_init(9600);
return;
}



void loop(void) {
 


Serial0_print( text );
Serial0_println( text );
Serial1_print( text );
Serial1_println( text );

uint8_t c=mem_read(100);


uart0_send(c);

//uint8_t c = uart0_receive();
//	while (c) {
//		uart0_send(c);
//		c = uart0_receive();
//	}
return;
}


int main (void)
{
 setup();

 while (1) {
 loop();
 uint32_t i;
 for (i=0;i<8000000;i++){}
 }  

}



