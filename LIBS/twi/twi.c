#include <config/config.h>


#include <avr/io.h>
#include <avr/interrupt.h>
#include <twi/twi.h>
#include <util/twi.h>


void TWI0_init(void){
	TWBR0 = (uint8_t)TWBR_TWI0;
}


void TWI1_init(void){
	TWBR1 = (uint8_t)TWBR_TWI1;
}


void TWI0_start(uint8_t addr){

	// reset TWI control register
	TWCR0 = 0;
	// transmit START condition 
	TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR0 & (1<<TWINT)) );
	
	// check if the start condition was successfully transmitted
	if((TWSR0 & 0xF8) != TW_START){ return 1; }
	
	// load slave address into data register
	TWDR0 = address;
	// start transmission of address
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR0 & (1<<TWINT)) );
	
	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;
	
	return 0;

}


void TWI1_start(uint8_t addr){


	// reset TWI control register
	TWCR1 = 0;
	// transmit START condition 
	TWCR1 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR1 & (1<<TWINT)) );
	
	// check if the start condition was successfully transmitted
	if((TWSR1 & 0xF8) != TW_START){ return 1; }
	
	// load slave address into data register
	TWDR1 = address;
	// start transmission of address
	TWCR1 = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR1 & (1<<TWINT)) );
	
	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;
	
	return 0;

}


uint8_t TWI0_write(uint8_t data){


	// load data into data register
	TWDR0 = data;
	// start transmission of data
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR0 & (1<<TWINT)) );
	
	if( (TWSR0 & 0xF8) != TW_MT_DATA_ACK ){ return 1; }
	
	return 0;

}


uint8_t TWI1_write(uint8_t data){


	// load data into data register
	TWDR1 = data;
	// start transmission of data
	TWCR1 = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR1 & (1<<TWINT)) );
	
	if( (TWSR1 & 0xF8) != TW_MT_DATA_ACK ){ return 1; }
	
	return 0;

}


uint8_t TWI0_read_ack(void) {

	// start TWI module and acknowledge data after reception
	TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	// wait for end of transmission
	while( !(TWCR0 & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR0;

}


uint8_t TWI1_read_ack(void) {

	// start TWI module and acknowledge data after reception
	TWCR1 = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	// wait for end of transmission
	while( !(TWCR1 & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR1;
}


uint8_t TWI0_read_nack(void) {

	// start receiving without acknowledging reception
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR0 & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR0;

}


uint8_t TWI1_read_nack(void) {

	// start receiving without acknowledging reception
	TWCR1 = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR1 & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR1;

}

void TWI0_stop(void){

	// transmit STOP condition
	TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

}


void TWI1_stop(void){

	// transmit STOP condition
	TWCR1 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}


