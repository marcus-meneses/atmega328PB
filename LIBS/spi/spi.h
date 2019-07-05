#ifndef _SPI_H_
#define _SPI_H_

#include <inttypes.h>



void spi1_init_master (void);
void spi1_begin(void); //set speed in the future
void spi1_end (void);
uint8_t spi1_transmit (uint8_t data);
void spi0_init_master (void);
void spi0_begin(void); //set speed in the future
void spi0_end (void);
uint8_t spi0_transmit (uint8_t data);



#endif // _SPI_H_
