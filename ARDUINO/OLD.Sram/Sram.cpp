/*
* 23LCV1024 library, written for the ICROP monitor project, foi AISER
* by Marcus Vinícius Martins Meneses
*
* notes: all operations are byte-wise, we still need to implement other alignments.
*		 strings are null-terminated memory chunks of unknown previous size (max-size:255).
*        a memory block implementation, with block number and usage will be made.
*/

#include <SPI.h>
#include <Sram.h>
#include <Arduino.h>

Sram::Sram(){}


Sram::Sram(SPI1Class *periph, uint8_t CS) {
    	peripherial = periph;
		SPI_cs = CS;
		release();
		pinMode(SPI_cs, OUTPUT);
		settings = SPISettings(SRAM_BASE_FREQ, MSBFIRST, SPI_MODE0);
		//peripherial->begin();
}

void Sram::loadPointers(){ 
	readPointer = (uint32_t) (memread(1)<<24) | (uint32_t) (memread(2)<16) | (uint32_t) (memread(3)<<8) | (uint32_t) (memread(4)); 
	writePointer  = (uint32_t) (memread(5)<<24) | (uint32_t) (memread(6)<16) | (uint32_t) (memread(7)<<8) | (uint32_t) (memread(8)); 
}



uint8_t Sram::getByte(uint32_t num, uint8_t byteno)
{
    uint8_t resp= (num >> ((3-byteno) * 8)) & 0xFF;    // Mask and return the required byte
    //Serial_2.println(resp,DEC);
    return resp;
}

void Sram::increaseWP(uint32_t siz){
 writePointer+=siz;


memwrite(5,  getByte(writePointer, 0) );
memwrite(6,  getByte(writePointer, 1) );
memwrite(7,  getByte(writePointer, 2) );
memwrite(8,  getByte(writePointer, 3) );


}

void Sram::increaseRP(uint32_t siz){
 readPointer+=siz;


memwrite(1,  getByte(readPointer, 0) );
memwrite(2,  getByte(readPointer, 1) );
memwrite(3,  getByte(readPointer, 2) );
memwrite(4,  getByte(readPointer, 3) );

  
}


void Sram::decreaseWP(uint32_t siz){
 writePointer-=siz;


memwrite(5,  getByte(writePointer, 0) );
memwrite(6,  getByte(writePointer, 1) );
memwrite(7,  getByte(writePointer, 2) );
memwrite(8,  getByte(writePointer, 3) );


}

void Sram::decreaseRP(uint32_t siz){
 readPointer-=siz;
 peripherial->begin();
memwrite(1,  getByte(readPointer, 0) );
memwrite(2,  getByte(readPointer, 1) );
memwrite(3,  getByte(readPointer, 2) );
memwrite(4,  getByte(readPointer, 3) );

  
}


void Sram::setWP(uint32_t siz){
 writePointer=siz;


memwrite(5,  getByte(writePointer, 0) );
memwrite(6,  getByte(writePointer, 1) );
memwrite(7,  getByte(writePointer, 2) );
memwrite(8,  getByte(writePointer, 3) );


}

void Sram::setRP(uint32_t siz){
 readPointer=siz;

memwrite(1,  getByte(readPointer, 0) );
memwrite(2,  getByte(readPointer, 1) );
memwrite(3,  getByte(readPointer, 2) );
memwrite(4,  getByte(readPointer, 3) );

  
}

void 	Sram::lock(){
	digitalWrite(SPI_cs,LOW);
	//DDRE = DDRE | 0x00000100;
	//PORTE = PORTE | 0x00000000;
}

void 	Sram::release(){
	digitalWrite(SPI_cs,HIGH);
	//DDRE = DDRE ^ 0x00000100;
	//PORTE = PORTE | 0x00000100;
}

void 	Sram::memclear(){
	startSerialWrite(0x00);
	for (uint32_t i=0; i<128000; i++){
		bytein(0x00);
	}
	endSerialWrite();
}

int8_t 	Sram::memcheck(){
	if (memread(0)!=0X21){
	 memwrite(0,0x23);
	   if(memread(0)!=0x23) return 0;
	   return -1;	
	}	
	return 1;
}


void    Sram::startSerialWrite(uint32_t address){
	peripherial->begin();
	lock();
	peripherial->transfer( SRAM_WRITE );
	peripherial->transfer( (uint8_t) (address >> 16) & 0xff);
	peripherial->transfer( (uint8_t) (address >> 8) & 0xff );
	peripherial->transfer( (uint8_t) address );
}


void    Sram::bytein(uint8_t data_byte){
	peripherial->transfer(data_byte);
}


void    Sram::endSerialWrite(){
	release();	
	peripherial->end();
}

void    Sram::startSerialRead(uint32_t address){
	peripherial->begin();
	lock();
	peripherial->transfer( SRAM_READ );
	peripherial->transfer( (uint8_t) (address >> 16) & 0xff);
	peripherial->transfer( (uint8_t) (address >> 8) & 0xff );
	peripherial->transfer( (uint8_t) address );
}

uint8_t Sram::byteout(){
	return peripherial->transfer(0x00);
}

void    Sram::endSerialRead(){
	release();	
	peripherial->end();

}	

uint8_t Sram::memread(uint32_t address){
	peripherial->begin();
	lock();
	peripherial->transfer( SRAM_READ );
	peripherial->transfer( (uint8_t) (address >> 16) & 0xff);
	peripherial->transfer( (uint8_t) (address >> 8) & 0xff );
	peripherial->transfer( (uint8_t) address );
	uint8_t ret = peripherial->transfer(0x00);
	release();	
	peripherial->end();
	return ret;
}

void Sram::memwrite(uint32_t address, uint8_t data_byte){
	peripherial->begin();
	lock();
	peripherial->transfer( SRAM_WRITE );
	peripherial->transfer( (uint8_t) (address >> 16) & 0xff );
	peripherial->transfer( (uint8_t) (address >> 8) & 0xff  );
	peripherial->transfer( (uint8_t) address );
	peripherial->transfer(data_byte);
	release();
	peripherial->end();
}

uint8_t Sram::byteread(uint32_t address){
  return memread(address+SRAM_OFFSET);
}

void Sram::bytewrite(uint32_t address, uint8_t data_byte){
   memwrite(address+SRAM_OFFSET, data_byte);
}
