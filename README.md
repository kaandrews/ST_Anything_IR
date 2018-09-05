# ST_Anything_IR

This library extends the ST_Anything library to add IR remote functionality to Samsung SmartThings.

This uses an IR led to transmit pre-configured codes to activate devices.  The details of the simple circuit are shown in the EX_SwitchIR.cpp file.

The initial version toggles on and off using the same code to turn the device on and off, as is the case with something like a TV.

To use the library you need to do the following:
1) Get up and running with the ST Anything system https://github.com/DanielOgorchock/ST_Anything
2) Add the Arduino library IRremoteESP8266 through the Arduino IDE Manage Libraries.
3) Add the EX_SwitchIR.h and EX_SwitchIR.cpp files to the your Arduino/libraries/ST_Anything folder
4) Load up the example sketch and set the IP address of your router etc
5) Connect up an IR LED to your NodeMCU

To use this library you do also need to find the IR code you need to transmit to control your device.  I used an IR receiver connected to an Arduino Uno.  There are tons of tutorials online for how to do this, for example:
https://learn.adafruit.com/using-an-infrared-library/hardware-needed

