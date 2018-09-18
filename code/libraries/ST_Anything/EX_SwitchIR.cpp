//******************************************************************************************
//  File: EX_SwitchIR.h
//  Authors: Kristian Andrews, based on work of Dan G Ogorchock & Daniel J Ogorchock (Father and Son)
//
//  Summary:  EX_SwitchIR is a class which implements the SmartThings "Switch" device capability.
//        It inherits from the st::Executor class.
//
//        Create an instance of this class in your sketch's global variable section
//        For Example:  st::EX_SwitchIR executor1("switch1", IR_PIN, 0xE0E040BF,32,1);
//
//        st::EX_SwitchIR() constructor requires the following arguments
//        - String &name - REQUIRED - the name of the object - must match the Groovy ST_Anything DeviceType tile name
//        - byte pin - REQUIRED - the Arduino Pin that is connected to the IR transmitter circuit, see below
//	  	  - unsigned long IRCode - REQUIRED - The IR code that you want to transmit
//		    - int bits - REQUIRED - the number of bits in the code
//		    - int type - REQUIRED - the type of transmitter e.g. LG, Samsung, etc
//
//  Change History:
//
//    Date        Who            What
//    ----        ---            ----
//    2015-01-03  Dan & Daniel   Original Creation
//    2018-08-30  Dan Ogorchock  Modified comment section above to comply with new Parent/Child Device Handler requirements
//    2018-08-30  K Andrews      Initial version of the IR code transmitting switch
//
//
//******************************************************************************************

//******************************************************************************************
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
//******************************************************************************************

#include "EX_SwitchIR.h"

#include "Constants.h"
#include "Everything.h"

namespace st
{
//private  
  void EX_SwitchIR::writeStateToPin()
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
  EX_SwitchIR::EX_SwitchIR(const __FlashStringHelper *name, byte pin, unsigned long IRCode, int IRBits, int IRType) :
    Executor(name),
    m_IRCode(IRCode),
    m_IRBits(IRBits),
    m_IRType(IRType)
  {
    setPin(pin);
  }

  //destructor
  EX_SwitchIR::~EX_SwitchIR()
  {
  
  }
  
  void EX_SwitchIR::init()
  {
    Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH ? F("on") : F("off")));
  }

  void EX_SwitchIR::beSmart(const String &str)
  {
    String s=str.substring(str.indexOf(' ')+1);
    if (st::Executor::debug) {
      Serial.print(F("EX_SwitchIR::beSmart s = "));
      Serial.println(s);
    }
    if(s==F("on"))
    {
      m_bCurrentState=HIGH;
    }
    else if(s==F("off"))
    {
      m_bCurrentState=LOW;
    }
    
    writeStateToPin();
    
    Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH?F("on"):F("off")));
  }
  
  void EX_SwitchIR::refresh()
  {
    Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH?F("on"):F("off")));
  }
  
  void EX_SwitchIR::setPin(byte pin)
  {
    m_nPin=pin;
    pinMode(m_nPin, OUTPUT);
    digitalWrite(m_nPin, LOW);
    //writeStateToPin();
  }
}
