# Arduino Mega W5500 Mach4 Modbus via TCP

This sketch requires the use of a W5100 or W5500 ethernet adapter.

Allows control of 32 inputs and 32 outputs using the Mach4 modbus plugin. 

Accesses the Arduino pins directly using port manipulation for faster signaling. 

The TestModbus.m4prof file is a Mach4 profile which includes the configuration and signal map for using the IO with the Modbus plugin.

<p align="center">
  <img src="/img/Mach4Modbus.PNG" width="500"/>
</p>

Pin 53 is used as the SS or CS pin so that pin 10 can be used as an output. If you are using the W5100 ethernet shield, then pin 10 should be bent or cut off and wire should be connected from pin 10 on the Ethernet shield to pin 53 of the Arduino Mega.

<p align="center">
  <img src="/img/bodge.jpg" width="500"/>
</p>

### Output Pins
{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 
 24, 25, 26, 27, 28, 29, 38, 39, 40, 41}
 
### Input Pins
{A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10,  
 A11, A12, A13, A14, A15, 30, 31, 32, 33, 34, 
 35, 36, 37, 42, 43, 44, 45, 46, 47, 48, 49}

## External Library Used (the edited version for Arduino Mega is included in the repo)
https://github.com/andresarmento/modbus-arduino
