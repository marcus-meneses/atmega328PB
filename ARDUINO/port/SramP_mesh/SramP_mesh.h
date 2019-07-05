#ifndef Sram_h
#define Sram_h

#include <Arduino.h>
#include <SPI.h>

#define SRAM_RDSR        5         // READ REGISTER
#define SRAM_WRSR        1         // WRITE REGISTER
#define SRAM_READ        3         // READ OPERATION
#define SRAM_WRITE       2         // WRITE OPERATION 

#define SRAM_OFFSET      0x00      // ADD something here to reserve space @0x00
#define SRAM_OK_MARK     0x21      // TO BE FOUND IN 0X00 ADDRESS IF MEMORY OK

#define SRAM_BASE_FREQ   8000000    // SRAM BASE FREQUENCY, 8MHz
#define SRAM_FREQ_1M     1000000   // 1MHz SPI frequency
#define SRAM_FREQ_2M     2000000   // 2MHz SPI frequency
#define SRAM_FREQ_4M     4000000   // 4MHz SPI frequency
#define SRAM_FREQ_8M     8000000   // 8MHz SPI frequency
                        //note: values above 8MHz are not defined and if necessary may be
                        //added below:


class Sram {

	public:
		 Sram();

		 Sram(SPIClass *periph, uint8_t CS);
		 void    	memclear();									//clears memory and puts it in started state 0x21
		 int8_t 	memcheck();									//0: not present, 1: ok, -1: uninitialized
		 
		 uint8_t memread(uint32_t address);						//read single byte from address
		 void 	 memwrite(uint32_t address, uint8_t data_byte); //write single byte to address
		 void    startSerialWrite(uint32_t address);			//start spi for sequential write
		 void    bytein(uint8_t data_byte);						//shift byte in memory
		 void    endSerialWrite();							    //end SPI serial write sequence
		 void    startSerialRead(uint32_t address);				//start spi for sequential read
		 uint8_t byteout();										//shift byte out
		 void    endSerialRead();								//end SPI serial read sequence
		 void    blockCopy(uint32_t startaddr, uint32_t endaddr, uint32_t size);
	         void    blockLoad(uint32_t startaddr, uint8_t * data , uint32_t size);
		 void    loadBlock(uint8_t * data, uint32_t startaddr , uint32_t size);
		 void    blockErase(uint32_t startaddr, uint32_t size);


		 // typed push-pops:
		 //bool 	fetchString (char * location);					//0: no string, 1:ok
		 //bool 	loadString (char * location);					//0: no room, 1:ok


	private:
		SPIClass *peripherial;	    	 //our SPI peripherial
		uint8_t  SPI_cs;	    	 //our chip select pin
		SPISettings settings;            // our spi settings object
		
		 void lock();		    	 //set SPI_cs low and wait for operations
		 void release();		 //set SPI_cs high and release use of port
		 uint8_t getByte(uint32_t num, uint8_t byteno); //helper function to decompose vars in bytes
};

#endif
