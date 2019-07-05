#include <config/config.h>

#include <spi/spi.h>
#include <sram/sram.h>
#include <inttypes.h>
#include <avr/io.h>



#if SPI_SRAM_PORT == 1
    

void start_sram() {
     DDRE|=(1<<2);  //SS1  -> PE2
     PORTE|=(1<<2); //SS1  -> PE2
     spi1_init_master();
     PORTE&=~(1<<2); //SS1  -> PE2
     return;
}

void end_sram() {
     DDRE|=(1<<2);  //SS1  -> PE2
     PORTE|=(1<<2); //SS1  -> PE2
     spi1_end();
     return;
}


uint8_t sram_transmit(uint8_t data) {
     return spi1_transmit (data);
}


#elif SPI_SRAM_PORT == 0
 #warning "using SPI PORT 0"

void start_sram() {
     DDRB|=(1<<2);  //SS0  -> PB2
     PORTB|=(1<<2); //SS0  -> PB2
     spi0_init_master();
     PORTB&=~(1<<2); //SS0  -> PB2
     return;
}


void end_sram(){
     DDRB|=(1<<2);  //SS0  -> PB2
     PORTB|=(1<<2); //SS0  -> PB2
     spi0_end();
     return;		
}


uint8_t sram_transmit(uint8_t data) {
      
     return spi0_transmit (data);
     
}

    
#else
    #error "UNAVAILABLE SPI PORT"
#endif



uint8_t mem_read(uint32_t address){
	start_sram();
	sram_transmit(SRAM_READ);
	sram_transmit( (uint8_t) (address>>16) & 0xff  );
        sram_transmit( (uint8_t) (address>>8) & 0xff  );
        sram_transmit( (uint8_t)  address );
        uint8_t ret = sram_transmit(0x00);  
        end_sram();
	return ret;
}

void 	mem_write(uint32_t address, uint8_t data_byte){
	start_sram();
	sram_transmit(SRAM_WRITE);
	sram_transmit( (uint8_t) (address>>16) & 0xff  );
        sram_transmit( (uint8_t) (address>>8) & 0xff  );
        sram_transmit( (uint8_t)  address );
        uint8_t ret = sram_transmit(data_byte);  
        end_sram();
	return;
}

void 	mem_clear(uint32_t sta, uint32_t enda){

	mem_ssw(sta);
        uint32_t i=0;
	for (i =0; i< enda-sta; i++) {
		mem_push(0x00);
	}
	
	mem_esw();
	return;
}

void    mem_ssr(uint32_t address){
	start_sram();
	sram_transmit(SRAM_READ);
	sram_transmit( (uint8_t) (address>>16) & 0xff  );
        sram_transmit( (uint8_t) (address>>8) & 0xff  );
        sram_transmit( (uint8_t)  address );
	return;
}

uint8_t mem_pop(){
	return sram_transmit(0x00); 
}

void    mem_esr(){
	end_sram();
	return;
}

void    mem_ssw(uint32_t address){
	start_sram();
	sram_transmit(SRAM_WRITE);
	sram_transmit( (uint8_t) (address>>16) & 0xff  );
        sram_transmit( (uint8_t) (address>>8) & 0xff  );
        sram_transmit( (uint8_t)  address );
	return;
}

void    mem_push(uint8_t data_byte){
	sram_transmit(data_byte);
	return;
}

void    mem_esw(){
	end_sram();
	return;
}
