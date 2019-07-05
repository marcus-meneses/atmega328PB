#ifndef Radio_h
#define Radio_h

#include <SPI.h>
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Sram.h>
 

#define RADIO_BAUD 57600

const char _frequency[]   PROGMEM = "freq";
const char _fsk[] 	      PROGMEM = "fsk";

const char _vdd[] 	      PROGMEM = "vdd";
const char _reset[]       PROGMEM = "reset";
const char _sleep[]       PROGMEM = "sleep";
const char _analog[]      PROGMEM = "ana";
const char _digitalIn[]   PROGMEM = "digin";
const char _digitalOut[]  PROGMEM = "digout";
const char _spcr[]        PROGMEM = " ";
const char _channel_1[]   PROGMEM = "902";
const char _channel_2[]   PROGMEM = "906";
const char _channel_3[]   PROGMEM = "910";
const char _channel_4[]   PROGMEM = "914";
const char _channel_5[]   PROGMEM = "918";
const char _channel_6[]   PROGMEM = "922";
const char _channel_7[]   PROGMEM = "924";
const char _channel_8[]   PROGMEM = "928";
const char _megahertz[]   PROGMEM = "000000";
const char _pinmode[]	  PROGMEM = "pinmode";
const char _pindig[]	  PROGMEM = "pindig";
const char _GPIO[]	      PROGMEM = "GPIO";
const char _radio[]	      PROGMEM = "radio";
const char _mac[] 	      PROGMEM = "mac";
const char _pause[] 	  PROGMEM = "pause";
const char _resume[]      PROGMEM = "resume";
const char _set[] 	      PROGMEM = "set";
const char _sys[] 	      PROGMEM = "sys";
const char _get[] 	      PROGMEM = "get";
const char _ver[] 	      PROGMEM = "ver";
const char _tx[] 	      PROGMEM = "tx";
const char _rx[] 	      PROGMEM = "rx";
const char _bt[] 	      PROGMEM = "bt";
const char _mod[] 	      PROGMEM = "mod";
const char _lora[] 	      PROGMEM = "lora";
const char _freq[] 	      PROGMEM = "freq";
const char _pwr[] 	      PROGMEM = "pwr";
const char _sf[] 	      PROGMEM = "sf";
const char _afcbw[] 	  PROGMEM = "afcbw";
const char _rxbw[] 	      PROGMEM = "rxbw";
const char _bitrate[] 	  PROGMEM = "bitrate";
const char _prlen[] 	  PROGMEM = "prlen";
const char _crc[] 	      PROGMEM = "crc";
const char _iqi[] 	      PROGMEM = "iqi";
const char _wdt[] 	      PROGMEM = "wdt";
const char _sync[] 	      PROGMEM = "sync";
const char _bw[] 	      PROGMEM = "bw";
const char _snr[] 	      PROGMEM = "radio";
const char _commit[]      PROGMEM = "\r\n";



class Radio {
    
        public:
            Radio();
            Radio(HardwareSerial *porta, Sram  * memory, uint32_t InBoundOff, uint32_t OutBoundOff, uint8_t reset_pin);
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
            bool        setBandWidth(uint8_t bwidth);//both tx and rx
            void        writeCommand(const char *command);
            int         writeExtCommand(char *command);   
            void        pushChar(uint8_t character); //pushes character to output bufferPointer
            void        pushEChar(uint8_t character, uint8_t * buff); //pushes character to output bufferPointer
	    void        pushECChar(uint8_t * buff);
	    void        clearbuffP();
            void        pushEmptyChar(); //pushes character to output bufferPointer
            uint8_t     popChar();                   //pop character from input bufferPointer
            void        bufferReset();               //resets pointer and clears memory buffer;
            void        inBufferReset();             //resets pointer and clears memory buffer;
            uint16_t    sendBuffer();

            uint16_t    sendPacket();
            uint16_t    sendRamPacket(uint8_t * pos);
            bool        receivePacket(uint32_t time);
	    bool        receiveRamPacket(uint32_t time, uint8_t * pos);
	    int16_t   getRamResponse(uint32_t time);   //writes to outbuffer and returns size
            bool      checkRamResponse(char * okval); //0: error, 1: ok

            bool 	    wanPause();
            bool 	    wanResume();
            uint16_t 	    inBufferPointer;
            uint8_t         SerialNumber[7];
            
        private:
            
            uint8_t         resetpin;
            HardwareSerial  *port;	//our serial port to the radio boi
            Sram            *sram;

            uint32_t inOffset;  //from radio module
            uint32_t outOffset; //to radio module
            uint16_t bufferPointer;
            

	    uint8_t tmpb[255];
	    uint8_t outb[255];
            uint16_t tmpbp;
            uint16_t tmpbp2;
           
            int16_t   getResponse(uint32_t time);   //writes to outbuffer and returns size
            bool      checkResponse(char * okval); //0: error, 1: ok


            
            void loadSerialNumber();

    
    };


    #endif
