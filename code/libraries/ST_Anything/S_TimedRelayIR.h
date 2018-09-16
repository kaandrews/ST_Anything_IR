//******************************************************************************************
//  File: S_TimedRelayIR.h
//  Authors: Kris Andrews based on the work of Dan G Ogorchock
//
//  Summary:  S_TimedRelayIR is a class which implements the SmartThings "Relay" device capability.  It features
//			  an automatic-turn-off time delay for a relay to emulate a button press.
//
//			  It inherits from the st::Sensor class and clones much from the st::Executor Class
//
//			  Create an instance of this class in your sketch's global variable section
//			  For Example:  st::S_TimedRelay sensor1("relaySwitch1", PIN_RELAY, 0x20df10ef, 32, 1, 500);
//
//			  st::S_TimedRelayIR() constructor requires the following arguments
//				- String &name - REQUIRED - the name of the object - must match the Groovy ST_Anything DeviceType tile name
//				- byte pinOutput - REQUIRED - the Arduino Pin to be used to send the IR code
//				- bool IRCode - REQUIRED - The IR code that you want to transmit
//				- bool IRBits - REQUIRED - the number of bits in the IR code
//				- bool IRType - REQUIRED - the number for the type of transmitter e.g. LG, Samsung, etc.  See list below.
//				- long onTime - REQUIRED - the number of milliseconds to keep the output on, DEFAULTS to 500 milliseconds
//
// Recommended to use pin D2 for the transmitter
//
// Note that this code creates a button that can turn on and off, both on and off transmit
// the same IR code.  A typical use would be to turn on and off a TV.
//
// To use this code you need to:
// 1) Install the library IRremoteESP8266 (available in the Arduino IDE Library Manager)
// 2) Put this file and the related header into your Arduino library folder
// 3) Get up and running with ST_Anything:
//    https://github.com/DanielOgorchock/ST_Anything
// 4) Figure out what codes you need to transmit
// 5) Build the transmitter circuit
// 
// Circuit To Build
// ----------------
//
// Required components:
//   IR LED
//   2N2222 transistor (or PN2222 etc)
//   1K resistor
//
// Others:
//   Jumper wires and breadboard
//
// Connections:
//   NodeMCU D2 to 1K resistor
//   1K resistor to 2N2222 Base
//   VCC(3.3v)+ to LED+
//   LED- to 2N2222 Collector
//   2N2222 Emitter to GND
//
// See this site for an example circuit:
//   https://learn.adafruit.com/using-an-infrared-library/sending-ir-codes
//
// To do anything useful with this library you need to know the following
//   The code to send, each button of remote will have a different code
//   The length of the code in bits, this tends to be standard but can vary
//   The protocol to use, it depends on the manufacturer of the remote you want to emulate
//
// To find these you need to use an IR receiver sketch or find the code online.
//
// I used an Arduino Uno and the IRremote example sketch that is included in the standard library
// IR receiver modules are easily available on Ebay or Aliexpress for very little money
//
// Using this library you can are emulate remotes that use the following protocols.
// The protocol does not always match the label on the remote, for example my LG TV
// comes out as using the NEC protocol. My Samsung TV does use Samsung protocol though.
//
// The number listed below is used in the constructor to specify the protocol to use.
//
// Protocol		Number
// NEC			1
// SONY 		2
// RC5			3
// RC6			4
// DISH 		5
// JVC			6
// SAMSUNG		7
// LG			8
// WHYNTER		9
// COOLIX		10
// DENON		11
// SHERWOOD		12
// RCMM			13
// MITSUBISHI	14
// MITSUBISHI2	15
// SHARP		16
// AIWARCT501	17
// MIDEA		18
// GICABLE		19
//
//  Change History:
//
//    Date        Who            What
//    ----        ---            ----
//    2015-12-29  Dan Ogorchock  Original Creation
//    2018-08-30  Dan Ogorchock  Modified comment section above to comply with new Parent/Child Device Handler requirements
//    2018-09-16  Kris Andrews   Modified to work as an IR code transmitter
//
//******************************************************************************************

#ifndef ST_S_TIMEDRELAYIR_H
#define ST_S_TIMEDRELAYIR_H

#include "Sensor.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"

namespace st
{
	class S_TimedRelayIR : public Sensor  //inherits from parent Sensor Class
	{
		private:
			bool m_bCurrentState;	//HIGH or LOW
			byte m_nPin;		//Arduino Pin used as a Digital Output for the switch - often connected to a relay or an LED
			unsigned long m_IRCode;		//The binary ID code of the transmitter
			int m_IRBits;	// Number of bits to send
			int m_IRType;	// Manufacturer code to use
			unsigned long m_lOnTime;		//number of milliseconds to keep digital output HIGH before automatically turning off
			unsigned long m_lOffTime;		//number of milliseconds to keep digital output LOW before automatically turning on
			unsigned int m_iNumCycles;		//number of on/off cycles of the digital output 
			unsigned int m_iCurrentCount;	//current number of on/off cycles of the digital output
			unsigned long m_lTimeChanged;	//time when the digital output was last changed
			bool m_bTimerPending;		//true if waiting on relay timer to expire
			

			void writeStateToPin();	//function to update the Arduino Digital Output Pin
			
		public:
			//constructor - called in your sketch's global variable declaration section
			S_TimedRelayIR(const __FlashStringHelper *name, byte pin, unsigned long IRCode, int IRBits, int IRType, unsigned long onTime = 500 );
			
			//destructor
			virtual ~S_TimedRelayIR();
			
			//initialization function
			virtual void init();

			//update function 
			void update();

			//SmartThings Shield data handler (receives command to turn "on" or "off" the switch (digital output)
			virtual void beSmart(const String &str);

			//called periodically by Everything class to ensure ST Cloud is kept consistent with the state of the contact sensor
			virtual void refresh();

			//gets
			virtual byte getPin() const { return m_nPin; }
			virtual bool getTimerActive() const { return m_bTimerPending; }

			//sets
			virtual void setOutputPin(byte pin);
	};
}


#endif