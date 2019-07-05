#ifndef Mesh_h
#define Mesh_h

#include <SPI.h>
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Sram.h>
#include <Radio.h>


//packet type constants
const char _tpp_routeoffer   PROGMEM = "ZZZ";
const char _tpp_frameack     PROGMEM = "YYY";
const char _tpp_framenack    PROGMEM = "XXX";
const char _tpp_routeaccept  PROGMEM = "WWW";
const char _tpp_syn	     PROGMEM = "VVV";
const char _tpp_urgent	     PROGMEM = "UUU";
const char _tpp_revk         PROGMEM = "TTT";
const char _tpp_arr	     PROGMEM = "RRR";
const char _tpp_nonpri	     PROGMEM = "QQQ";
 


//shell commands
const char _comm_send_test	PROGMEM = "send";		//send ABCDEFGHIJKLMNOP
const char _comm_set_from	PROGMEM = "set-from";		//set-from 1111111
const char _comm_set_to 	PROGMEM = "set-to";		//set-to 1111111
const char _comm_set_origin	PROGMEM = "set-origin";		//set-origin 1111111
const char _comm_set_destin	PROGMEM = "set-destination";	//set-destination 1111111
const char _comm_set_funique	PROGMEM = "set-frameunique";	//set-frameunique S00
const char _comm_set_maxframes	PROGMEM = "set-maxframes";	//set-maxframes 3
const char _comm_set_frameno	PROGMEM = "set-framenumber";    //set-framenumber 3
								//obs: payload size is not set, as paload is a parameter to _comm_send_test
const char _comm_set_serial	PROGMEM = "set-serial";		//set-serial AAAAAAA
const char _comm_set_nettime	PROGMEM = "set-nettime";	//set-nettime 9879874
const char _comm_con_release	PROGMEM = "con-release";	//con-release
const char _comm_con_connect	PROGMEM = "con-connect";	//con-connect
const char _comm_mod_reset	PROGMEM = "module-reset";	//module-reset
const char _comm_mod_reset	PROGMEM = "module-reboot";	//module-reboot




//R000001000000011018000R0000010000000A


/*packet formation

//	reserved uniques:
//	Z** -> route offer						FRAME_ROUTE_OFFER		indirectly addressed
//	Y** -> ACK   (ACKNOLEDGED)					FRAME_ACK		        directly addressed
//	X** -> NACK  (NOT ACKNOLEDGED)					FRAME_NACK			directly addressed
//	W** -> route accept						FRAME_ROUTE_ACCEPT		directly addressed
//	V** -> SYN   (Syncronization, payload has unix time)		FRAME_SYN			directly addressed
//	U** -> URG   (urgent and only frame, please relay)		FRAME_URGENT			directly addressed
//	T** -> RST   (revoke or release route)				FRAME_RELEASE			directly addressed
//	S** -> start offer route procedure				FRAME_OFFER_ROUTES		direclty addressed
//	R** -> active request route					FRAME_ROUTE_REQUEST		indireclty addressed
//	Q** -> request non=prioritary data upload			FRAME_DATA_UPLOAD		direclty addressed
//	P** -> request upload window					UPLOAD_WINDOW_REQ		direclty addressed


//   0..9** -> data							FRAME_DATA			directly addressed
//      *** -> malformed						FRAME_ERROR			nothing to do, degrade in false chain



frame

frame header  :22
frame total   :128
frame payload :106 

FROM(7);  TO(7);  FRAME_MAX(1); FRAME_ID(1);UNIQUE(3);PAYLOAD_SIZE(3);	PAYLOAD(PSIZE) (psize <= 106)
0000000   0000000 0 		0 	    000       000             	000...000


packet total : 1024

packet
PACKET_ID(3);ORIGIN(7);DESTINATION(7);PTYPE(1);PPAYLOAD(PSIZE) 
000 	     0000000   0000000 	      0        000.........000

*/ 

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
#define FRAME_ERROR				0
 

#define RADIO_OUTPUT_OFFSET_LOW   		512		//512 bytes for inbound frame (total)	               9876543210
#define RADIO_INPUT_PAYLOAD_OFFSET   		1034		//This is where the encoded data actually is, 10 chars offset "radio_rx  "

#define RADIO_INPUT_DECODED_OFFSET   		6144		//This is where the 

#define OUT_FRAME_IDENTIFICATION_OFFSET   	128		//outbound frameb
#define IN_FRAME_IDENTIFICATION_OFFSET   	256		//inbound
#define FRAME_FROM_ID   			0		//512 bytes for inbound frame (total) [0000000 0000000 0 0 000 000]
#define FRAME_TO_ID   				7		//512 bytes for inbound frame (total) 
#define FRAME_MAX  				14		//1 byte (from 0 to 9)
#define FRAME_INDEX  				15		//1 byte (from 0 to 9)
#define FRAME_UNIQUE  				16		//3 bytes (from 000 to ZZZ)
#define FRAME_PSIZE  				19		//3 bytes (from 000 to 999)

#define IN_PACKET	  			2048		//1 kb for complete unencoded packet
#define OUT_PACKET 				3072		//1 kb for complete unencoded packet

#define MAX_CLIENTS				20
#define CLIENTS_STRUC_OFFSET			4096		//connected clients structure		[0 0000000 0 _]
#define CLIENT_SLOT_AVAILABLE			0
#define CLIENT_ID_SIZE				1
#define CLIENT_IDLE_COUNT			8

#define ROUTES_MAX				63		//keep 63 routes
#define ROUTED_STRUC_OFFSET			5120		//routed packets structure		[0000000 0000000 00]
#define ROUTED_ORI_DST				0
#define ROUTED_FROM_TO				7
#define ROUTED_TTL				14		//MAX 99 route events without refresh to release slot	

#define START_RADIO_PWR				8


typedef struct Tframe_identification {  //22 byte registry
   char from[7];
   char to[7];
   char maxFrames[1];
   char frameIndex[1];
   char frameUnique[3];
   char framePsize[3];
} FrameInfo;


typedef struct Tclients_structure {    //10 byte registry
   char slotAvailable[1];
   char slotId[7];
   char slotIdleCount[1];
   char reserved[1]; 
} ClientInfo;

typedef struct Trouted_packets {       //16 byte registry
   char originTurnedDestination[7];
   char fromTurnedTo[7];
   char routedTTL[2];
} RouteInfo;



//biomarcadores têm sido de grande valia para o diagnóstico precoce, pois permitem identificar com antecedência problemas incipientes no organismo.

class Mesh {
    
   public:
		    Mesh();
		    Mesh(Radio * radiolink);
	bool        restartRadio();
	bool	    meshMaintenance();
	bool	    meshHop();
	bool	    meshConnect();

	bool	    waitRoute();
        bool	    offerRoute();
	bool	    requestRoute();  //avoid using this, please
	bool 	    acceptRoute();

	bool 	    waitForWindow();

	bool	    transmitPriorityPacket(uint8_t priorityChannel);
        bool	    receivePriorityPacket(uint8_t priorityChannel);

	bool	    transmitPacket();
	bool	    receivePacket(uint32_t maxtime);
	bool	    processPacket();

	bool	    buildOutFrame();

	bool	    transmitFrame();
	uint8_t	    receiveFrame(uint32_t maxtime);
	bool	    processFrame();

	bool 	    adjustRadioPower(uint8_t pwr);

	uint8_t    SerialNumber[7];
	uint8_t    EEMEM       radio_address[7] = {'M', '1', '2', '3', '4', '5', '6'};
	uint8_t    EEMEM     operation_start[8] = {'0', '5', '/', '1', '0', '/', '1', '8'};
	uint8_t    EEMEM       firmware_date[8] = {'0', '5', '/', '1', '0', '/', '1', '8'};
	uint8_t    EEMEM         customer_id[7] = {'0', '0', '0', '0', '0', '0', '1'};
	uint8_t    EEMEM  hidden_declaration[4] = {'A', 'V', 'F', 'M'};
	uint8_t    EEMEM         shell_login[9] = {'A', 'I', 'S', 'E', 'R', '1', '0', '1', '0'};
	uint8_t    EEMEM      shell_password[6] = {'l', 'd', 'k', '3', '6', 'n'};

            
   private:
            
	Radio	 	*radioLink;						//our radio object
	uint8_t   	currentChannel;						//mesh phys channel
	uint8_t   	currentPower;						//mesh phys pwr
	uint8_t		currentHop;						//our current mesh hop;

 	uint8_t     	entrySerial[7];						//our entry point to the mesh network
	uint16_t        currentPacketID;
	uint8_t		currentFramePointer;



	bool      	loadSerialNumber();

	bool		parseInFrameInfo(); 					//gets input frame data to its proper place in mem
	uint8_t		getInFrameType();
	bool		getInFrameFrom(uint8_t id[7]);
	bool		getInFrameTo(uint8_t id[7]);
	bool		getInFrameIndex(uint8_t id[1]);
	bool		getInFrameMax(uint8_t id[1]);
	bool		getInFramePsize(uint8_t id[3]);	

	uint8_t		getOutFrameType();
	bool		getOutFrameFrom(uint8_t id[7]);
	bool		getOutFrameTo(uint8_t id[7]);
	bool		getOutFrameIndex(uint8_t id[1]);
	bool		getOutFrameMax(uint8_t id[1]);
	bool		getOutFramePsize(uint8_t id[3]);	

	bool 		loadOutFrameInfo(FrameInfo * dataplace);
	bool 		loadInFrameInfo(FrameInfo * dataplace);

	bool 		setOutFrameInfo(FrameInfo * dataplace);
	bool 		setInFrameInfo(FrameInfo * dataplace);

	bool 		loadClientInfo(ClientInfo * dataplace, uint8_t clientIndex);
	bool 		setClientInfo(ClientInfo * dataplace, uint8_t clientIndex);
	int8_t 		findDeadClientEntry();
	int8_t		findClientByID( char * Id );
	bool 		strikeIdle(uint8_t clientIndex); //strikes and kills accordingly

	bool 		loadRouteInfo(RouteInfo * dataplace, uint8_t routeIndex);
	bool 		setRouteInfo(RouteInfo * dataplace, uint8_t routeIndex); 
	int8_t 		findDeadRouteEntry();
	bool 		strikeTTL(uint8_t routeIndex); //strikes and kills accordingly


	uint8_t   	unencodeHex(uint8_t encodedChar[2]);
	
};


#endif










