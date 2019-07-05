#ifndef Sram_h
#define Sram_h

#include <Arduino.h>
#include <SPI.h>

#define SRAM_RDSR        5         // READ REGISTER
#define SRAM_WRSR        1         // WRITE REGISTER
#define SRAM_READ        3         // READ OPERATION
#define SRAM_WRITE       2         // WRITE OPERATION 

#define SRAM_OFFSET      1024      // FIRST 5KB ARE Structured headers
#define SRAM_OK_MARK     0x21      // TO BE FOUND IN 0X00 ADDRESS IF MEMORY OK

#define SRAM_BASE_FREQ   500000   // SRAM BASE FREQUENCY, 4MHz
#define SRAM_FREQ_1M     1000000   // 1MHz SPI frequency
#define SRAM_FREQ_2M     2000000   // 2MHz SPI frequency
#define SRAM_FREQ_4M     4000000   // 4MHz SPI frequency
#define SRAM_FREQ_8M     8000000   // 8MHz SPI frequency
                        //note: values above 8MHz are not defined and if necessary may be
                        //added below:


#define _SPI1 1 // change this to SPI0 for SPI0 USAGE


#ifdef _SPI1
 
	typedef SPI1Class SPI_Class;

#else

	typedef SPI0Class SPI_Class;

#endif

//SRAM_OFFSET
class Sram {

	public:
		 Sram();

		 Sram(SPI1Class *periph, uint8_t CS);
		 void    	memclear();									//clears memory and puts it in started state 0x21
		 int8_t 	memcheck();									//0: not present, 1: ok, -1: uninitialized

		 //offset-unaffected versions:
		 uint8_t memread(uint32_t address);						//read single byte from address
		 void 	 memwrite(uint32_t address, uint8_t data_byte); //write single byte to address
		 //offset-affected versions:
		 uint8_t byteread(uint32_t address);					//read single byte from address
		 void 	 bytewrite(uint32_t address, uint8_t data_byte);//write single byte to address

		 void    startSerialWrite(uint32_t address);			//start spi for sequential write
		 void    bytein(uint8_t data_byte);						//shift byte in memory
		 void    endSerialWrite();							    //end SPI serial write sequence
		 void    startSerialRead(uint32_t address);				//start spi for sequential read
		 uint8_t byteout();										//shift byte out
		 void    endSerialRead();								//end SPI serial read sequence

		 // typed push-pops:
		 bool 	fetchString (char * location);					//0: no string, 1:ok
		 bool 	loadString (char * location);					//0: no room, 1:ok


	//private:
		uint32_t readPointer;       	//for sequential access
		uint32_t writePointer;	    	//for sequential access
		SPI1Class *peripherial;	    	//our SPI peripherial
		uint8_t  SPI_cs;	    	    //our chip select pin
		SPISettings settings;           // our spi settings object
		
		 void lock();		    	    //set SPI_cs low and wait for operations
		 void release();		    	//set SPI_cs high and release use of port
		 void loadPointers();	    	//loads pointers from non volatile to ram
		 void setRP(uint32_t siz);   	//set Read pointer (bytes 1,2,3,4 of memory);
		 void setWP(uint32_t siz);   	//set Write pointer (bytes 5,6,7,8 of memory);
		 void decreaseRP(uint32_t siz);	//decrease read pointer by parameter
		 void decreaseWP(uint32_t siz);	//decrease write pointer by parameter
		 void increaseRP(uint32_t siz);  //increase read pointer by parameter
		 void increaseWP(uint32_t siz);	//increase write pointer by parameter
		 uint8_t getByte(uint32_t num, uint8_t byteno); //helper function to decompose vars in bytes
};

#endif
