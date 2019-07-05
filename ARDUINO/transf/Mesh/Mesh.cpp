/*
* Mesh library, written for the ICROP monitor project, for AISER
* by Marcus Vinícius Martins Meneses
*
* dependencies:  Sram 23LCV1024 library, RN2903 LoRa library,  by the same author
*/


//strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i])));
#include <SPI.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Sram_mesh.h>
#include <Radio_mesh.h>
#include <Mesh.h>     

Mesh::Mesh(){

}

Mesh::Mesh(Radio * radiolink){
	radioLink = radiolink;
	restartRadio();
	currentChannel=1;						//mesh phys channel
	currentPower=START_RADIO_PWR;				        //mesh phys pwr
	currentHop=0;						        //mesh phys pwr
	loadSerialNumber();
}



//restarts radio interface with current settings
bool	Mesh::restartRadio(){
	radioLink->bufferReset();
	radioLink->hardReset();
	radioLink->wanPause(); 				//stop lorawan
	radioLink->setMode(1); 				//start lora radio modem
	radioLink->setFrequency(currentChannel);	//set channel
	radioLink->setPower(currentPower);		//set power

}

//mesh maintenance procedures, housekeeping mostly
bool	Mesh::meshMaintenance(){

}

//cycle mesh hops (on command, mostly)
bool	Mesh::meshHop(){


	restartRadio();
}

//wait for route offer
bool	Mesh::meshConnect(){

	uint8_t trycounter=0;
 	uint8_t failcounter=0;

	while (!waitRoute()){
		trycounter++;

		if (trycounter>8) {
			adjustRadioPower(currentPower++);
			restartRadio();
			requestRoute();
		}

		if (trycounter>15) {
			adjustRadioPower(START_RADIO_PWR);
			restartRadio();
			trycounter = 0;
			failcounter++;
				if (failcounter>3) return false;
			meshHop();
		}
	}

	trycounter=0;
 	failcounter=0;

	while (!acceptRoute()){
		trycounter++;
		
		if (trycounter>8) {
			adjustRadioPower(currentPower++);
			restartRadio();
		}

		if (trycounter>15) {
			 adjustRadioPower(START_RADIO_PWR);
			 restartRadio();
 			 return false;
			 
		}
	}

	return true;

}



//wait for route offer
bool	Mesh::waitRoute(){


	//wait for undirected route offer frame;
	if (receiveFrame(60000)==21) {
	
		//here is the postition for a random n-window size delay.
		//it may hinder you from acquiring a slot, but a random delay for everyone
		//helps everyone get a slot eventually;

		if (acceptRoute()) {
			return true;
		}
	
	}

   return false;
}

//send route offer
bool	Mesh::offerRoute(){

}

//accept route offer
bool	Mesh::acceptRoute(){

/*
   char from[7];
   char to[7];
   char maxFrames[1];
   char frameIndex[1];
   char frameUnique[3];
   char framePsize[3];
*/

FrameInfo * outframe = malloc(sizeof(FrameInfo));
FrameInfo * inframe = malloc(sizeof(FrameInfo));

loadInFrameInfo(inframe);

strncpy( SerialNumber , outframe->from, 7);
strncpy( inframe->from, outframe->to, 7);

strncpy( "1", outframe->maxFrames, 1);
strncpy( "1", outframe->frameIndex, 1);

strncpy( "000", outframe->framePsize, 1);

strncpy_P( _tpp_routeaccept, outframe->frameUnique, 3);

setOutFrameInfo(outframe);

//send prepared frame

	//wait for directed ACK frame
//	if (receiveFrame(60000)==11) {
	  //setup route data;
//	  return true;
//	}
 //   return false;

}


// send request route frame
bool	Mesh::requestRoute(){
// send request route frame

}


bool 	 Mesh::adjustRadioPower(uint8_t pwr){

}


//ask and wait for transmission window
bool	Mesh::waitForWindow(){

}

//send a priority packet through one of the priority channels. Priority channels ignore transmission
//windows and are put in line by any receiver. It will be forwarded by the first node to acheive window;
bool	Mesh::transmitPriorityPacket(uint8_t priorityChannel){

}

//listen for priority packet. This procedure must be called whenever hop transmissions are ended, and there will
//be some time to wait and relay priority
bool	Mesh::receivePriorityPacket(uint8_t priorityChannel){

}


// the following function does not transmit a packet, it releases packet for transmission, instead
// i.e: the packet will be sent ASAP
bool	Mesh::transmitPacket(){

}


//wait for a common-priority packet to arrive
bool	Mesh::receivePacket(uint32_t maxtime){
	


}


//process recently-arrived packet
bool	Mesh::processPacket(){

}





// the following function transmits a frame that has been built in memory
bool	Mesh::transmitFrame(){


//	reserved uniques:
//	Z** -> route offer						NOACK							31
//	R** -> active request route					NOACK							32
//	Y** -> ACK   (ACKNOLEDGED)					NOACK							11
//	X** -> NACK  (NOT ACKNOLEDGED)					NOACK							16
//	T** -> RST   (revoke or release route)				NOACK


//	W** -> route accept						ACK then call setclient					13
//	V** -> SYN   (Syncronization, payload has unix time)		ACK then call sync					14
//	U** -> URG   (urgent and only frame, please relay)		ACK then call relay					15
//	S** -> start offer route procedure				ACK then call relay procedure				17		
//	Q** -> request non-prioritary data upload			ACK then call connections accept procedure   		20
//	P** -> request upload window					ACK then call connections accept procedure		19

/*
#define FRAME_ROUTE_OFFER			1
#define FRAME_ACK				2
#define FRAME_NACK				3
#define FRAME_ROUTE_ACCEPT			4
#define FRAME_SYN				5
#define FRAME_URGENT				6
#define FRAME_RELEASE				7
#define FRAME_OFFER_ROUTES			8
#define FRAME_DATA				9
#define FRAME_DATA_UPLOAD			10
#define UPLOAD_WINDOW_REQ			11
*/
	uint8_t recvd = getOutFrameType();
	bool waitack = false;


switch (recvd) {
        case FRAME_ROUTE_ACCEPT:
	    waitack = true;
            break;
        case FRAME_SYN:
 	    waitack = true;
            break;
        case FRAME_URGENT:
	    waitack = true;
            break;
        case FRAME_OFFER_ROUTES:
	    waitack = true;
            break;
        case FRAME_DATA_UPLOAD:
	    waitack = true;
            break;
        case UPLOAD_WINDOW_REQ:
	    waitack = true;
            break;
        default:
	    waitack = false;
        }


uint8_t trycount=0;

	if (!waitack) {
		
		while ( !radiolink->sendPacket() ){
			trycount++;
			if (trycount>2) return false;
		} 
		return true;
	}

uint8_t trycount2=0;
trycount=0;

	if (waitack) {


		do {
		
			while ( !radiolink->sendPacket() ){
				trycount++;
				if (trycount>3) break;
			} 
			
			uint8_t recval = receiveFrame(10000 * (trycount2+1));
			if (recval == 12) return false;

			trycount2++;

			if (trycount2>2) return false;
			
		} while (recval!=11);
	  	
		return true;
	}





}


//receive a frame adressed to me or broadcast by another radio
uint8_t	Mesh::receiveFrame(uint32_t timeout){

	int count_timeout=0;
	uint8_t frameid[7];
	uint8_t ftype;

	uint8_t returnVal=0;

	radioLink->bufferReset();
	radioLink->setWdt(timeout+100); //wait tops

	while (!radiolink->receivePacket(timeout div 4)){
  		count_timeout++;

  		if (count_timeout > 3) {
    			//20 seconds dead
			return 0; //frame timeout
  		}
	}

	processFrame();
	getInFrameTo(frameid);
        ftype = getInFrameType();

		
		if ( strncmp (frameid, SerialNumber , 7) =='0') { //accept incoming frame to my address
 			returnVal = 10; //directly addressed
			if (ftype==FRAME_ACK)   { returnVal+= 1; }
			else if (ftype==FRAME_NACK) { returnVal+= 2; }
			else if (ftype==FRAME_ROUTE_ACCEPT) { returnVal+= 3; }
			else if (ftype==FRAME_SYN) { returnVal+= 4; }
			else if (ftype==FRAME_URGENT) { returnVal+= 5; }
			else if (ftype==FRAME_RELEASE) { returnVal+= 6; }
			else if (ftype==FRAME_OFFER_ROUTES) { returnVal+= 7; }
			else if (ftype==FRAME_DATA) { returnVal+= 8; }  
			else if (ftype==UPLOAD_WINDOW_REQ) { returnVal+= 9; } 
			else if (ftype==FRAME_DATA_UPLOAD) { returnVal+= 10; } 	
			else { returnVal=0; } // invalid frametype, frame corrupt?
			return returnVal;
		} else 	if ( strncmp (frameid, '0000000' , 7) =='0') { 	//accept incoming frame to zero address
 			returnVal = 30; //indirectly addressed

			if (ftype==FRAME_ROUTE_OFFER)   { returnVal+= 1; }
			else if (ftype==FRAME_ROUTE_REQUEST) { returnVal+= 2; }
			else { returnVal= 0; } // invalid frametype, frame corrupt?
			return returnVal;
		} else { //this denotes how busy the net is

		}



	return 30; // oddly addressed
}


//process received frame


bool	Mesh::processFrame(){
	parseInFrameInfo();

}


bool	Mesh::getInFrameFrom(uint8_t id[7]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadInFrameInfo(dataplace);

strncpy( id, dataplace->from, 1);

free(dataplace);
return true;
}


bool	Mesh::getInFrameTo(uint8_t id[7]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadInFrameInfo(dataplace);

strncpy( id, dataplace->to, 1);

free(dataplace);
return true;
}


bool	Mesh::getInFrameIndex(uint8_t id[1]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadInFrameInfo(dataplace);

strncpy( id, dataplace->frameIndex, 1);

free(dataplace);
return true;
}


bool	Mesh::getInFrameMax(uint8_t id[1]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadInFrameInfo(dataplace);

strncpy( id, dataplace->maxFrames, 1);

free(dataplace);
return true;
}

bool	Mesh::getInFramePsize(uint8_t id[3]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadInFrameInfo(dataplace);

strncpy( id, dataplace->framePsize, 3);

free(dataplace);
return true;
}
	

//gets type of inbound frame
uint8_t		Mesh::getInFrameType(){
FrameInfo * dataplace = malloc(sizeof(FrameInfo));
loadInFrameInfo(dataplace);

//free(dataplace);
//dataplace->frameUnique
//FRAME_ERROR

//	reserved uniques:
//	Z** -> route offer						FRAME_ROUTE_OFFER
//	Y** -> ACK   (ACKNOLEDGED)					FRAME_ACK		
//	X** -> NACK  (NOT ACKNOLEDGED)					FRAME_NACK
//	W** -> route accept						FRAME_ROUTE_ACCEPT
//	V** -> SYN   (Syncronization, payload has unix time)		FRAME_SYN
//	U** -> URG   (urgent and only frame, please relay)		FRAME_URGENT
//	T** -> RST   (revoke or release route)				FRAME_RELEASE
//	S** -> start offer route procedure				FRAME_OFFER_ROUTES
//   0..9** -> data							FRAME_DATA
//      *** -> malformed						FRAME_ERROR


switch (dataplace->frameUnique[0]) {
        case 'Z':
	    free(dataplace);
            return FRAME_ROUTE_OFFER;
            break;
        case 'Y':
 	    free(dataplace);
            return FRAME_ACK;
            break;
        case 'X':
	    free(dataplace);
            return FRAME_NACK;
            break;
        case 'W':
	    free(dataplace);
            return FRAME_ROUTE_ACCEPT;
            break;
        case 'V':
	    free(dataplace);
            return FRAME_SYN;
            break;
        case 'U':
	    free(dataplace);
            return FRAME_URGENT;
            break;
        case 'T':
	    free(dataplace);
            return FRAME_RELEASE;
            break;
        case 'S':
	    free(dataplace);
            return FRAME_OFFER_ROUTES;
            break;
        case 'R':
	    free(dataplace);
            return FRAME_ROUTE_REQUEST;
            break;
        case 'Q':
	    free(dataplace);
            return FRAME_DATA_UPLOAD;
            break;
 	case 'P':
	    free(dataplace);
            return UPLOAD_WINDOW_REQ;
            break;
        case '0':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '1':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '2':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '3':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '4':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '5':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '6':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '7':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '8':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '9':
	    free(dataplace);
            return FRAME_DATA;
            break;
        default:
	    free(dataplace);
            return FRAME_ERROR;
        }

free(dataplace);
return FRAME_ERROR;
}





bool	Mesh::getOutFrameFrom(uint8_t id[7]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadOutFrameInfo(dataplace);

strncpy( id, dataplace->from, 1);

free(dataplace);
return true;
}


bool	Mesh::getOutFrameTo(uint8_t id[7]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadOutFrameInfo(dataplace);

strncpy( id, dataplace->to, 1);

free(dataplace);
return true;
}


bool	Mesh::getOutFrameIndex(uint8_t id[1]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadOutFrameInfo(dataplace);

strncpy( id, dataplace->frameIndex, 1);

free(dataplace);
return true;
}


bool	Mesh::getOutFrameMax(uint8_t id[1]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadOutFrameInfo(dataplace);

strncpy( id, dataplace->maxFrames, 1);

free(dataplace);
return true;
}

bool	Mesh::getOutFramePsize(uint8_t id[3]){
FrameInfo * dataplace= malloc(sizeof(FrameInfo));
loadOutFrameInfo(dataplace);

strncpy( id, dataplace->framePsize, 3);

free(dataplace);
return true;
}
	

//gets type of inbound frame
uint8_t		Mesh::getOutFrameType(){
FrameInfo * dataplace = malloc(sizeof(FrameInfo));
loadOutFrameInfo(dataplace);

//free(dataplace);
//dataplace->frameUnique
//FRAME_ERROR

//	reserved uniques:
//	Z** -> route offer						FRAME_ROUTE_OFFER
//	Y** -> ACK   (ACKNOLEDGED)					FRAME_ACK		
//	X** -> NACK  (NOT ACKNOLEDGED)					FRAME_NACK
//	W** -> route accept						FRAME_ROUTE_ACCEPT
//	V** -> SYN   (Syncronization, payload has unix time)		FRAME_SYN
//	U** -> URG   (urgent and only frame, please relay)		FRAME_URGENT
//	T** -> RST   (revoke or release route)				FRAME_RELEASE
//	S** -> start offer route procedure				FRAME_OFFER_ROUTES
//   0..9** -> data							FRAME_DATA
//      *** -> malformed						FRAME_ERROR


switch (dataplace->frameUnique[0]) {
        case 'Z':
	    free(dataplace);
            return FRAME_ROUTE_OFFER;
            break;
        case 'Y':
 	    free(dataplace);
            return FRAME_ACK;
            break;
        case 'X':
	    free(dataplace);
            return FRAME_NACK;
            break;
        case 'W':
	    free(dataplace);
            return FRAME_ROUTE_ACCEPT;
            break;
        case 'V':
	    free(dataplace);
            return FRAME_SYN;
            break;
        case 'U':
	    free(dataplace);
            return FRAME_URGENT;
            break;
        case 'T':
	    free(dataplace);
            return FRAME_RELEASE;
            break;
        case 'S':
	    free(dataplace);
            return FRAME_OFFER_ROUTES;
            break;
        case 'R':
	    free(dataplace);
            return FRAME_ROUTE_REQUEST;
            break;
        case 'Q':
	    free(dataplace);
            return FRAME_DATA_UPLOAD;
            break;
 	case 'P':
	    free(dataplace);
            return UPLOAD_WINDOW_REQ;
            break;
        case '0':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '1':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '2':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '3':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '4':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '5':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '6':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '7':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '8':
	    free(dataplace);
            return FRAME_DATA;
            break;
        case '9':
	    free(dataplace);
            return FRAME_DATA;
            break;
        default:
	    free(dataplace);
            return FRAME_ERROR;
        }

free(dataplace);
return FRAME_ERROR;
}







//parse inbound packet frame info to frame info buffer in memory
bool	Mesh::parseInFrameInfo(){
//radioLink->sram->blockCopy(uint32_t startaddr, uint32_t endaddr, uint32_t size);
//radioLink->sram->blockErase(uint32_t startaddr, uint32_t size);


//decode inbound frame to decoded offset

//RADIO_INPUT_PAYLOAD_OFFSET

uint32_t ptr = 0;
uint32_t dptr = 0;
uint8_t encodedChar[2];
uint8_t decodedChar;

        if (radioLink->sram->memread(RADIO_INPUT_PAYLOAD_OFFSET)==0) return false;

	while (radioLink->sram->memread(RADIO_INPUT_PAYLOAD_OFFSET+ptr)!=0) {

		encodedChar[0] = radioLink->sram->memread(RADIO_INPUT_PAYLOAD_OFFSET+ptr);
		ptr++;
		encodedChar[1] = radioLink->sram->memread(RADIO_INPUT_PAYLOAD_OFFSET+ptr);
		ptr++;

		decodedchar = unencodeHex(encodedChar);

		memwrite(RADIO_INPUT_DECODED_OFFSET+dptr, uint8_t decodedchar);

		dptr++;
	}


radioLink->sram->blockCopy(RADIO_INPUT_DECODED_OFFSET+FRAME_FROM_ID, IN_FRAME_IDENTIFICATION_OFFSET+FRAME_FROM_ID, 7);
radioLink->sram->blockCopy(RADIO_INPUT_DECODED_OFFSET+FRAME_TO_ID  , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_TO_ID,   7);
radioLink->sram->blockCopy(RADIO_INPUT_DECODED_OFFSET+FRAME_MAX	   , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_MAX,     1);
radioLink->sram->blockCopy(RADIO_INPUT_DECODED_OFFSET+FRAME_INDEX  , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_INDEX,   1);
radioLink->sram->blockCopy(RADIO_INPUT_DECODED_OFFSET+FRAME_UNIQUE , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_UNIQUE,  3);
radioLink->sram->blockCopy(RADIO_INPUT_DECODED_OFFSET+FRAME_PSIZE  , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_PSIZE,   3);

return true;

} 

 
//set outbound packet frame info
bool	Mesh::loadOutFrameInfo(FrameInfo * dataplace){

if (radioLink->sram->memread(OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_FROM_ID)==0) return false;

radioLink->sram->blockLoad(OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_FROM_ID, dataplace->from 	, 7);
radioLink->sram->blockLoad(OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_TO_ID  , dataplace->to		, 7);
radioLink->sram->blockLoad(OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_MAX    , dataplace->maxFrames	, 1);
radioLink->sram->blockLoad(OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_INDEX  , dataplace->frameIndex	, 1);
radioLink->sram->blockLoad(OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_UNIQUE , dataplace->frameUnique, 3);
radioLink->sram->blockLoad(OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_PSIZE  , dataplace->framePsize	, 3);

return true;

}


//load inbound frame info to structure
bool	Mesh::loadInFrameInfo(FrameInfo * dataplace){

if (radioLink->sram->memread(IN_FRAME_IDENTIFICATION_OFFSET+FRAME_FROM_ID)==0) return false;

radioLink->sram->blockLoad(IN_FRAME_IDENTIFICATION_OFFSET+FRAME_FROM_ID, dataplace->from 	, 7);
radioLink->sram->blockLoad(IN_FRAME_IDENTIFICATION_OFFSET+FRAME_TO_ID  , dataplace->to		, 7);
radioLink->sram->blockLoad(IN_FRAME_IDENTIFICATION_OFFSET+FRAME_MAX    , dataplace->maxFrames	, 1);
radioLink->sram->blockLoad(IN_FRAME_IDENTIFICATION_OFFSET+FRAME_INDEX  , dataplace->frameIndex	, 1);
radioLink->sram->blockLoad(IN_FRAME_IDENTIFICATION_OFFSET+FRAME_UNIQUE , dataplace->frameUnique	, 3);
radioLink->sram->blockLoad(IN_FRAME_IDENTIFICATION_OFFSET+FRAME_PSIZE  , dataplace->framePsize	, 3);

return true;
}


//move structured to output frame data memory buffer
bool	Mesh::setOutFrameInfo(FrameInfo * dataplace){


radioLink->sram->loadBlock(dataplace->from 	 , OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_FROM_ID, 7);
radioLink->sram->loadBlock(dataplace->to	 , OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_TO_ID  , 7);
radioLink->sram->loadBlock(dataplace->maxFrames	 , OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_MAX    , 1);
radioLink->sram->loadBlock(dataplace->frameIndex , OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_INDEX  , 1);
radioLink->sram->loadBlock(dataplace->frameUnique, OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_UNIQUE , 3);
radioLink->sram->loadBlock(dataplace->framePsize , OUT_FRAME_IDENTIFICATION_OFFSET+FRAME_PSIZE  , 3);

return true;
}



bool	    buildOutFrame(uint8_t Psize, uint16_t Poffset){
FrameInfo * dataplace = malloc(sizeof(FrameInfo));
loadOutFrameInfo(dataplace);






free(dataplace);
}



//move structure to input frame data memory buffer
bool	Mesh::setInFrameInfo(FrameInfo * dataplace){

radioLink->sram->loadBlock(dataplace->from 	 , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_FROM_ID, 7);
radioLink->sram->loadBlock(dataplace->to	 , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_TO_ID  , 7);
radioLink->sram->loadBlock(dataplace->maxFrames	 , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_MAX    , 1);
radioLink->sram->loadBlock(dataplace->frameIndex , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_INDEX  , 1);
radioLink->sram->loadBlock(dataplace->frameUnique, IN_FRAME_IDENTIFICATION_OFFSET+FRAME_UNIQUE , 3);
radioLink->sram->loadBlock(dataplace->framePsize , IN_FRAME_IDENTIFICATION_OFFSET+FRAME_PSIZE  , 3);

return true;
}


//load client from buffer to memory struct
bool	Mesh::loadClientInfo(ClientInfo * dataplace, uint8_t clientIndex){

/*
   char slotAvailable[1];
   char slotId[7];
   char slotIdleCount[1];

#define CLIENT_SLOT_AVAILABLE			0
#define CLIENT_ID_SIZE				1
#define CLIENT_IDLE_COUNT			8

*/

uint32_t offset = clientIndex * 10;

radioLink->sram->blockLoad(CLIENTS_STRUC_OFFSET+CLIENT_SLOT_AVAILABLE+offset	, dataplace->slotAvailable	, 1);
radioLink->sram->blockLoad(CLIENTS_STRUC_OFFSET+CLIENT_ID_SIZE+offset  		, dataplace->slotId		, 7);
radioLink->sram->blockLoad(CLIENTS_STRUC_OFFSET+CLIENT_IDLE_COUNT+offset    	, dataplace->slotIdleCount	, 1);



}


//set client buffer unit from memory struct
bool	Mesh::setClientInfo(ClientInfo * dataplace, uint8_t clientIndex){

uint32_t offset = clientIndex * 10;

radioLink->sram->loadBlock(dataplace->slotAvailable 	, CLIENTS_STRUC_OFFSET+CLIENT_SLOT_AVAILABLE+offset	, 1);
radioLink->sram->loadBlock(dataplace->slotId	 	, CLIENTS_STRUC_OFFSET+CLIENT_ID_SIZE+offset		, 7);
radioLink->sram->loadBlock(dataplace->slotIdleCount 	, CLIENTS_STRUC_OFFSET+CLIENT_IDLE_COUNT+offset 	, 1);


}


//find an open client entry

int8_t	Mesh::findDeadClientEntry(){

ClientInfo * dataSet = malloc(sizeof(ClientInfo));

	for (uint8_t i =0; i<MAX_CLIENTS; i++) {

		loadClientInfo(dataSet, i);

		if (dataSet->slotAvailable=='0') { 
			free(dataSet);
			return i;

		}

	}  

free(dataSet);
return -1;

}


//find a client entry by its id
int8_t	Mesh::findClientByID( char * Id ){

ClientInfo * dataSet = malloc(sizeof(ClientInfo));

	for (uint8_t i =0; i<MAX_CLIENTS; i++) {

		loadClientInfo(dataSet, i);

		if ( strncmp (dataSet->slotId, (const char *) Id, 7) =='0') {
                        free(dataSet);
			return i;
		}

	}  

free(dataSet);
return -1;

}

//strike client entry as idle (if it does not answer for some time)
bool	Mesh::strikeIdle(uint8_t clientIndex){

	ClientInfo * dataSet = malloc(sizeof(ClientInfo));

	loadClientInfo(dataSet, clientIndex);

	dataSet->slotIdleCount--;

	if (dataSet->slotIdleCount<=0) dataSet->slotAvailable=='0';

	setClientInfo(dataSet,  clientIndex);

	free(dataSet);

return true;

}


//load route info from memory buffer to memory structure
bool	Mesh::loadRouteInfo(RouteInfo * dataplace, uint8_t routeIndex){

/*
define ROUTED_ORI_DST				0
define ROUTED_FROM_TO				7
define ROUTED_TTL				14	

   char originTurnedDestination[7];
   char fromTurnedTo[7];
   char routedTTL[2];	
*/

uint32_t offset = routeIndex * 16;

radioLink->sram->blockLoad(ROUTED_STRUC_OFFSET+ROUTED_ORI_DST+offset  , dataplace->originTurnedDestination 	, 7);
radioLink->sram->blockLoad(ROUTED_STRUC_OFFSET+ROUTED_FROM_TO+offset  , dataplace->fromTurnedTo			, 7);
radioLink->sram->blockLoad(ROUTED_STRUC_OFFSET+ROUTED_TTL+offset      , dataplace->routedTTL			, 2);
 
}


// save memory structure to route memory buffer
bool	Mesh::setRouteInfo(RouteInfo * dataplace, uint8_t routeIndex){

uint32_t offset = routeIndex * 16;

radioLink->sram->loadBlock(dataplace->originTurnedDestination 	 , ROUTED_STRUC_OFFSET+ROUTED_ORI_DST+offset, 7);
radioLink->sram->loadBlock(dataplace->fromTurnedTo		 , ROUTED_STRUC_OFFSET+ROUTED_FROM_TO+offset, 7);
radioLink->sram->loadBlock(dataplace->routedTTL			 , ROUTED_STRUC_OFFSET+ROUTED_TTL+offset    , 2);

}

//find a route entry that has expired
//every time a client routes a packet, it strikes every other route and
//regenerates the current route to full TTL 
int8_t	Mesh::findDeadRouteEntry(){

RouteInfo * dataSet = malloc(sizeof(RouteInfo));

	for (uint8_t i =0; i<ROUTES_MAX; i++) {

		loadRouteInfo(dataSet, i);

		if (dataSet->routedTTL=='00') { 
			free(dataSet);
			return i;

		}

	}  

free(dataSet);
return -1;


}

//strikes TTL of indexed route structure

bool	Mesh::strikeTTL(uint8_t routeIndex){

/*
define ROUTED_ORI_DST				0
define ROUTED_FROM_TO				7
define ROUTED_TTL				14	

   char originTurnedDestination[7];
   char fromTurnedTo[7];
   char routedTTL[2];	
*/
char tbuff[3];
uint8_t tcount;

//int atoi (const char * str);
//itoa ( int value, char * str, int base );

RouteInfo * dataSet = malloc(sizeof(RouteInfo));

loadRouteInfo(dataSet, routeIndex);

tbuff[0]=routeIndex->routedTTL[0];
tbuff[1]=routeIndex->routedTTL[1];
tbuff[2]='\0';

tcount = atoi(tbuff);
tcount--;
if (tcount<0) tcount=0;

itoa(tcount, tbuff, 10);

routeIndex->routedTTL[0]=tbuff[0];
routeIndex->routedTTL[1]=tbuff[1];

setRouteInfo(dataSet, routeIndex);

}


uint8_t   Mesh::unencodeHex(uint8_t encodedChar[2]){
        	uint8_t  decodeChar[4];
        	decodeChar[0] = (uint8_t)'0';
        	decodeChar[1] = (uint8_t)'x';
        	decodeChar[2] = encodedChar[0];
        	decodeChar[3] = encodedChar[1];
        	return (uint8_t)strtol((const char *)decodeChar, NULL, 16);
}


bool	Mesh::loadSerialNumber(){
        //if(EEPROM.read(0)!=0x38) return false;
        for (int i=0; i<7; i++) {
            SerialNumber[i] = EEPROM.read(radio_address[i]);
        }
	return true;
}


