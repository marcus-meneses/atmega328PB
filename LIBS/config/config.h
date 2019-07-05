#ifndef _CFG_DEFS_H_
#define _CFG_DEFS_H_


#define F_CPU 16000000UL


//MEMORY ORGANIZATION

#define MAX_SERIAL_BUFFER_SIZE  1024
#define SERIAL_BUFFER_SIZE 	1024
#define BASE_BUFFER_POSITION    1024
#define SERIAL_0_IB             BASE_BUFFER_POSITION
#define SERIAL_0_OB		SERIAL_0_IB + SERIAL_BUFFER_SIZE
#define SERIAL_1_IB             SERIAL_0_OB + SERIAL_BUFFER_SIZE
#define SERIAL_1_OB		SERIAL_1_IB + SERIAL_BUFFER_SIZE

#define RADIO_PARSE_POS	    	3072      //parse incoming packet to this position, structured
#define RADIO_BUILD_POS     	4096      //build outgoing packet at this position, structured
#define CLIENTS_STRUC_POS   	5128      //up to 1KB of clients structs (about 128 clients)


//SRAM OPERATION CONSTANTS
#define SRAM_RDSR        	5         // READ REGISTER
#define SRAM_WRSR        	1         // WRITE REGISTER
#define SRAM_READ        	3         // READ OPERATION
#define SRAM_WRITE       	2         // WRITE OPERATION 


//Port Configuration
#define SPI_SRAM_PORT		1	  //AVAILABLE AT PORTS 1 OR 0, comment to disable

#define BAUD_UART1 		57600	  //Comment if not using port as serial
#define BAUD_UART0 		57600      //Comment if not using port as serial

#define FREQ_TWI1		100000	  //Comment if not using port as i2c
#define FREQ_TWI0		100000    //Comment if not using port as i2c

#define true			1
#define false			0








#endif //  _CFG_DEFS_H_
