//******************************************************************************************
//  File: ST_Anything_IR_ESP8266WiFi.ino
//  Authors: K Andrews based on the work of Dan G Ogorchock & Daniel J Ogorchock (Father and Son)
//
//  Summary:  This Arduino Sketch, along with the ST_Anything library and the revised SmartThings
//            library, demonstrates the ability of one NodeMCU ESP8266 to
//            control infrared remote devices via SmartThings.
//            The ST_Anything library takes care of all of the work to schedule device updates
//            as well as all communications with the NodeMCU ESP8266's WiFi.
//
//            ST_Anything_Multiples implements the following ST Capabilities as a demo of what is possible with a single NodeMCU ESP8266
//              - 6 x IR Relay devices (used to send an IR code to a device, sends the code each time the button is pressed)
//              - 3 x IR Switch devices (used to turn on and off an IR controlled device (e.g. TV, DVD, etc...) remembers the on/off state)
//
//  Change History:
//
//    Date        Who            What
//    ----        ---            ----
//    2015-01-03  Dan & Daniel   Original Creation
//    2017-02-12  Dan Ogorchock  Revised to use the new SMartThings v2.0 library
//    2017-04-17  Dan Ogorchock  New example showing use of Multiple device of same ST Capability
//                               used with new Parent/Child Device Handlers (i.e. Composite DH)
//    2017-05-25  Dan Ogorchock  Revised example sketch, taking into account limitations of NodeMCU GPIO pins
//    2018-02-09  Dan Ogorchock  Added support for Hubitat Elevation Hub
//    2018-09-04  K Andrews      Modified to support IR controlled devices
//    2018-09-16  K Andrews      Added IR Timed Relay device examples
//
//******************************************************************************************
//******************************************************************************************
// SmartThings Library for ESP8266WiFi
//******************************************************************************************
#include <SmartThingsESP8266WiFi.h>

//******************************************************************************************
// ST_Anything Library
//******************************************************************************************
#include <Constants.h>       //Constants.h is designed to be modified by the end user to adjust behavior of the ST_Anything library
#include <Device.h>          //Generic Device Class, inherited by Sensor and Executor classes
#include <Executor.h>        //Generic Executor Class, typically receives data from ST Cloud (e.g. Switch)
#include <Everything.h>      //Master Brain of ST_Anything library that ties everything together and performs ST Shield communications
#include <EX_SwitchIR.h>     //Implements an Executer (EX) IR transmitter to toggle a device On/Off
#include <S_TimedRelayIR.h>  // IR Timed relay

//*************************************************************************************************
//NodeMCU v1.0 ESP8266-12e Pin Definitions (makes it much easier as these match the board markings)
//*************************************************************************************************
//#define LED_BUILTIN 16
//#define BUILTIN_LED 16
//
//#define D0 16  //no internal pullup resistor
//#define D1  5
//#define D2  4
//#define D3  0  //must not be pulled low during power on/reset, toggles value during boot
//#define D4  2  //must not be pulled low during power on/reset, toggles value during boot
//#define D5 14
//#define D6 12
//#define D7 13
//#define D8 15  //must not be pulled high during power on/reset

//******************************************************************************************
//Define which Arduino Pins will be used for each device
//******************************************************************************************
#define PIN_IR_1             D2  //Pin used to control the IR led

//******************************************************************************************
//ESP8266 WiFi Information
//******************************************************************************************
String str_ssid     = "wifissid";                           //  <---You must edit this line!
String str_password = "wifipassword";                   //  <---You must edit this line!
IPAddress ip(192, 168, x, y);       //Device IP Address       //  <---You must edit this line!
IPAddress gateway(192, 168, x, y);    //Router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, x, y);  //DNS server              //  <---You must edit this line!
const unsigned int serverPort = 8090; // port to run the http server on

// Smartthings / Hubitat Hub TCP/IP Address
IPAddress hubIp(192, 168, x, y);    // smartthings/hubitat hub ip //  <---You must edit this line!

// SmartThings / Hubitat Hub TCP/IP Address: UNCOMMENT line that corresponds to your hub, COMMENT the other
const unsigned int hubPort = 39500;   // smartthings hub port
//const unsigned int hubPort = 39501;   // hubitat hub port

//******************************************************************************************
//st::Everything::callOnMsgSend() optional callback routine.  This is a sniffer to monitor
//    data being sent to ST.  This allows a user to act on data changes locally within the
//    Arduino sktech.
//******************************************************************************************
void callback(const String &msg)
{
  //  Serial.print(F("ST_Anything Callback: Sniffed data = "));
  //  Serial.println(msg);

  //TODO:  Add local logic here to take action when a device's value/state is changed

  //Masquerade as the ThingShield to send data to the Arduino, as if from the ST Cloud (uncomment and edit following line)
  //st::receiveSmartString("Put your command here!");  //use same strings that the Device Handler would send
}

//******************************************************************************************
//Arduino Setup() routine
//******************************************************************************************
void setup()
{
  //******************************************************************************************
  //Declare each Device that is attached to the Arduino
  //  Notes: - For each device, there is typically a corresponding "tile" defined in your
  //           SmartThings Device Hanlder Groovy code, except when using new COMPOSITE Device Handler
  //         - For details on each device's constructor arguments below, please refer to the
  //           corresponding header (.h) and program (.cpp) files.
  //         - The name assigned to each device (1st argument below) must match the Groovy
  //           Device Handler names.  (Note: "temphumid" below is the exception to this rule
  //           as the DHT sensors produce both "temperature" and "humidity".  Data from that
  //           particular sensor is sent to the ST Hub in two separate updates, one for
  //           "temperature" and one for "humidity")
  //         - The new Composite Device Handler is comprised of a Parent DH and various Child
  //           DH's.  The names used below MUST not be changed for the Automatic Creation of
  //           child devices to work properly.  Simply increment the number by +1 for each duplicate
  //           device (e.g. contact1, contact2, contact3, etc...)  You can rename the Child Devices
  //           to match your specific use case in the ST Phone Application.
  //******************************************************************************************

  //Special sensors/executors (uses portions of both polling and executor classes)
  // LG TV Controls, examples from my TV
  // The buttons stay on for 1/2 second by default, send the code each time the button in pressed
  // This is ideal for buttons such as volume control or program up/down as my example
  static st::S_TimedRelayIR sensor1(F("relaySwitch1"), PIN_IR_1, 0x20df10ef, 32, 1); // LG Power
  static st::S_TimedRelayIR sensor2(F("relaySwitch2"), PIN_IR_1, 0x20df40bf, 32, 1); // LG Vol Up
  static st::S_TimedRelayIR sensor3(F("relaySwitch3"), PIN_IR_1, 0x20dfc03f, 32, 1); // LG Vol Down
  static st::S_TimedRelayIR sensor4(F("relaySwitch4"), PIN_IR_1, 0x20df00ff, 32, 1); // LG Channel Up
  static st::S_TimedRelayIR sensor5(F("relaySwitch5"), PIN_IR_1, 0x20df807f, 32, 1); // LG Channel Down
  static st::S_TimedRelayIR sensor6(F("relaySwitch6"), PIN_IR_1, 0x20df55aa, 32, 1); // LG Info

  //Executors
  // LG TV Controls
  // These buttons remember the On/Off state, click once to turn on, click again to turn off
  // The same code is sent each time.  Ideal for buttons such as TV Power On/Off
  static st::EX_SwitchIR executor1(F("switch1"), PIN_IR_1, 0xE0E040BF, 32, 7); // Send Samsung TV Power button IR code
  static st::EX_SwitchIR executor2(F("switch2"), PIN_IR_1, 0xE0E0D02F, 32, 7); // Send Samsung TV Volume Down IR code
  static st::EX_SwitchIR executor3(F("switch3"), PIN_IR_1, 0x20df10ef, 32, 1); // Send LG TV Power button IR code (uses NEC protocol)

  //*****************************************************************************
  //  Configure debug print output from each main class
  //  -Note: Set these to "false" if using Hardware Serial on pins 0 & 1
  //         to prevent communication conflicts with the ST Shield communications
  //*****************************************************************************
  st::Everything::debug = true;
  st::Executor::debug = true;
  st::Device::debug = true;

  //*****************************************************************************
  //Initialize the "Everything" Class
  //*****************************************************************************

  //Initialize the optional local callback routine (safe to comment out if not desired)
  st::Everything::callOnMsgSend = callback;

  //Create the SmartThings ESP8266WiFi Communications Object
  //STATIC IP Assignment - Recommended
  st::Everything::SmartThing = new st::SmartThingsESP8266WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, st::receiveSmartString);

  //DHCP IP Assigment - Must set your router's DHCP server to provice a static IP address for this device's MAC address
  //st::Everything::SmartThing = new st::SmartThingsESP8266WiFi(str_ssid, str_password, serverPort, hubIp, hubPort, st::receiveSmartString);

  //Run the Everything class' init() routine which establishes WiFi communications with SmartThings Hub
  st::Everything::init();

  //*****************************************************************************
  //Add each sensor to the "Everything" Class
  //*****************************************************************************
  st::Everything::addSensor(&sensor1);
  st::Everything::addSensor(&sensor2);
  st::Everything::addSensor(&sensor3);
  st::Everything::addSensor(&sensor4);
  st::Everything::addSensor(&sensor5);
  st::Everything::addSensor(&sensor6);

  //*****************************************************************************
  //Add each executor to the "Everything" Class
  //*****************************************************************************
  st::Everything::addExecutor(&executor1); // Add the IR switch1
  st::Everything::addExecutor(&executor2); // Add the IR switch2
  st::Everything::addExecutor(&executor3); // Add the IR switch3

  //*****************************************************************************
  //Initialize each of the devices which were added to the Everything Class
  //*****************************************************************************
  st::Everything::initDevices();

}

//******************************************************************************************
//Arduino Loop() routine
//******************************************************************************************
void loop()
{
  //*****************************************************************************
  //Execute the Everything run method which takes care of "Everything"
  //*****************************************************************************
  st::Everything::run();
}
