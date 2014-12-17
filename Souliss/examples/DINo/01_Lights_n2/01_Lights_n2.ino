/**************************************************************************
	Souliss - Lights
	
	It handle the four relays either via IN1 to IN4 inputs or using the
	Android interface. Connecting the relays to lights or similar electrial 
	appliace, you can get remote control of them.
	
	Applicable for:
		- Light
		- Other ON/OFF electrical appliance
	
***************************************************************************/

#include "bconf/DINo_v2.h"					// Define the board type
#include "conf/DymanicAddressing.h"			// Use dynamic address

// Include framework code and libraries
#include <SPI.h>
#include "Souliss.h"

// By default the board will get an IP address with .77 as last byte, you can change it
// in runtime using the Android application SoulissApp

#define LIGHT1					0			// This is the memory slot used for the execution of the logic
#define LIGHT2					1			
#define LIGHT3					2			
#define LIGHT4					3			

void setup()
{	
	// Init the board
	InitDINo();
	
	// Set the inputs
	SetInput1();
    SetInput2();
    SetInput3();
    SetInput4();
	
	// Set the outputs
	SetRelay1();
	SetRelay2();
	SetRelay3();
	SetRelay4();
	
	// Set the status LED
	SetLED();
	
	// Define two Simple Light logics and the relays
	Set_SimpleLight(RELAY1);
	Set_SimpleLight(RELAY2);
	Set_SimpleLight(RELAY3);
	Set_SimpleLight(RELAY4);	
	
	// This board (peer) request an address to the gateway one at runtime, no need
	// to configure any parameter here
	SetDynamicAddressing();
	GetAddress();
	
}

void loop()
{ 
	// Here we start to play
	EXECUTEFAST() {						
		UPDATEFAST();	
		
		FAST_50ms() {	// We process the logic and relevant input and output every 50 milliseconds
		
			DigIn(IN1, Souliss_T1n_ToogleCmd, LIGHT1);			// Read inputs from IN1
			DigIn(IN2, Souliss_T1n_ToogleCmd, LIGHT2);			// Read inputs from IN2
			DigIn(IN3, Souliss_T1n_ToogleCmd, LIGHT3);			// Read inputs from IN3
			DigIn(IN4, Souliss_T1n_ToogleCmd, LIGHT4);			// Read inputs from IN4
		
			Logic_SimpleLight(LIGHT1);							// Execute the logic for Relay 1
			Logic_SimpleLight(LIGHT2);							// Execute the logic for Relay 2
			Logic_SimpleLight(LIGHT3);							// Execute the logic for Relay 3
			Logic_SimpleLight(LIGHT4);							// Execute the logic for Relay 4
			
			DigOut(RELAY1, Souliss_T1n_Coil, LIGHT1);			// Drive the Relay 1
			DigOut(RELAY2, Souliss_T1n_Coil, LIGHT2);			// Drive the Relay 2
			DigOut(RELAY3, Souliss_T1n_Coil, LIGHT3);			// Drive the Relay 3
			DigOut(RELAY4, Souliss_T1n_Coil, LIGHT4);			// Drive the Relay 4
		
		} 
			
		// Here we process all communication with other nodes
		FAST_PeerComms();
		
		// At first runs, we look for a gateway to join
		START_PeerJoin();
		
		// Periodically check if the peer node has joined the gateway
		FAST_1110ms() {
			if(JoinInProgress())	// If join is in progress, toggle the LED at every turn
				ToogleLED();
			else
				TurnOnLED();		// Once completed, turn ON
		}		
		
	}
	
	EXECUTESLOW() {	
		UPDATESLOW();

		SLOW_10s() {  // Process the timer every 10 seconds  
			Timer_SimpleLight(LIGHT1);
			Timer_SimpleLight(LIGHT2);	
			Timer_SimpleLight(LIGHT3);
			Timer_SimpleLight(LIGHT4);				
		} 	  
		
		// Here we periodically check for a gateway to join
		SLOW_PeerJoin();		
	}
} 