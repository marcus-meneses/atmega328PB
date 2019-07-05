#ifndef Timer4_clock_h
#define Timer4_clock_h

 
#include <Arduino.h>
#include <avr/pgmspace.h>
 

#define EPOCH_HOURS			0
#define EPOCH_MINUTES			0
#define EPOCH_SECONDS			0
#define EPOCH_YEAR			1970
#define EPOCH_MONTH			1
#define EPOCH_DAY			1
#define EPOCH_WEEKDAY			5
#define EPOCH_SEASON_NORTH		3
#define EPOCH_SEASON_SOUTH		1
#define EPOCH_WEEKOFYEAR		1
#define EPOCH_TIMESTAMP			0

//common requests 
const char _hours[]   		PROGMEM = "horas";
const char _minutes[] 	      	PROGMEM = "minutos";
const char _seconds[] 	      	PROGMEM = "segundos";

const char _year[]        	PROGMEM = "ano";

const char _january[]       	PROGMEM = "janeiro";
const char _february[]       	PROGMEM = "fevereiro";
const char _march[]      	PROGMEM = "março";
const char _april[]   		PROGMEM = "abril";
const char _may[]  		PROGMEM = "maio";
const char _june[]        	PROGMEM = "junho";
const char _july[]        	PROGMEM = "julho";
const char _august[]        	PROGMEM = "agosto";
const char _september[]        	PROGMEM = "setembro";
const char _october[]        	PROGMEM = "outubro";
const char _november[]        	PROGMEM = "novembro";
const char _december[]        	PROGMEM = "dezembro";

const char _sunrday[]        	PROGMEM = "domingo"; //1
const char _monday[]        	PROGMEM = "segunda"; //2
const char _tuesday[]        	PROGMEM = "terça";   //3
const char _wednesday[]        	PROGMEM = "quarta";  //4
const char _thursday[]        	PROGMEM = "quinta";  //5
const char _friday[]        	PROGMEM = "sexta";   //6
const char _saturday[]       	PROGMEM = "sabado";  //7


const char _summer[]        	PROGMEM = "verao";	//1
const char _fall[]        	PROGMEM = "outono";     //2
const char _winter[]        	PROGMEM = "inverno";    //3
const char _spring[]        	PROGMEM = "primavera";  //4




typedef struct Ttime_structure {  //128 byte registry  O.O
   uint8_t hours;
   uint8_t minutes;
   uint8_t seconds;
   uint16_t year;
   uint8_t month;
   uint8_t day;
   uint8_t weekday;
   uint32_t timestamp;
} TimeInfo;


class Timer4 {
    
   public:
	Timer4();
	Timer4(TimeInfo * Timeholder);
	 
   bool updateTime(uint32_t timestamp2);
   bool tick(void);
   bool parsetime();
            		
   private:
 
	TimeInfo TimeVar;

};


#endif
