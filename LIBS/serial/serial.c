#include <config/config.h>


 
#include <avr/interrupt.h>
#include <string.h>
#include <sram/sram.h>
#include <uart/uart.h>
#include <serial/serial.h>


uint16_t Serial0_input_head=0;
uint16_t Serial0_input_tail=0;
uint16_t Serial0_input_count = 0;
uint8_t  Serial0_input_full = false;

uint16_t Serial0_output_head=0;
uint16_t Serial0_output_tail=0;
uint16_t Serial0_output_count = 0;
uint8_t  Serial0_output_full = false;

uint16_t Serial1_input_head=0;
uint16_t Serial1_input_tail=0;
uint16_t Serial1_input_count = 0;
uint8_t  Serial1_input_full = false;

uint16_t Serial1_output_head=0;
uint16_t Serial1_output_tail=0;
uint16_t Serial1_output_count = 0;
uint8_t  Serial1_output_full = false;

 
ISR(USART0_RX_vect)
{
    //uart0_send('k');
    //start_sram();
    //uart1_init();
    //reti();
}


ISR(USART1_RX_vect)
{
    //start_sram();
    //uart1_init();
    //reti();
}

 
void Serial0_begin(unsigned long baud){
//uint16_t ubrr =  (uint16_t)(((F_CPU) + 4UL * (baud)) / (8UL * (baud)) -1UL);
uint16_t ubrr =  (uint16_t) (F_CPU/(16*baud))-1;
uart0_init(ubrr);
}

void Serial1_begin(unsigned long baud){
//uint16_t ubrr =  (uint16_t)(((F_CPU) + 4UL * (baud)) / (8UL * (baud)) -1UL);
uint16_t ubrr =  (uint16_t) (F_CPU/(16*baud))-1;
uart1_init(ubrr);
}

void Serial0_end(){
uart0_end();
}

void Serial1_end(){
uart1_end();
} 



void Serial1_print(uint8_t * pos){
uint32_t i=0;
	for (i=0; i< strlen(pos); i++) {
	uart1_send(pos[i]);
	}
}

void Serial1_println(uint8_t * pos){
uint32_t i=0;
	for (i=0; i< strlen(pos); i++) {
	uart1_send(pos[i]);
	}
uart1_send('\r');
uart1_send('\n');
}


void Serial0_print(uint8_t * pos){
uint32_t i=0;
	for (i=0; i< strlen(pos); i++) {
	uart0_send(pos[i]);
	}
}

void Serial0_println(uint8_t * pos){
uint32_t i=0;
	for (i=0; i< strlen(pos); i++) {
	uart0_send(pos[i]);
	}
uart0_send('\r');
uart0_send('\n');
}

