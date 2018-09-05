# ST_Anything_IR

This library extends the ST_Anything library to add IR remote functionality to Samsung SmartThings.

This project uses an IR led connected to a NodeMCU to transmit pre-configured IR codes to various devices.

There are several circuits you can build to transmit the codes with an IR LED, it is recommended to use an NPN transistor, such as a 2N2222, to allow more power to the LED as the digital output pin cannot deliver much current.  The circuit I am using is the same as shown in the link below:
https://learn.adafruit.com/using-an-infrared-library/sending-ir-codes

Using this I am able to turn my LG television on and off from across the room, around 4m away.  My Samsung TV works from a shorter distance, around 2m, it could be that the TV receiver is less sensitive.

The first release of the code represents an IR controlled device as an On/Off button in SmartThings.  The main use case is to on and off a device that uses the same code for both operations, a typical example of this is a TV where the power button normally does both operations.

To use the library you need to do the following:
1) Get up and running with the ST Anything system https://github.com/DanielOgorchock/ST_Anything
2) Download the library IRremoteESP8266 through the Arduino IDE Manage Libraries interface.
3) Add the EX_SwitchIR.h and EX_SwitchIR.cpp files to the your Arduino/libraries/ST_Anything folder
4) Load up the example sketch and modify the required parameters:
    - Set the SSID of your WiFi network
    - Set the password for your WiFi network
    - Set the IP address of the NodeMCU
    - Set the gateway address, this will normally be your WiFi router
    - Set the subnet mask, most home networks use 255.255.255.0
    - Set the DNS server, typically the same as the gateway address
    - Set the IP address of your SmartThings hub
    - Set the IR code, length and protocol type (see below on how to find the code)  The protocol type list is shown in the EX_SwitchIR.cpp file.
5) Connect up an IR LED to your NodeMCU as described above

Finding IR Codes
To use this library you need to know the correct IR code to transmit, the length of the code and the protocol to use.  To find this information I used an IR receiver connected to an Arduino Uno.  There are lots of tutorials online for how to do this, for example:
https://learn.adafruit.com/using-an-infrared-library/hardware-needed
My example sketch includes the codes I found for my LG and Samsung TV power buttons.

Next Steps
As the next steps for this library I have planned the following:
1) Create a momentary push button that does not toggle on and off, this could be used for cases such as adjusting a TV channel or the volume.
2) Create an IR macro button that can send a series of IR codes, perhaps to turn on the TV, change to a certain input and adjust the volume all with one click.
