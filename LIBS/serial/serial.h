/*
this serial library is higly integrated to aiser monitor system. odds are it will not fit any other purpose.
our serial buffers are written/read directly from external SRAM via SPI, freeing the uC memory for other maintenance
function data.
Serial0_input_base limit = 1024
We do not use any memory below 1024, this area is reserved for SRAM management, and is used by sram library only.
*/

#ifndef _SERIAL_H_
#define _SERIAL_H_


#include <inttypes.h>
#include <config/config.h>
#include <string.h>
#include <sram/sram.h>
#include <uart/uart.h>




 

void Serial0_begin(unsigned long baud);
void Serial1_begin(unsigned long baud);

void Serial0_end();
void Serial1_end();

void Serial1_print(uint8_t * pos);
void Serial1_println(uint8_t * pos);
void Serial0_print(uint8_t * pos);
void Serial0_println(uint8_t * pos);




#endif // _SERIAL_H_
