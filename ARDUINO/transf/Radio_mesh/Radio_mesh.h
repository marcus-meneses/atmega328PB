#ifndef Radio_h
#define Radio_h

#include <SPI.h>
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Sram_mesh.h>


/*packet formation
FROM(7);TO(7);FRAME_MAX(1);FRAME_ID(1);PAYLOAD_SIZE(3);PAYLOAD(PSIZE)

0000000 0000000 0 0 000 000...000

*/ 
 

#define RADIO_BAUD 57600
#define RADIO_OUTPUT_OFFSET_LOW   		512		//512 bytes for inbound frame (total)
#define RADIO_OUTPUT_OFFSET_HIGH  		1023		//512 bytes for inbound frame (total)

#define RADIO_INPUT_OFFSET_LOW  		1024		//512 bytes for outbound frame (total)		9876543210
#define RADIO_INPUT_PAYLOAD_OFFSET   		1034		//This is where the data actually is, 10 chars offset "radio_rx  "
#define RADIO_INPUT_OFFSET_HIGH 		1535		//512 bytes for outbound frame (total)


//common requests 
const char _frequency[]   	PROGMEM = "freq";
const char _fsk[] 	      	PROGMEM = "fsk";
const char _vdd[] 	      	PROGMEM = "vdd";
const char _reset[]       	PROGMEM = "reset";
const char _sleep[]       	PROGMEM = "sleep";
const char _analog[]      	PROGMEM = "ana";
const char _digitalIn[]   	PROGMEM = "digin";
const char _digitalOut[]  	PROGMEM = "digout";
const char _spcr[]        	PROGMEM = " ";

const char _channel_0[]   	PROGMEM = "928";  // fallback channel
const char _channel_1[]   	PROGMEM = "902";  // hop0-1 repeater - gateway
const char _channel_2[]   	PROGMEM = "904";  // hop0-2 node - node - repeater  
const char _channel_3[]   	PROGMEM = "906";  // hop1-1 repeater - gateway
const char _channel_4[]   	PROGMEM = "908";  // hop1-2 node - node - repeater 
const char _channel_5[]   	PROGMEM = "910";  // hop2-1 repeater - gateway
const char _channel_6[]   	PROGMEM = "912";  // hop2-2 node - node - repeater 
const char _channel_7[]   	PROGMEM = "914";  // hop3-1 repeater - gateway
const char _channel_8[]   	PROGMEM = "916";  // hop3-2 node - node - repeater 
const char _channel_9[]   	PROGMEM = "918";  //hi-priority 1
const char _channel_10[]   	PROGMEM = "920";  //hi-priority 2
const char _channel_11[]   	PROGMEM = "922";  //hi-priority 3
const char _channel_12[]   	PROGMEM = "924";  //low-priority 1
const char _channel_13[]   	PROGMEM = "926";  //low-priority 2


const char _megahertz[]   	PROGMEM = "000000";
const char _pinmode[]	  	PROGMEM = "pinmode";
const char _pindig[]	  	PROGMEM = "pindig";
const char _GPIO[]	      	PROGMEM = "GPIO";
const char _radio[]	      	PROGMEM = "radio";
const char _mac[] 	      	PROGMEM = "mac";
const char _pause[] 	  	PROGMEM = "pause";
const char _resume[]      	PROGMEM = "resume";
const char _set[] 	      	PROGMEM = "set";
const char _sys[] 	      	PROGMEM = "sys";
const char _get[] 	      	PROGMEM = "get";
const char _ver[] 	      	PROGMEM = "ver";
const char _tx[] 	      	PROGMEM = "tx";
const char _rx[] 	      	PROGMEM = "rx";
const char _bt[] 	      	PROGMEM = "bt";
const char _mod[] 	      	PROGMEM = "mod";
const char _lora[] 	      	PROGMEM = "lora";
const char _freq[] 	      	PROGMEM = "freq";
const char _pwr[] 	      	PROGMEM = "pwr";
const char _sf[] 	      	PROGMEM = "sf";
const char _afcbw[] 	  	PROGMEM = "afcbw";
const char _rxbw[] 	     	PROGMEM = "rxbw";
const char _bitrate[] 	  	PROGMEM = "bitrate";
const char _prlen[] 	  	PROGMEM = "prlen";
const char _crc[] 	      	PROGMEM = "crc";
const char _iqi[] 	      	PROGMEM = "iqi";
const char _wdt[] 	      	PROGMEM = "wdt";
const char _sync[] 	      	PROGMEM = "sync";
const char _bw[] 	      	PROGMEM = "bw";
const char _snr[] 	      	PROGMEM = "radio";
const char _commit[]      	PROGMEM = "\r\n";

// common responses
const char _answ_ok[]      	PROGMEM = "ok\r\n";
const char _answ_tx_ok[]      	PROGMEM = "radio_tx_ok\r\n";
const char _answ_rx_recv[]      PROGMEM = "radio_rx";
const char _answ_start[]      	PROGMEM = "RN2903";
const char _answ_wpause[]      	PROGMEM = "429";



class Radio {
    
   public:
		    Radio();
		    Radio(HardwareSerial *porta, Sram  * memory, uint8_t reset_pin);
	bool        sleep(uint32_t time);
	bool        reset();
	bool        hardReset();
	bool        radioPinMode(uint8_t pin, uint8_t mode);
	uint8_t     digitalPinRead(uint8_t pin);
	uint16_t    analogPinRead(uint8_t pin);            
	bool        pinWrite(uint8_t pin, uint8_t state); //digital
	uint16_t    readVcc();

	bool        setBt(uint8_t bt);
	bool        setMode(uint8_t modulation);
	bool        setFrequency(uint8_t channel);
	bool        setPower(uint8_t power);
	bool        setSpread(uint8_t factor);
	bool        setAutoBand(uint8_t afb);
	bool        setBitRate(uint32_t bitrate);
	bool        setFdev(uint32_t deviation);
	bool        setPreamble(uint16_t preamble);
	bool        setCrc(uint8_t crcstate);
	bool        setIqi(uint8_t iqistate);
	bool        setCr(uint8_t crval);
	bool        setWdt(uint32_t wdtimer);
	bool        setSyncWord(uint8_t synw);
	bool        setBandWidth(uint8_t bwidth);				//both tx and rx
	void        writeCommand(const char *command);
	int         writeExtCommand(char *command);
	uint16_t    sendBuffer();   
	
	void        pushChar(uint8_t character); 				//pushes character to output bufferPointer
	void        pushEmptyChar(); 						//pushes character 0 to output bufferPointer
	void        outBufferReset();               				//resets pointer and clears memory buffer;
	uint16_t    sendPacket();
	bool        receivePacket(uint32_t time);

	bool 	    wanPause();
	bool 	    wanResume();
	uint16_t    inBufferPointer;
	uint16_t    outBufferPointer;

            
   private:
            
	HardwareSerial  	*port;						//our serial port to the radio boi
	Sram            	*sram;
	uint8_t   		resetpin;


	int16_t   getResponse(uint32_t time);   				//writes to outbuffer and returns size
	bool      checkResponse(char * okval); 					//0: error, 1: ok
	bool      checkFSResponse(char * okval); 				//0: error, 1: ok
	

};


#endif
