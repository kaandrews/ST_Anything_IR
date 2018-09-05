//******************************************************************************************
//  File: EX_SwitchIR.h
//  Authors: Kristian Andrews, based on work of Dan G Ogorchock & Daniel J Ogorchock (Father and Son)
//
//  Summary:  EX_SwitchIR is a class which implements the SmartThings "Switch" device capability.
//        It inherits from the st::Executor class.
//
//        Create an instance of this class in your sketch's global variable section
//        For Example:  st::EX_SwitchIR executor1("switch1", IR_PIN, 0xE0E040BF);
//
//        st::EX_SwitchIR() constructor requires the following arguments
//        - String &name - REQUIRED - the name of the object - must match the Groovy ST_Anything DeviceType tile name
//        - byte pin - REQUIRED - the Arduino Pin that is connected to the IR transmitter
//	  	  - int IRCode - REQUIRED - The Nexa transmitter ID code, uniquely identifies the transmitter
//
//  Change History:
//
//    Date        Who            What
//    ----        ---            ----
//    2015-01-03  Dan & Daniel   Original Creation
//    2018-08-30  Dan Ogorchock  Modified comment section above to comply with new Parent/Child Device Handler requirements
//    2018-08-30  K Andrews      Modified to work as a class for Nexa 433 MHz remotes
//
//
//******************************************************************************************
#ifndef ST_EX_SWITCHIR
#define ST_EX_SWITCHIR

#include "Executor.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"

namespace st
{
	class EX_SwitchIR: public Executor
	{
		private:
			bool m_bCurrentState;	//HIGH or LOW
			byte m_nPin;		//Arduino Pin used to transmit the IR signal
			unsigned long m_IRCode;		//The binary ID code of the transmitter
			int m_IRBits;	// Number of bits to send
			int m_IRType;	// Manufacturer code to use

			void writeStateToPin();	//function to update the Arduino Digital Output Pin
		
		public:
			//constructor - called in your sketch's global variable declaration section
			EX_SwitchIR(const __FlashStringHelper *name, byte pin, unsigned long IRCode, int IRBits, int IRType);
			
			//destructor
			virtual ~EX_SwitchIR();

			//initialization routine
			virtual void init();

			//SmartThings Shield data handler (receives command to turn "on" or "off" the switch (digital output)
			virtual void beSmart(const String &str);
			
			//called periodically to ensure state of the switch is up to date in the SmartThings Cloud (in case an event is missed)
			virtual void refresh();
			
			//gets
			virtual byte getPin() const {return m_nPin;}
			
			virtual bool getStatus() const { return m_bCurrentState; }	//whether the switch is HIGH or LOW

			//sets
			virtual void setPin(byte pin);
	};
}

#endif
