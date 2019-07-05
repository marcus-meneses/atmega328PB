/*
* RN2903 LoRa library, written for the ICROP monitor project, for AISER
* by Marcus Vinícius Martins Meneses
*
* dependencies:  Sram 23LCV1024 library by the same author
*/


//strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i])));
#include <SPI.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <SramP_mesh.h>
#include <RadioP_mesh.h>
     

Radio::Radio(){}

Radio::Radio(HardwareSerial * porta, Sram  * memory, uint8_t reset_pin){
        port = porta;
        sram = memory;
        resetpin = reset_pin;
        inBufferPointer=0;
	outBufferPointer=0;
        pinMode(resetpin, OUTPUT);
        digitalWrite(resetpin,HIGH);
       
        //delay(100);
    }



bool	Radio::sleep(uint32_t time){
        char buffer[30];
        for (int i=0; i<30; i++) {
            buffer[i]='\0';
        }
        itoa(time, buffer, 10);
        writeCommand(_sys);
        writeCommand(_spcr);
        writeCommand(_sleep);
        writeCommand(_spcr);
        writeExtCommand(buffer);
        writeCommand(_commit);

        //returns: ok\r\n                -> if successful                                                                 
        //         invalid_param \r\n    -> if error
        if (getResponse(9999)==0) return 0;

        return checkResponse("ok\r\n");

    }




uint16_t   Radio::sendPacket(){
    
        writeCommand(_radio);
        writeCommand(_spcr);
        writeCommand(_tx);
        writeCommand(_spcr);
        uint16_t chcount = sendBuffer();
        writeCommand(_commit);

        //returns: ok\r\n                -> if successful
        //         invalid_param \r\n    -> if error
        if (getResponse(9999)==0) return 0;

        if (checkResponse("ok\r\n")){

		
            if (getResponse(9999)==0) return 0;

            if (checkResponse("radio_tx_ok\r\n")){

		return chcount;

	    }  else if (checkResponse("busy\r\n")) {
		delay(1000);
		return 0;
       	    }  else {

		return 0;

	    }


        } else if (checkResponse("busy\r\n")) {
		delay(1000);
		return 0;
        } else {

	}

    }





bool   Radio::receivePacket(uint32_t time){
        inBufferReset();
        char buffer[10];
        for (int i=0; i<10; i++) {
            buffer[i]='\0';
        }
        ltoa(time, buffer, 10);

	writeCommand(_radio);
	writeCommand(_spcr);
	writeCommand(_rx);
	writeCommand(_spcr);
        writeExtCommand(buffer);
	//writeExtCommand("0");
	writeCommand(_commit);
    
            //returns: ok\r\n                -> if successful
            //         invalid_param \r\n    -> if error
            if (getResponse(9999)==0) return 0;

            if (!checkResponse("ok\r\n")){

                return false;

            } else {

                if (getResponse(9999)==0) return 0;
                return checkResponse("radio_rx");

            }
    
        }



bool	Radio::reset(){
        writeCommand(_sys);
        writeCommand(_spcr);
        writeCommand(_reset);
        writeCommand(_commit);

        //returns: RN2903 \r\n                -> if successful
        if (getResponse(9999)==0) return 0;
        return checkResponse("RN2903");
    }




bool	Radio::hardReset(){
        pinMode(resetpin, OUTPUT);
        digitalWrite(resetpin,LOW);
        delay(300);
        digitalWrite(resetpin,HIGH);
        delay(300);

        //returns: RN2903 \r\n                -> if successful
        if (getResponse(9999)==0) return 0;
        return checkResponse("RN2903");
    }




bool    Radio::radioPinMode(uint8_t pin, uint8_t mode){ //pin: 0 to 14, mode: 1:analog 2: digout, 3: digin

        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i]='\0';
        }
        itoa(pin, buffer, 10);

        writeCommand(_sys);
        writeCommand(_spcr);
        writeCommand(_set);
        writeCommand(_spcr);
        writeCommand(_pinmode);
        writeCommand(_spcr);
        writeCommand(_GPIO);
        writeExtCommand(buffer);
        writeCommand(_spcr);
        if (mode==1) {
            writeCommand(_analog);
        } else if (mode==2) {
            writeCommand(_digitalOut);
        } else {
            writeCommand(_digitalIn); 
        }
        writeCommand(_commit);
        
        //returns: ok\r\n                -> if successful
        //         invalid_param \r\n    -> if error
        if (getResponse(9999)==0) return 0;
        return checkResponse("ok\r\n");

    }



uint8_t     Radio::digitalPinRead(uint8_t pin){


    }





uint16_t    Radio::analogPinRead(uint8_t pin){


    }   




bool    Radio::pinWrite(uint8_t pin, uint8_t state){

        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i]='\0';
        }
        itoa(pin, buffer, 10);


        writeCommand(_sys); 
        writeCommand(_spcr);
        writeCommand(_set);
        writeCommand(_spcr);
        writeCommand(_pindig);
        writeCommand(_spcr);
        writeCommand(_GPIO);
        writeExtCommand(buffer);
        writeCommand(_spcr);

        for (int i=0; i<5; i++) {
            buffer[i]='\0';
        }
        itoa(state, buffer, 10);
        writeExtCommand(buffer);
        writeCommand(_commit);

        //returns: ok\r\n                -> if successful
        //         invalid_param \r\n    -> if error
        if (getResponse(9999)==0) return 0;
        return checkResponse("ok\r\n");

    }



uint16_t	Radio::readVcc(){
        uint8_t ansize=0;
        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i]='\0';
        }
        writeCommand(_sys); 
        writeCommand(_spcr);
        writeCommand(_get);
        writeCommand(_spcr);
        writeCommand(_vdd);
        writeCommand(_commit);
        ansize=getResponse(99);
        if (ansize==0) return 0;
        for (int j=0; j<ansize; j++){
            char getcha = sram->memread(RADIO_INPUT_OFFSET_LOW+j);
            if ((getcha=='\r') || (getcha=='\n'))  {
              getcha = '\0';  
            }
            buffer[j] = getcha;
        }
        return atoi(buffer);
    }




bool	Radio::setBt(uint8_t bt){
        // not available for LORA modulation

    }




bool	Radio::setMode(uint8_t modulation){
        writeCommand(_radio); 
        writeCommand(_spcr);
        writeCommand(_set);
        writeCommand(_spcr);
        writeCommand(_mod);
        writeCommand(_spcr);
        if (modulation==1) {
           writeCommand(_lora); 
        } else {
            writeCommand(_fsk); 
        }
        writeCommand(_commit);
        if (getResponse(99999)==0) return 0;
        return checkResponse("ok\r\n");
    }




bool    Radio::setFrequency(uint8_t channel){
        writeCommand(_radio); 
        writeCommand(_spcr);
        writeCommand(_set);
        writeCommand(_spcr);
        writeCommand(_frequency);
        writeCommand(_spcr);

        switch (channel) {
        case 0:
            writeCommand(_channel_0);
            break;
        case 1:
            writeCommand(_channel_1);
            break;
        case 2:
            writeCommand(_channel_2);
            break;
        case 3:
            writeCommand(_channel_3);
            break;
        case 4:
            writeCommand(_channel_4);
            break;
        case 5:
            writeCommand(_channel_5);
            break;
        case 6:
            writeCommand(_channel_6);
            break;
        case 7:
            writeCommand(_channel_7);
            break;
        case 8:
            writeCommand(_channel_8);
            break;
        case 9:
            writeCommand(_channel_9);
            break;
        case 10:
            writeCommand(_channel_10);
            break;
        case 11:
            writeCommand(_channel_11);
            break;
        case 12:
            writeCommand(_channel_12);
            break;
        case 13:
            writeCommand(_channel_13);
            break;
        default:
            writeCommand(_channel_0);
        }

        writeCommand(_megahertz);
        writeCommand(_commit);

        if (getResponse(9999)==0) return 0;
        return checkResponse("ok\r\n");
    }

    


bool	Radio::setPower(uint32_t power){

        if (power>17) power=17;
        if (power<2) power = 2;

        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i]='\0';
        }
         ltoa(power, buffer, 10);
	

        writeCommand(_radio); 
        writeCommand(_spcr);
        writeCommand(_set);
        writeCommand(_spcr);
        writeCommand(_pwr);
        writeCommand(_spcr);
        writeExtCommand(buffer);
        writeCommand(_commit);

        if (getResponse(9999)==0) return 0;
        return checkResponse("ok\r\n");

    }


    
bool	Radio::setSpread(uint8_t factor){


    }
   
 
    
bool	Radio::setAutoBand(uint8_t afb){
        // not available for lora modulation

    }
   
 
   
bool	Radio::setBitRate(uint32_t bitrate){
        // not available for lora modulation

    }

    

bool	Radio::setFdev(uint32_t deviation){
        // not available for lora modulation

    }

   
 
bool	Radio::setPreamble(uint16_t preamble){
        // not available for lora modulation

    }

  
  
bool	Radio::setCrc(uint8_t crcstate){


    }

   
 
bool	Radio::setIqi(uint8_t iqistate){


    }


    
bool	Radio::setCr(uint8_t crval){


    }


    
bool	Radio::setWdt(uint32_t wdtimer){

        char buffer[10];
        for (int i=0; i<10; i++) {
            buffer[i]='\0';
        }
        ltoa(wdtimer, buffer, 10);
  
        writeCommand(_radio); 
        writeCommand(_spcr);
        writeCommand(_set);
        writeCommand(_spcr);
        writeCommand(_wdt);
        writeCommand(_spcr);
        writeExtCommand(buffer);
        writeCommand(_commit);

        if (getResponse(9999)==0) return 0;
        return checkResponse("ok\r\n");

    }

  
  
bool        Radio::setSyncWord(uint8_t synw){


    }
   
 
   
bool        Radio::setBandWidth(uint8_t bwidth) { //both tx and rx
    
    }

    


void     Radio::writeCommand(const char *command){
    char buffer[20];
        strcpy_P(buffer, (char*)command);
	//Serial.print(">");
        for (uint16_t i=0; i<strlen(buffer); i++) {
            port->print(buffer[i]);
	     //REMOVER
		//Serial.write(buffer[i]); 	
        }

    }

 
   
int     Radio::writeExtCommand(char *command){ 
            int ctr = 0;   
            for (uint16_t i=0; i<strlen(command); i++) {
                port->print(command[i]);
	     //REMOVER
		//Serial.write(command[i]); 
              ctr++;
            }
          return ctr;
        }


    

uint16_t    Radio::sendBuffer(){


	//sram->memwrite(RADIO_OUTPUT_OFFSET_LOW+1, 77);
	

        sram->startSerialRead(RADIO_OUTPUT_OFFSET_LOW);
	//Serial.println();//Serial.print(F("SSR "));//Serial.println(RADIO_OUTPUT_OFFSET_LOW);
        char grab=10;
        uint16_t ctt = 0;
       // grab = sram->byteout();

        while (grab!=0) {
	    grab = sram->byteout();	
            port->print(grab);
            


				//REMOVER
				//Serial.write(grab);

	    //delay(1);
	   ctt++;
           if (ctt>255) {
	   return 0;	
           }
        }
        
        sram->endSerialRead();
	return ctt/2;
        
    }


    
void	Radio::pushChar(uint8_t character){
        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i]='\0';
        }
        sprintf(buffer, "%02X", character);
	////Serial.print(buffer);//Serial.print(' ');
        sram->memwrite(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer, buffer[0]);
	//Serial.print(" @");//Serial.print(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer);
	//Serial.print(" ");//Serial.write(character);//Serial.print("->");//Serial.write(buffer[0]);
        outBufferPointer++;
        sram->memwrite(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer, buffer[1]);
	//Serial.write(buffer[1]);//Serial.print(" ");
        outBufferPointer++;
	////Serial.println();
    }



bool 	 Radio::seekAndPush(uint8_t * str, uint8_t count){
	 uint8_t i=0;
         //Serial.println();//Serial.print(F("BSP "));//Serial.print(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer);//Serial.print(F("   "));
	 for (i=0;i<count;i++){
		pushChar(str[i]);
		//Serial.print(" was[");//Serial.print(str[count],HEX);
		//Serial.print("] (");
		//Serial.write(sram->memread(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer-2));//Serial.print("-");
		//Serial.write(sram->memread(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer-1));
		//Serial.print(") ");
		}
	//Serial.println();

	}

    

void	Radio::pushEmptyChar(){
 
        sram->memwrite(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer, 0);
        outBufferPointer++;
	//Serial.println();//Serial.print(F("WEC "));//Serial.print(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer-1);//Serial.print(F("   "));//Serial.println(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer);
        sram->memwrite(RADIO_OUTPUT_OFFSET_LOW+outBufferPointer, 0);
        outBufferPointer++;
    }


    
   
void	Radio::outBufferReset(){
        outBufferPointer = 0;
        sram->startSerialWrite(RADIO_OUTPUT_OFFSET_LOW);
	//Serial.println();//Serial.print(F("SSCO "));//Serial.println(RADIO_OUTPUT_OFFSET_LOW);

        for (int i=0; i<512; i++) {
            sram->bytein(0);
		//Serial.print(RADIO_OUTPUT_OFFSET_LOW+i);//Serial.print(' ');
		//if (((i+1) % 128)==0) Serial.println();
        }


        sram->endSerialWrite();
    }  

void	Radio::inBufferReset(){
        inBufferPointer = 0;
        sram->startSerialWrite(RADIO_INPUT_OFFSET_LOW);
	//Serial.println();//Serial.print(F("SSCI "));//Serial.println(RADIO_INPUT_OFFSET_LOW);

        for (int i=0; i<512; i++) {
            sram->bytein(0);
	    	//Serial.print(RADIO_INPUT_OFFSET_LOW+i);;//Serial.print(' ');
		//if (((i+1) % 128)==0) //Serial.println();
        }

        sram->endSerialWrite();
    }  


    
   


    
int16_t   Radio::getResponse(uint32_t time){ //writes to inbuffer and returns size 
		uint16_t ccount=0;
		uint16_t counter=0;
		while(! port->available() ) {
			counter++;
			delay(10);
			if(counter>(time*100)) return 0;
		}

		if(port->available()){ //hook it as soon as available  (i think this is unnecessary)
			sram->startSerialWrite(RADIO_INPUT_OFFSET_LOW);
	       		char e;

            	//Serial.println();
		//Serial.print(F("SGR  "));
			while(port->available()){  
				e=port->read();
				//sram->memwrite(RADIO_INPUT_OFFSET_LOW+ccount, e);


				//REMOVER
				//Serial.write(e);

				sram->bytein(e);
				inBufferPointer++;
				ccount++;
				delay(1); //evaluate the need of this delay here
			}
              //Serial.println();
			//sram->bytein('\0');
			sram->endSerialWrite();
			////Serial.println(ccount);
			return ccount;
		}

 
}
    
    
bool	Radio::checkResponse(char * okval){
		uint8_t buffer[20];
		uint8_t mxlgt = 0;

		for (uint8_t idx=0; idx<20; idx++) {
			buffer[idx]='\0';
		}

		int i=0;

		sram->startSerialRead(RADIO_INPUT_OFFSET_LOW);
                
			for (i=0; i< strlen((char *)okval); i++){
				//buffer[i] = sram->memread(RADIO_INPUT_OFFSET_LOW+i);
				buffer[i] = sram->byteout();
			}

		if (strlen((char *)okval)>strlen((char *)buffer)) mxlgt = strlen((char *)buffer);
		sram->endSerialRead();
		buffer[i] = '\0';
		 
            
		if(strncmp((const char *)buffer, (const char *)okval, mxlgt )!=0) {
			return false;
		} else {
			return true;
		}

}
    

bool	Radio::checkFSResponse(char *okval){

	char buffer[20];
	char bufferSecond[20];
	uint8_t indx=0;
        for (indx=0; indx<20; indx++) {
               buffer[indx]='\0';
	       bufferSecond[indx]='\0';
        }

        strcpy_P(buffer, (char*)okval);

	sram->startSerialRead(RADIO_INPUT_OFFSET_LOW);
		for (uint16_t idd=0; idd< strlen(okval); idd++){
                    //buffer[i] = sram->memread(RADIO_INPUT_OFFSET_LOW+i);
                    bufferSecond[idd] = sram->byteout();
                }

	sram->endSerialRead();
	bufferSecond[indx] = '\0';
	

	if(strcmp((const char *)buffer, (const char *)bufferSecond)!=0) {
		return false;
	} else {
		return true;
	}


}



bool	Radio::wanPause(){
        writeCommand(_mac);
        writeCommand(_spcr);
        writeCommand(_pause);
        writeCommand(_commit); 
        if (getResponse(9999)==0) return 0;
        return checkResponse("42949672");
}


    
bool	Radio::wanResume(){
        writeCommand(_mac);
        writeCommand(_spcr);
        writeCommand(_resume);
        writeCommand(_commit);
        if (getResponse(9999)==0) return 0;
        return checkResponse("ok\r\n");
}
