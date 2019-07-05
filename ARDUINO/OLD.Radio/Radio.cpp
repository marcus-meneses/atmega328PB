
//strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i])));
#include <SPI.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Sram.h>
#include <Radio.h>
     

Radio::Radio(){}

    Radio::Radio(HardwareSerial * porta, Sram  * memory, uint32_t InBoundOff, uint32_t OutBoundOff, uint8_t reset_pin){
        port = porta;
        sram = memory;
        resetpin = reset_pin;
        inOffset = InBoundOff;
        outOffset = OutBoundOff;
        bufferPointer=0;
        loadSerialNumber();
        port->begin(RADIO_BAUD);
         pinMode(resetpin, OUTPUT);
  
         digitalWrite(resetpin,HIGH);
       
        //delay(100);
    }

   bool        Radio::sleep(uint32_t time){
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
        if (getResponse(99)==0) return 0;
        return checkResponse("ok\r\n");

    }


    uint16_t        Radio::sendPacket(){
    
        writeCommand(_radio);
        writeCommand(_spcr);
        writeCommand(_tx);
        writeCommand(_spcr);
        uint16_t chcount = sendBuffer();
        writeCommand(_commit);

        //returns: ok\r\n                -> if successful
        //         invalid_param \r\n    -> if error
        if (getResponse(99)==0) return 0;
        if (!checkResponse("ok\r\n")){
            return false;
        } else {
            if (getResponse(99)==0) return 0;
            if (checkResponse("radio_tx_ok\r\n")){
		return chcount;
	    } else {
		return 0;
		}

        }

    }





uint16_t    Radio::sendRamPacket(uint8_t * pos){
    
        writeCommand(_radio);
        writeCommand(_spcr);
        writeCommand(_tx);
        writeCommand(_spcr);
        //uint16_t chcount = sendBuffer();
	uint16_t chcount = writeExtCommand((char *)pos);
        writeCommand(_commit);

        //returns: ok\r\n                -> if successful
        //         invalid_param \r\n    -> if error
        if (getResponse(99)==0) return 0;
        if (!checkResponse("ok\r\n")){
            return false;
        } else {
            if (getResponse(99)==0) return 0;
            if (checkResponse("radio_tx_ok\r\n")){
		return chcount;
	    } else {
		return 0;
		}

        }

    }





    bool        Radio::receivePacket(uint32_t time){
        inBufferReset();
        char buffer[10];
        for (int i=0; i<10; i++) {
            buffer[i]='\0';
        }
        itoa(time, buffer, 10);

            writeCommand(_radio);
            writeCommand(_spcr);
            writeCommand(_rx);
            writeCommand(_spcr);
            writeExtCommand("0");
            writeCommand(_commit);
    
            //returns: ok\r\n                -> if successful
            //         invalid_param \r\n    -> if error
            if (getResponse(99)==0) return 0;
            if (!checkResponse("ok\r\n")){
                return false;
            } else {
                if (getResponse(9999)==0) return 0;
                return checkResponse("radio_rx");
            }
    
        }



    bool        Radio::reset(){
        writeCommand(_sys);
        writeCommand(_spcr);
        writeCommand(_reset);
        writeCommand(_commit);

        //returns: RN2903 \r\n                -> if successful
        if (getResponse(9999)==0) return 0;
        return checkResponse("RN2903");
    }

    bool        Radio::hardReset(){
        pinMode(resetpin, OUTPUT);
        digitalWrite(resetpin,LOW);
        delay(300);
        digitalWrite(resetpin,HIGH);
        delay(300);

        //returns: RN2903 \r\n                -> if successful
        if (getResponse(9999)==0) return 0;
        return checkResponse("RN2903");
    }

    bool       Radio::radioPinMode(uint8_t pin, uint8_t mode){ //pin: 0 to 14, mode: 1:analog 2: digout, 3: digin

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
        if (getResponse(99)==0) return 0;
        return checkResponse("ok\r\n");

    }

    uint8_t     Radio::digitalPinRead(uint8_t pin){


    }

    uint16_t    Radio::analogPinRead(uint8_t pin){


    }   

   bool        Radio::pinWrite(uint8_t pin, uint8_t state){

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
        if (getResponse(99)==0) return 0;
        return checkResponse("ok\r\n");

    }

    uint16_t    Radio::readVcc(){
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
            char getcha = sram->memread(inOffset+j);
            if ((getcha=='\r') || (getcha=='\n'))  {
              getcha = '\0';  
            }
            buffer[j] = getcha;
        }
        return atoi(buffer);
    }

    bool        Radio::setBt(uint8_t bt){
        // not available for LORA modulation

    }

    bool        Radio::setMode(uint8_t modulation){
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
        if (getResponse(99)==0) return 0;
        return checkResponse("ok\r\n");
    }

    bool        Radio::setFrequency(uint8_t channel){
        writeCommand(_radio); 
        writeCommand(_spcr);
        writeCommand(_set);
        writeCommand(_spcr);
        writeCommand(_frequency);
        writeCommand(_spcr);

        switch (channel) {
        
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
        default:
            writeCommand(_channel_1);
        }

        writeCommand(_megahertz);
        writeCommand(_commit);

        if (getResponse(99)==0) return 0;
        return checkResponse("ok\r\n");
    }

    bool        Radio::setPower(uint8_t power){

        if (power>17) power=17;
        if (power<2) power = 2;

        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i]='\0';
        }
        itoa(power, buffer, 10);

        writeCommand(_radio); 
        writeCommand(_spcr);
        writeCommand(_set);
        writeCommand(_spcr);
        writeCommand(_pwr);
        writeCommand(_spcr);
        writeExtCommand(buffer);
        writeCommand(_commit);

        if (getResponse(99)==0) return 0;
        return checkResponse("ok\r\n");

    }

    bool        Radio::setSpread(uint8_t factor){


    }
    
    bool        Radio::setAutoBand(uint8_t afb){
        // not available for lora modulation

    }
   
    bool        Radio::setBitRate(uint32_t bitrate){
        // not available for lora modulation

    }

    bool        Radio::setFdev(uint32_t deviation){
        // not available for lora modulation

    }

    bool        Radio::setPreamble(uint16_t preamble){
        // not available for lora modulation

    }

    bool        Radio::setCrc(uint8_t crcstate){


    }

    bool        Radio::setIqi(uint8_t iqistate){


    }

    bool        Radio::setCr(uint8_t crval){


    }

    bool        Radio::setWdt(uint32_t wdtimer){

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

        if (getResponse(99)==0) return 0;
        return checkResponse("ok\r\n");

    }

    bool        Radio::setSyncWord(uint8_t synw){


    }
    
    bool        Radio::setBandWidth(uint8_t bwidth) { //both tx and rx
    
    }

    void     Radio::writeCommand(const char *command){
    char buffer[20];
        strcpy_P(buffer, (char*)command);

        for (uint16_t i=0; i<strlen(buffer); i++) {
            port->print(buffer[i]);
        }

    }

    int     Radio::writeExtCommand(char *command){ 
            int ctr = 0;   
            for (uint16_t i=0; i<strlen(command); i++) {
                port->print(command[i]);
              ctr++;
            }
          return ctr;
        }


    uint16_t    Radio::sendBuffer(){
        sram->startSerialRead(outOffset);
        char grab=0;
	uint16_t i=0;
        uint16_t ctt = 0;
        grab = sram->byteout();

        while (grab!=0) {
            port->print(grab);
            grab = sram->byteout();
	i++;
	    delay(1);
	   ctt++;
           if (ctt>255) {
	   return 0;	
           }
        }
        
        sram->endSerialRead();
	return i/2;
        
    }


    void         Radio::pushChar(uint8_t character){
        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i]=0;
        }
        sprintf(buffer, "%02X", character);
        sram->memwrite(outOffset+bufferPointer, buffer[0]);
        bufferPointer++;
        sram->memwrite(outOffset+bufferPointer, buffer[1]);
        bufferPointer++;
    }

    uint32_t ebuffP=0;

    void         Radio::pushEChar(uint8_t character, uint8_t * buff){
        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i]=0;
        }
        sprintf(buffer, "%02X", character);
        buff[ebuffP]= buffer[0];
        ebuffP++;
        buff[ebuffP]= buffer[1];
        ebuffP++;
    }


    void         Radio::pushECChar(uint8_t * buff){
 
       
        buff[ebuffP]=0;
        ebuffP++;
        buff[ebuffP]= 0;
        ebuffP++;
    }


   void Radio::clearbuffP(){

   ebuffP=0;

   }



    void         Radio::pushEmptyChar(){
 
        sram->memwrite(outOffset+bufferPointer, 0);
        bufferPointer++;
        sram->memwrite(outOffset+bufferPointer, 0);
        bufferPointer++;
    }


    uint8_t         Radio::popChar(){

        char e=sram->memread(inOffset+inBufferPointer);
        if (e!=0) inBufferPointer++;
        return e;

    }


    void         Radio::bufferReset(){
        bufferPointer = 0;
        sram->startSerialWrite(outOffset);

        for (int i=0; i<1024; i++) {
            sram->bytein('a');
        }

        sram->endSerialWrite();
    }  


    void         Radio::inBufferReset(){
        inBufferPointer = 0;
        sram->startSerialWrite(inOffset);

        for (int i=0; i<1024; i++) {
            sram->bytein(0);
        }

        sram->endSerialWrite();
    }  


    int16_t   Radio::getResponse(uint32_t time){ //writes to outbuffer and returns size 
        uint16_t ccount=0;
        uint16_t counter=0;
        while(! port->available() ) {
        counter++;
        delay(1000);
        if(counter>time) return 0;
        }

        if(port->available()){ //hook it as soon as available
           sram->startSerialWrite(inOffset);
       
           char e;
            
            while(port->available()){  
                e=port->read();
                //sram->memwrite(inOffset+ccount, e);
                //port->write(e);
                sram->bytein(e);
                ccount++;
                delay(10);
             }
              
             //sram->bytein('\0');
             sram->endSerialWrite();
             //Serial.println(ccount);
             return ccount;
        }

 
    }
    
    bool     Radio::checkResponse(char * okval){
            uint8_t buffer[30];

             for (uint8_t idx=0; idx<20; idx++) {
               buffer[idx]=='\0';
            }
            int i=0;
             sram->startSerialRead(inOffset);
                for (i=0; i< strlen(okval); i++){
                    //buffer[i] = sram->memread(inOffset+i);
                    buffer[i] = sram->byteout();
                }
             sram->endSerialRead();
             buffer[i] = '\0';
             //Serial.println((char*)buffer);
            
             if(strcmp((const char *)buffer, (const char *)okval)!=0) {
                return false;
             } else {
                return true;
             }
    }


    void      Radio::loadSerialNumber(){
        if(EEPROM.read(0)!=0x38) return;
        for (int i=0; i<7; i++) {
            SerialNumber[i] = EEPROM.read(i+1);
        }
    }

    bool      Radio::wanPause(){
        writeCommand(_mac);
        writeCommand(_spcr);
        writeCommand(_pause);
        writeCommand(_commit); 
        if (getResponse(99)==0) return 0;
        return checkResponse("4294967295\r\n");
    }

    bool     Radio::wanResume(){
        writeCommand(_mac);
        writeCommand(_spcr);
        writeCommand(_resume);
        writeCommand(_commit);
        if (getResponse(99)==0) return 0;
        return checkResponse("ok\r\n");
    }
