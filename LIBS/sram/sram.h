#ifndef _SRAM_H_
#define _SRAM_H_

#include <spi/spi.h>
#include <inttypes.h>


void 	start_sram();
void 	end_sram();
uint8_t sram_transmit(uint8_t data);

uint8_t mem_read(uint32_t address);				//read byte to memory
void 	mem_write(uint32_t address, uint8_t data_byte);		//write byte to memory	
void 	mem_clear(uint32_t sta, uint32_t enda);			//clear memory area

void    mem_ssr(uint32_t address);				//start serial read
uint8_t mem_pop();						//"pop" char
void    mem_esr();						//end serial read	

void    mem_ssw(uint32_t address);				//start serial write
void    mem_push(uint8_t data);					//"push" char to memory 
void    mem_esw();						//end serial write


#endif // _SRAM_H_
