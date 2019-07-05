#ifndef _UART_H_
#define _UART_H_


#include <inttypes.h>
#include <config/config.h>
#include <string.h>


#ifndef F_CPU
	#warning F_CPU is undefined, UART may not work correctly without this
#endif

#define UBRR_UART1 F_CPU/16/BAUD_UART1-1

#define UBRR_UART0 (((F_CPU) + 4UL * (BAUD_UART0)) / (8UL * (BAUD_UART0)) -1UL)


void uart1_init(uint16_t ubrr);

void uart0_init(uint16_t ubrr);

void uart1_send(uint8_t data);

void uart0_send(uint8_t data);


//these receive functions are polled ones, and must be replaced by
//interrupt-driven io ASAP ---------------------------------------

uint8_t uart1_receive();


uint8_t uart0_receive();

//---------------------------------------------------------------


void uart1_flush();

void uart0_flush();

void uart1_end();

void uart0_end();





#endif // _UART_H_
