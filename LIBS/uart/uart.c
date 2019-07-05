#include <config/config.h>


#include <avr/io.h>
#include <avr/interrupt.h>
#include <uart/uart.h>




void uart1_init(uint16_t ubrr){
/*Set baud rate */
UBRR1H = (unsigned char)(ubrr>>8);
UBRR1L = (unsigned char)(ubrr & 0xff);
UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(0<<UCSZ12);  //ENABLE ACTION
/* Set frame format: 8data, 1stop bit */
UCSR1A = (0<<U2X1); //1 to DOUBLE THE SPEED
UCSR1C = (1<<UCSZ10)|(1<<UCSZ11); //8 bit

return;
}

void uart0_init(uint16_t ubrr){
/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)(ubrr & 0xff);
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02);  //ENABLE ACTION 
/* Set frame format: 8data, 1stop bit */
UCSR0A = (0<<U2X0); //1 to DOUBLE THE SPEED
UCSR0C = (1<<UCSZ00)|(1<<UCSZ01); //8 bit

return;
}


void uart1_send(uint8_t data){
/* Wait for empty transmit buffer */
while ( !( UCSR1A & (1<<UDRE1)) )
;
/* Put data into buffer, sends the data */
UDR1 = data;
return;
}


void uart0_send(uint8_t data){
/* Wait for empty transmit buffer */
while ( !( UCSR0A & (1<<UDRE0)) )
;
/* Put data into buffer, sends the data */
UDR0 = data;
return;
}


//these receive functions are polled ones, and must be replaced by
//interrupt-driven io ASAP ---------------------------------------

uint8_t uart1_receive(){
/* Wait for data to be received */
while ( !(UCSR1A & (1<<RXC1)) )
;
/* Get and return received data from buffer */
return UDR1;
}


uint8_t uart0_receive(){
/* Wait for data to be received */
while ( !(UCSR0A & (1<<RXC0)) )
;
/* Get and return received data from buffer */
return UDR0;
}

//---------------------------------------------------------------


void uart1_flush(){
unsigned char dummy;
while ( UCSR1A & (1<<RXC1) ) dummy = UDR1;
return;
}


void uart0_flush(){
unsigned char dummy;
while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
return;
}

void uart1_end(){
UCSR1B = (0<<RXEN1)|(0<<TXEN1);
return;
}

void uart0_end(){
UCSR0B = (0<<RXEN0)|(0<<TXEN0);
return;
}


 




