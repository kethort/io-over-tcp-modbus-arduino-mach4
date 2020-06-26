# Arduino Mega W5500 Mach4 Modbus via TCP

This sketch requires the use of a W5100 or W5500 ethernet adapter and optionally supports either an Adafruit ADS1115 ADC and MCP4725 DAC device (or both). To use the ADC or DAC uncomment the #defines at the top of the sketch. 

The program allows control of 32 inputs and 32 outputs using the Mach4 modbus plugin. 

The sketch accesses the Arduino pins directly using port manipulation for faster signaling. 

There are 3 different versions of the Mach4 profile. To use just the digital inputs and outputs use IO_Modbus.m4prof. Otherwise, use either IO_With_ADC.m4prof, IO_With_DAC.m4prof or IO_With_ADC_DAC.m4prof. 

<p align="center">
  <img src="/img/Mach4Modbus.PNG" width="750"/>
</p>

Pin 53 is used as the SS or CS pin so that pin 10 can be used as an output. If you are using the W5100 ethernet shield, then pin 10 should be bent or cut off and wire should be connected from pin 10 on the Ethernet shield to pin 53 of the Arduino Mega.

<p align="center">
  <img src="/img/bodge.jpg" width="750"/>
</p>

The IP address in the sketch and the Mach4 modbus plugin configuration must be setup according to the network configuration of the connected device. The modbus plugin configuration can be accessed through the Mach4 file menu at: Configure>Plugins...>Modbus. 

<p align="center">
  <img src="/img/Mach4ModbusIPConfig.PNG" width="750"/>
</p>

The following image shows the response time difference of using a for loop in the Arduino loop() function versus just using the loop() function and incrementing a global position variable to read/write modbus registers.

<p align="center">
  <img src="/img/loopTimeDifference.PNG" width="750"/>
</p>

### ADC & DAC
The ADC & DAC inputs and outputs communicate over the i2C bus. Use the i2C scanner at this link: https://gist.github.com/xzenzza/62d35c316191d3a5bae1da68374d4790 to find the address of the devices you are using and change the variables in the sketch accordingly.


### Output Pins
{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 
 24, 25, 26, 27, 28, 29, 38, 39, 40, 41}
 
### Input Pins
{A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10,  
 A11, A12, A13, A14, A15, 30, 31, 32, 33, 34, 
 35, 36, 37, 42, 43, 44, 45, 46, 47, 48, 49}

## Modbus TCP Library Used (the edited version for Arduino Mega is included in the repo)
https://github.com/andresarmento/modbus-arduino

## ADC and DAC libraries
https://github.com/adafruit/Adafruit_MCP4725
https://github.com/adafruit/Adafruit_BusIO

https://github.com/adafruit/Adafruit_ADS1X15