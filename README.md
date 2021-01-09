# Arduino Mega W5500 Mach4 Modbus via TCP

This sketch requires the use of a W5100 or W5500 ethernet adapter and optionally supports either an Adafruit ADS1115 ADC and MCP4725 DAC device (or both). To use the ADC or DAC uncomment the #defines at the top of the sketch. 

<p align="center">
  <img src="/img/sharp_defines.PNG" height="125" width="700"/>
</p>

The program allows control of 32 inputs and 32 outputs using the Mach4 modbus plugin. 

The sketch accesses the Arduino pins directly using port manipulation for faster signaling. The input pins are active low (NPN -GND) and the output pins are active high (PNP +5v). 

You can either use one of the included profiles in the Mach4 Profiles directory, or use the ArduinoModbusInstaller.exe from the dist folder of the repo to update an existing Mach4 profile.


### ADC & DAC
The ADC & DAC inputs and outputs communicate over the i2C bus. Use the i2C scanner at this link: https://gist.github.com/xzenzza/62d35c316191d3a5bae1da68374d4790 to find the address of the devices you are using and change the variables in the sketch accordingly.

<p align="center">
  <img src="/img/i2c_address.PNG" width="300"/>
</p>

## ADC and DAC libraries (if using the ADC and/or DAC modules)
https://github.com/adafruit/Adafruit_MCP4725 <br>
https://github.com/adafruit/Adafruit_BusIO <br>
https://github.com/adafruit/Adafruit_ADS1X15

## Firmware and Network Setup
1. Copy the nested folders in the <b>libraries</b> directory to the Arduino sketch folder on your computer (usually in Documents/Arduino/libraries). 
2. Change the IP address of the Arduino in the Mach4ModbusW5500.ino sketch to conform to your network. For example, if your network subnet is 192.168.1.X then change the IP address to something like 192.168.1.85.  

<p align="center">
  <img src="/img/edit_IP.PNG" width="500"/>
</p>

3. Upload the Mach4ModbusW5500.ino file to the Arduino Mega using the Arduino (or other) IDE.
4. Connect the Arduino to power and the network.

## Mach4 Software Setup (installer)
There are several configurations that can be made using the ArduinoModbusInstaller.exe program. <br>
&ensp;&ensp;a. Use just the inputs and outputs on the Arduino Mega (this is the default). <br>
&ensp;&ensp;b. Use inputs outputs and ADC. <br>
&ensp;&ensp;c. Use inputs outputs and DAC. <br>
&ensp;&ensp;d. Use inputs outputs ADC and DAC. <br>

To change these configurations select the appropriate check box(es) in the installer.

<p align="center">
  <img src="/img/adc_dac_check.PNG" width="400"/>
</p>

1. Launch the <b>ArduinoModbusInstaller.exe</b> from the dist folder of the repo.
2. Select the Mach4 profile you want to update, enter the IP address of the Arduino and press install (ip address is from the Arduino sketch). 

<p align="center">
  <img src="/img/ip_address.PNG" height="500" width="700"/>
</p>

4. Open Mach4 and navigate to Diagnostic->Modbus in the File Menu.
5. To connect or reconnect to the modbus device, press the stop button in the diagnostic window and then the play button. 

<p align="center">
  <img src="/img/modbus_diag.PNG" width="300"/>
</p>
 
## Parts List
1.  [Arduino Mega](https://www.amazon.com/ARDUINO-MEGA-2560-REV3-A000067/dp/B0046AMGW0/ref=sr_1_4?dchild=1&keywords=arduino+mega&qid=1609909788&s=electronics&sr=1-4)
2.  [W5100 Ethernet Shield](https://www.amazon.com/SunFounder-Ethernet-Shield-W5100-Arduino/dp/B00HG82V1A/ref=sr_1_1_sspa?dchild=1&keywords=w5100&qid=1609909809&s=electronics&sr=1-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyM0FCNVY1VDRXOUI2JmVuY3J5cHRlZElkPUEwODQ1MzQ4M0FMMTlHRzdTWUw1MCZlbmNyeXB0ZWRBZElkPUEwNTI1MzE2MzJJR0pGTUxXVUJLRyZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)
3.  [W5500 Ethernet Shield](https://www.amazon.com/HiLetgo-Ethernet-Network-Interface-WIZ820io/dp/B08KXM8TKJ/ref=sr_1_2?dchild=1&keywords=w5500&qid=1609909837&s=electronics&sr=1-2)
4.  [12-bit DAC](https://www.amazon.com/Adafruit-MCP4725-Breakout-Board-Interface/dp/B00SK8MBXI)
5.  [ADS1115 ADC](https://www.amazon.com/HiLetgo-Converter-Programmable-Amplifier-Development/dp/B01DLHKMO2/ref=sr_1_1_sspa?crid=1IGPUNOC1A5R2&dchild=1&keywords=ads1115&qid=1609909759&s=electronics&sprefix=ads1%2Celectronics%2C210&sr=1-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUExUUtHNVYzREtSSEdEJmVuY3J5cHRlZElkPUEwOTkwMzI0MUpTMzJTMjYyUTVWUiZlbmNyeXB0ZWRBZElkPUEwNDAyOTIzMTlLVEM3UjJER1RFWiZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)

## W5100 Connection

Pin 53 is used as the SS or CS pin so that pin 10 can be used as an output. If you are using the W5100 ethernet shield, then pin 10 should be bent or cut off and wire should be connected from pin 10 on the Ethernet shield to pin 53 of the Arduino Mega.

<p align="center">
  <img src="/img/bodge.jpg" width="750"/>
</p>

## Mach4 Software Setup (profile)
<i>***This setup is not necessary of you used the exe installer; only if you are importing one of the provided Mach4 profiles in the repo</i>
  
The IP address in the sketch and the Mach4 modbus plugin configuration must be setup according to the network configuration of the connected device. The modbus plugin configuration can be accessed through the Mach4 file menu at: Configure>Plugins...>Modbus. 

<p align="center">
  <img src="/img/Mach4ModbusIPConfig.PNG" width="750"/>
</p>

### Output Pins
{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 
 24, 25, 26, 27, 28, 29, 38, 39, 40, 41}
 

### Input Pins
{A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10,  
 A11, A12, A13, A14, A15, 30, 31, 32, 33, 34, 
 35, 36, 37, 42, 43, 44, 45, 46, 47, 48, 49}

<p align="center">
  <img src="/img/Pins1.png" width="750"/>
</p>

<p align="center">
  <img src="/img/Pins2.png" width="750"/>
</p>

## Demonstration
https://www.youtube.com/watch?v=koNoJPKRC_M

## Modbus TCP Library Used (the edited version for Arduino Mega is included in the repo)
https://github.com/andresarmento/modbus-arduino

## Miscellaneous Tests
The following image shows the response time difference of using a for loop in the Arduino loop() function versus just using the loop() function and incrementing a global position variable to read/write modbus registers. Using just the Arduino loop results in much faster response times.

<p align="center">
  <img src="/img/loopTimeDifference.PNG" width="750"/>
</p>
