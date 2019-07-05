/*
* 23LCV1024 library, written for the ICROP monitor project, for AISER
* by Marcus Vinícius Martins Meneses
*/

#include <SPI.h>
#include <Sram_mesh.h>
#include <Arduino.h>

Sram::Sram(){}


Sram::Sram(SPI1Class *periph, uint8_t CS) {
    	peripherial = periph;
		SPI_cs = CS;
		release();
		pinMode(SPI_cs, OUTPUT);
		settings = SPISettings(SRAM_BASE_FREQ, MSBFIRST, SPI_MODE0);
		//peripherial->begin();
         if (memcheck()==-1) {
		memclear();
	 }
}


//clears memory from 0x00 to 128000.
//sets check byte for memory OK
void 	Sram::memclear(){
	startSerialWrite(0x00);
	bytein(SRAM_OK_MARK);
	for (uint32_t i=1; i<128000; i++){
		bytein(0x00);
	}
	endSerialWrite();
}

//  1 -> memory ok
// -1 -> memory uninitialized (please clr)
//  0 -> memory error
int8_t 	Sram::memcheck(){
	if (memread(0)!=SRAM_OK_MARK){
	 memwrite(0,0x23);
	   if(memread(0)!=0x23) return 0;
	   return -1;	
	}	
	return 1;
}


//returns byte read @address
uint8_t Sram::memread(uint32_t address){
	peripherial->begin();
	lock();
	address = address + SRAM_OFFSET;
	peripherial->transfer( SRAM_READ );
	peripherial->transfer( (uint8_t) (address >> 16) & 0xff);
	peripherial->transfer( (uint8_t) (address >> 8) & 0xff );
	peripherial->transfer( (uint8_t) address );
	uint8_t ret = peripherial->transfer(0x00);
	release();	
	peripherial->end();
	return ret;
}


//writes byte data_byte @address
void Sram::memwrite(uint32_t address, uint8_t data_byte){
	peripherial->begin();
	lock();
	address = address + SRAM_OFFSET;
	peripherial->transfer( SRAM_WRITE );
	peripherial->transfer( (uint8_t) (address >> 16) & 0xff );
	peripherial->transfer( (uint8_t) (address >> 8) & 0xff  );
	peripherial->transfer( (uint8_t) address );
	peripherial->transfer(data_byte);
	release();
	peripherial->end();
}


//starts a serial write event
void    Sram::startSerialWrite(uint32_t address){
	peripherial->begin();
	lock();
	address = address + SRAM_OFFSET;
	peripherial->transfer( SRAM_WRITE );
	peripherial->transfer( (uint8_t) (address >> 16) & 0xff);
	peripherial->transfer( (uint8_t) (address >> 8) & 0xff );
	peripherial->transfer( (uint8_t) address );
}

//pushes data to sram
void    Sram::bytein(uint8_t data_byte){
	peripherial->transfer(data_byte);
}

//ends serial write event
void    Sram::endSerialWrite(){
	release();	
	peripherial->end();
}

//starts a serial read event
void    Sram::startSerialRead(uint32_t address){
	peripherial->begin();
	lock();
	address = address + SRAM_OFFSET;
	peripherial->transfer( SRAM_READ );
	peripherial->transfer( (uint8_t) (address >> 16) & 0xff);
	peripherial->transfer( (uint8_t) (address >> 8) & 0xff );
	peripherial->transfer( (uint8_t) address );
}


//pops data from sram
uint8_t Sram::byteout(){
	return peripherial->transfer(0x00);
}


//ends serial read event
void    Sram::endSerialRead(){
	release();	
	peripherial->end();

}


//copies a block of memory of <size> from startaddr to endaddr
void    blockCopy(uint32_t startaddr, uint32_t endaddr, uint32_t size){

	for (uint32_t cursor=0; cursor<size; cursor++){
	        memwrite(endaddr+cursor,memread(startaddr+cursor));
	}

}

//loads sized array from memory block
void    blockLoad(uint32_t startaddr, uint8_t * data , uint32_t size) {
	startSerialRead(startaddr);
	for (uint32_t cursor=0; cursor<size; cursor++){
	          data[cursor] = byteout();
	}
	endSerialRead();
}

//sends sized  array to memory block
void    loadBlock(uint8_t * data, uint32_t startaddr , uint32_t size) {
	startSerialWrite(startaddr);
	for (uint32_t cursor=0; cursor<size; cursor++){
	          bytein(data[cursor]);
	}
	endSerialWrite();
}


//clears a block of memory of <size> starting from startaddr
void    blockErase(uint32_t startaddr, uint32_t size){

	startSerialWrite(startaddr);
		for (uint32_t cursor=0; cursor<size; cursor++){
			bytein(0x00);
		}
	endSerialWrite()
}


//locks SPI port for single usage
void 	Sram::lock(){
	digitalWrite(SPI_cs,LOW);
	//DDRE = DDRE | 0x00000100;
	//PORTE = PORTE | 0x00000000;
}


//unlocks SPI port for single usage
void 	Sram::release(){
	digitalWrite(SPI_cs,HIGH);
	//DDRE = DDRE ^ 0x00000100;
	//PORTE = PORTE | 0x00000100;
}


//retuns byte(s) value(s) from 32-bit integer
uint8_t Sram::getByte(uint32_t num, uint8_t byteno)
{
    uint8_t resp= (num >> ((3-byteno) * 8)) & 0xFF;    // Mask and return the required byte
    //Serial_2.println(resp,DEC);
    return resp;
}

	


 
