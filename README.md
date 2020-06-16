# Arduino Mega W5500 Mach4 Modbus Over TCP

This sketch requires the use of a W5100 or W5500 ethernet adapter.

This sketch allows control of 32 inputs and 32 outputs using the Mach4 modbus plugin. 

The sketch accesses the Arduino pins directly using port manipulation for faster signaling. 

The TestModbus.m4prof file is a Mach4 profile which includes the configuration and signal map for using the IO with the Modbus plugin.



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
