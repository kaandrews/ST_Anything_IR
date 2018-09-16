//******************************************************************************************
//  File: S_TimedRelayIR.cpp
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
//
//******************************************************************************************

#include "S_TimedRelayIR.h"

#include "Constants.h"
#include "Everything.h"

namespace st
{
//private
	void S_TimedRelayIR::writeStateToPin()
	{
		
		Serial.println("IR Send ");
		Serial.print("Code: ");
		Serial.println(m_IRCode);
	
		Serial.print("Bits: ");
		Serial.println(m_IRBits);
	
		Serial.print("Type: ");
		Serial.println(m_IRType);
	
		IRsend irsend(m_nPin);
		irsend.begin();
	
		switch(m_IRType)
		{
		case 1: irsend.sendNEC(m_IRCode,m_IRBits);break;			//NEC
		case 2: irsend.sendSony(m_IRCode,m_IRBits);break;			//SONY
		case 3: irsend.sendRC5(m_IRCode,m_IRBits);break;			//RC5
		case 4: irsend.sendRC6(m_IRCode,m_IRBits);break;			//RC6
		case 5: irsend.sendDISH(m_IRCode,m_IRBits);break;			//DISH
		case 6: irsend.sendJVC(m_IRCode,m_IRBits);break;			//JVC
		case 7: irsend.sendSAMSUNG(m_IRCode,m_IRBits);break;		//SAMSUNG
		case 8: irsend.sendLG(m_IRCode,m_IRBits);break;				//LG
		case 9: irsend.sendWhynter(m_IRCode,m_IRBits);break;		//WHYNTER
		case 10: irsend.sendCOOLIX(m_IRCode,m_IRBits);break;		//COOLIX
		case 11: irsend.sendDenon(m_IRCode,m_IRBits);break;			//DENON
		case 12: irsend.sendSherwood(m_IRCode,m_IRBits);break;		//SHERWOOD
		case 13: irsend.sendRCMM(m_IRCode,m_IRBits);break;			//RCMM
		case 14: irsend.sendMitsubishi(m_IRCode,m_IRBits);break;	//MITSUBISHI
		case 15: irsend.sendMitsubishi2(m_IRCode,m_IRBits);break;	//MITSUBISHI2
		case 16: irsend.sendSharpRaw(m_IRCode,m_IRBits);break;		//SHARP
		case 17: irsend.sendAiwaRCT501(m_IRCode,m_IRBits);break;	//AIWARCT501
		case 18: irsend.sendMidea(m_IRCode,m_IRBits);break;			//MIDEA
		case 19: irsend.sendGICable(m_IRCode,m_IRBits);break;		//GICABLE
		}
	}

//public
	//constructor
	S_TimedRelayIR::S_TimedRelayIR(const __FlashStringHelper *name, byte pinOutput, unsigned long IRCode, int IRBits, int IRType, unsigned long onTime ) :
		Sensor(name),
		m_bCurrentState(LOW),
		m_lOnTime(onTime),
		m_lOffTime(0),
		m_iNumCycles(1),
		m_iCurrentCount(1),
		m_lTimeChanged(0),
		m_bTimerPending(false),
		m_IRCode(IRCode),
    	m_IRBits(IRBits),
    	m_IRType(IRType)
		{
			
			setOutputPin(pinOutput);
			
		}
	
	//destructor
	S_TimedRelayIR::~S_TimedRelayIR()
	{
	}
	
	void S_TimedRelayIR::init()
	{
		Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH ? F("on") : F("off")));
	}

	//update function 
	void S_TimedRelayIR::update()
	{
		if (m_iCurrentCount < m_iNumCycles)
		{
			//Turn off digital output if timer has expired
			if ((m_bCurrentState == HIGH) && (millis() - m_lTimeChanged >= m_lOnTime))
			{	
				m_bCurrentState = LOW;
				//writeStateToPin();
				m_lTimeChanged = millis();
			}
			else if ((m_bCurrentState == LOW) && (millis() - m_lTimeChanged >= m_lOffTime))
			{	
				//add one to the current count since we finished an on/off cycle, and turn on output if needed
				m_iCurrentCount++;
				if (m_iCurrentCount < m_iNumCycles)
				{
					m_bCurrentState = HIGH;
					writeStateToPin();
					m_lTimeChanged = millis();
				}
				
			}
			
			//Check to see if we just finished the requested number of cycles
			if (m_iCurrentCount == m_iNumCycles)
			{
				//Decrement number of active timers
				if (st::Everything::bTimersPending > 0) st::Everything::bTimersPending--;
				m_bTimerPending = false;

				//Queue the relay status update the ST Cloud
				Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH ? F("on") : F("off")));
			}
		}
	}
	
	void S_TimedRelayIR::beSmart(const String &str)
	{
		String s = str.substring(str.indexOf(' ') + 1);
		if (st::Device::debug) {
			Serial.print(F("S_TimedRelay::beSmart s = "));
			Serial.println(s);
		}
		if ((s == F("on")) && (m_bCurrentState == LOW))
		{
			m_bCurrentState = HIGH;

			//Save time turned on
			m_lTimeChanged = millis();

			//Increment number of active timers
			if (!m_bTimerPending)
			{
				st::Everything::bTimersPending++;
				m_bTimerPending = true;
			}
			//Queue the relay status update the ST Cloud 
			Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH ? F("on") : F("off")));
			
			//Set the initial count to zero
			m_iCurrentCount = 0;

			//update the digital output
			writeStateToPin();
		}
		else if ((s == F("off")) && (m_bCurrentState == HIGH))
		{
			m_bCurrentState = LOW;

			//Decrement number of active timers
			if (st::Everything::bTimersPending > 0) st::Everything::bTimersPending--;
			m_bTimerPending = false;
			
			//Queue the relay status update the ST Cloud 
			Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH ? F("on") : F("off")));
			
			//Reset the count to the number of required cycles to prevent Update() routine from running if someone sends an OFF command
			m_iCurrentCount = m_iNumCycles;

			//update the digital output
			writeStateToPin();
		}
		
	}

	//called periodically by Everything class to ensure ST Cloud is kept consistent with the state of the contact sensor
	void S_TimedRelayIR::refresh()
	{
		//Queue the relay status update the ST Cloud
		Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH ? F("on") : F("off")));
	}

	void S_TimedRelayIR::setOutputPin(byte pin)
	{
		m_nPin = pin;
		pinMode(m_nPin, OUTPUT);
		digitalWrite(m_nPin,LOW);
		//writeStateToPin();
	}

}