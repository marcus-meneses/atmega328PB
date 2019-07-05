#ifndef _TWI_H_
#define _TWI_H_


#include <inttypes.h>


//scl1 PE1
//sda1 PE0
//scl0 PC5
//sda0 PC4
// ((F_CPU / frequency) - 16) / 2

#ifndef F_CPU
	#warning F_CPU is undefined, I2C may not work correctly without this
#endif

#ifndef FREQ_TWI1
	#warning TWI1 FREQ is undefined, TWI1 will not work correctly without this
#endif

#ifndef FREQ_TWI0
	#warning TWI0 FREQ is undefined, TWI0 will not work correctly without this
#endif

#define TWBR_TWI0 ((F_CPU / FREQ_TWI0) - 16) / 2
#define TWBR_TWI1 ((F_CPU / FREQ_TWI1) - 16) / 2

//--------------------------------------------------------------------------------

void TWI0_init(void);
void TWI1_init(void);
void TWI0_start(uint8_t addr);
void TWI1_start(uint8_t addr);
uint8_t TWI0_write(uint8_t data);
uint8_t TWI1_write(uint8_t data);
uint8_t TWI0_read_ack(void);
uint8_t TWI1_read_ack(void);
uint8_t TWI0_read_nack(void);
uint8_t TWI1_read_nack(void);
void TWI0_stop(void);
void TWI1_stop(void);




//-------------------------------------------------------------------------------


#endif // _TWI_H_
