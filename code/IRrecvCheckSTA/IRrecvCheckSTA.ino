/*
 * IRremoteESP8266: IRrecvCheckSTA - Provides IR codes and other information
 * required to use the ST_Anything_IR_ESP8266 library
 * 
 * This only provides details on protocols that the ST Anything library can currently transmit
 * There are several other protocols supported by the transmit library, I will update in future
 * 
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * 
 * I used an IR receive module, readily available for the Arduino
 * Connected as below:
 * IR Rx  NodeMCU
 * G      Ground
 * R      3v
 * Y      D5
 * 
 * Based on work copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/markszabo/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.3 August, 2018
 *     Repurposed for the ST_Anything_IR_ESP8266 project
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
uint16_t RECV_PIN = 14;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(RECV_PIN);
}

void loop() {
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)

    // Show the IR code that was received
    Serial.print("Code : 0x");
    serialPrintUint64(results.value, HEX);
    Serial.println("");

    // Show the number of bits
    Serial.print("Bits: ");
    Serial.println(results.bits);

    // Show the protocol as needed for the ST Anything IR library
    Serial.print("Protocol: ");

    switch(results.decode_type)
    {
    case 1: Serial.print("3, RC5");break;
    case 2: Serial.print("4, RC6");break;
    case 3: Serial.print("1, NEC");break;
    case 4: Serial.print("2, SONY");break;
    case 6: Serial.print("6, JVC");break;
    case 7: Serial.print("7, SAMSUNG");break;
    case 8: Serial.print("9, WHYNTER");break;
    case 9: Serial.print("17, AIWARCT501");break;
    case 10: Serial.print("8, LG");break;
    case 12: Serial.print("14, MITSUBISHI");break;
    case 13: Serial.print("5, DISH");break;
    case 14: Serial.print("16, SHARP");break;
    case 15: Serial.print("10, COOLIX");break;
    case 17: Serial.print("11, DENON");break;
    case 19: Serial.print("12, SHERWOOD");break;
    case 21: Serial.print("13, RCMM");break;
    case 34: Serial.print("18, MIDEA");break;
    case 39: Serial.print("15, MITSUBISHI2");break;
    case 43: Serial.print("19, GICABLE");break;
    default:
      Serial.println("Other protocol that needs to be added");
      Serial.print(resultToHumanReadableBasic(&results));
    }

    Serial.println("");
    Serial.println("");
    
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}
