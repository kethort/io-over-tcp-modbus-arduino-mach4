/*
-----------------------------------------------------------------------------
-- Name:        Mach4 Modbus with W5500, ADS1115 & MCP4725
-- Author:      Kenneth Orton
-- Created:     06/15/2020
-----------------------------------------------------------------------------
 */

// LSB      P0  P1  P2  P3  P4  P5, P6, P7    MSB
//-------------------------------------------------------------
// OUTPUTS
//-------------------------------------------------------------
// PORTA = {22, 23, 24, 25, 26, 27, 28, 29};       

// PORTG = {41, 40, 39, xx, xx, 4,  xx, xx};
// PORTB = {xx, xx, xx, xx, 10, 11, 12, 13}; // w5500 ethernet module uses 53, 52, 51, and 50;
                                             // cannot use those pins
// PORTD = {21, 20, 19, 18, xx, xx, xx, 38};
// PORTE = {xx, xx, xx, 5,  2,  3,  xx, xx};

// PORTH = {17, 16, xx, 6,  7,  8,  9,  xx};
// PORTJ = {15, 14, xx, xx, xx, xx, xx, xx};

//-------------------------------------------------------------
// INPUTS
//-------------------------------------------------------------
// PORTF = {A0, A1, A2, A3, A4, A5, A6, A7};         
// PORTK = {A8, A9, A10, A11, A12, A13, A14, A15}; 
// PORTC = {37, 36, 35, 34, 33, 32, 31, 30};       
// PORTL = {49, 48, 47, 46, 45, 44, 43, 42};      
//-------------------------------------------------------------

// Uncomment one or both of these if using the Adafruit ADC or DAC devices
//#define USE_ADC
//#define USE_DAC

#define ADC_DIV 266

#ifdef USE_ADC || USE_DAC
#include <Wire.h>
#endif

#ifdef USE_ADC
#include <Adafruit_ADS1015.h>
#define ADC_ADDR 0x48
Adafruit_ADS1115 ads(ADC_ADDR);
#endif

#ifdef USE_DAC
#include <Adafruit_MCP4725.h>

#define DAC_ADDR 0x62
Adafruit_MCP4725 dac;
#endif

#include <ModbusIP.h>
ModbusIP mb;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  
byte ip[] = {192, 168, 0, 50}; 

byte pos = 0;

/*
    The modbus TCP protocol only processes register addresses 7 bytes at a time. Every eighth register
    address (byte in the modbusRegs array) does not get sent through. 
    That's why they are skipped in the register array and in the Mach4 modbus plugin configuration.

    The bytes in the modbusRegs array do not have to be in any order, but the value of the bytes do have to be different
    in each location to avoid collisions on the other side.  
*/
byte modbusRegs[] = {1,  2,  3,  4,  5,  6,  7,  9,   // 0 - 7    PORTF
                     10, 11, 12, 13, 14, 15, 17, 18,  // 8 - 15   PORTK
                     19, 20, 21, 22, 23, 25, 26, 27,  // 16 - 23  PORTC
                     28, 29, 30, 31, 33, 34, 35, 36,  // 24 - 31  PORTL
                     
                     37, 38, 39, 41, 42, 43, 44, 45,  // 32 - 39  PORTA
                     46, 47, 49, 50, 51, 52, 53, 54,  // 40 - 47  PORTB & PORTG
                     55, 57, 58, 59, 60, 61, 62, 63,  // 48 - 55  PORTD & PORTE
                     65, 66, 67, 68, 69, 70, 71, 73,  // 56 - 63  PORTH & PORTJ
                     74, 75, 76, 77, 78};             // 64 - 68  ADC (4 channels i2c bus)
               
byte numRegs = sizeof(modbusRegs)/sizeof(modbusRegs[0]);

void setup() {
#ifndef USE_ADC || USE_DAC
  numRegs -= 5;
#endif
  
#ifdef USE_ADC
  ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV  0.1875mV (default)
  ads.begin();
#endif

#ifdef USE_DAC
  dac.begin(DAC_ADDR);
#endif
  
  // DDR is the port data direction; outputs are 1 inputs are 0

  // define inputs 
  DDRF = DDRK = DDRC = DDRL = 0;
  PORTF = PORTK = PORTC = PORTL = 0xFF; // initalizes the input pins as HIGH or pullups

  // define outputs
  DDRA = 0xFF; 
  PORTA = 0;
  
  DDRB = 0xF1; 
  PORTB &= ~0xF1;
  
  DDRD = 0x8F;
  PORTD &= ~0x8F;
  
  DDRE = 0x38;
  PORTE &= ~0x38;
  
  DDRG = 0x27;
  PORTG &= ~0x27;
  
  DDRH = 0x7B;
  PORTH &= ~0x7B;
  
  DDRJ = 0x03;
  PORTJ = ~0x03;
   
  mb.config(mac, ip);
  
  for(byte i = 0; i < numRegs; i++) {
    if (i >= 64 && i <= 67) {     // ADC registers 
      mb.addIreg(modbusRegs[i]);   
    } else if (i == 68) {         // DAC register
      mb.addHreg(modbusRegs[i]);
    } else {
      mb.addCoil(modbusRegs[i]); // input and output registers
    }
  }
}

/*
    The pos variable is used to shift the bit to the pin on the PORT. It also points to the
    modbus register number in the modbusRegs array and is used to switch to the next 
    port after each full read/write of the 8-bits in the port is finished. This allows the Arduino
    loop() to iterate through all of the ports and pins on the device without the use of for loops.
*/
void loop() {  
  mb.task();

  pos = pos == numRegs ? 0 : pos;

  readInputs();
  processOutputs();

#ifdef USE_ADC || USE_DAC
  readADCwriteDAC();
#endif

  pos++;
}

/*
    The readInputs function reads the pin value from the PORT bit-map and sets the modbus
    register to the current state.  
*/
void readInputs() {
  byte switchState = pos / 8; // switch to a new port every 8 bits
  byte invertedState = 0;
  
  switch(switchState) {
    case 0:
      // pins A0 - A7
      invertedState = ((PINF >> pos % 8) & 0x01) ? 0 : 1; // inputs are active low; this gets the inverted state of the pin
      mb.Coil(modbusRegs[pos], invertedState); // put the input state into the coil register
      break;
    case 1:
      // pins A8 - A15
      invertedState = ((PINK >> pos % 8) & 0x01) ? 0 : 1;
      mb.Coil(modbusRegs[pos], invertedState);
      break;
    case 2: 
      // pins 37 - 30
      invertedState = ((PINC >> pos % 8) & 0x01) ? 0 : 1;
      mb.Coil(modbusRegs[pos], invertedState);
      break;
    case 3:
      // pins 49 - 42
      invertedState = ((PINL >> pos % 8) & 0x01) ? 0 : 1;
      mb.Coil(modbusRegs[pos], invertedState);
      break;
  }
}

/*
    The processOutputs function reads the current modbus coil state in the register array and 
    writes the value of that bit to the port.   
    
    This function requires much more positional manipulation through the ports due to 
    the scattered orientation of the pins across the rest of the unused ports. 

    As a result, the bit manipulation is brute forced using various if statements, so changing
    any of the output pins might be a difficult task and isn't recommended. 
    
    The advantage to the port manipulation is much faster write access vs the Arduino digitalWrite function.
    
    Separating the input and output ports in this way was intentional, as it was the easiest way to 
    'split' the Arduino Mega in half with inputs on one side of the PCB, and the outputs on the other side 
    while maintaining organization of the port bit-masks in the code.
*/
void processOutputs() {
  byte switchState = pos / 8; // switch to a new port every 8 bits

  switch(switchState) {
    case 4: // pins 22 - 29
      PORTA = mb.Coil(modbusRegs[pos]) ? PORTA | (1 << (pos % 8)) : PORTA & ~(1 << (pos % 8));
      break;
    case 5:
      if (pos >= 40 && pos <= 42) { 
        // pins 41, 40 and 39
        PORTG = mb.Coil(modbusRegs[pos]) ? PORTG | (1 << (pos % 8)) : PORTG & ~(1 << (pos % 8));
      }else if (pos == 43) { 
        // pin 10
        PORTB = mb.Coil(modbusRegs[pos]) ? PORTB | (1 << ((pos + 1) % 8)) : PORTB & ~(1 << ((pos + 1) % 8));
      }else if (pos == 44) { 
        // pin 4
        PORTG = mb.Coil(modbusRegs[pos]) ? PORTG | (1 << ((pos + 1) % 8)) : PORTG & ~(1 << ((pos + 1) % 8));
      } else if (pos >= 45 && pos <= 47) { 
        // pins 11 - 13
        PORTB = mb.Coil(modbusRegs[pos]) ? PORTB | (1 << (pos % 8)) : PORTB & ~(1 << (pos % 8));
      }
      break;
    case 6: 
      if (pos >= 48 && pos <= 51) { 
        // pins 21, 20 and 19
        PORTD = mb.Coil(modbusRegs[pos]) ? PORTD | (1 << (pos % 8)) : PORTD & ~(1 << (pos % 8));
        
        if (pos == 51) {
          // pin 5
          PORTE = mb.Coil(modbusRegs[pos + 1]) ? PORTE | (1 << (pos % 8)) : PORTE & ~(1 << (pos % 8));
        }
      } else if (pos >= 52 && pos <= 53) {
        // pins 2 and 3
        PORTE = mb.Coil(modbusRegs[pos + 1]) ? PORTE | (1 << (pos % 8)) : PORTE & ~(1 << (pos % 8));
      } else if (pos == 54) {
        // pin 38
        PORTD = mb.Coil(modbusRegs[pos + 1]) ? PORTD | (1 << ((pos + 1) % 8)) : PORTD & ~(1 << ((pos + 1) % 8));
      }
      break;
    case 7:
      if (pos == 56) {
        // pin 17
        PORTH = mb.Coil(modbusRegs[pos]) ? PORTH | (1 << (pos % 8)) : PORTH & ~(1 << (pos % 8));
        // pin 15
        PORTJ = mb.Coil(modbusRegs[pos + 1]) ? PORTJ | (1 << (pos % 8)) : PORTJ & ~(1 << (pos % 8));
      } else if (pos == 57) {
        // pin 16
        PORTH = mb.Coil(modbusRegs[pos + 1]) ? PORTH | (1 << (pos % 8)) : PORTH & ~(1 << (pos % 8));
        // pin 14
        PORTJ = mb.Coil(modbusRegs[pos + 2]) ? PORTJ | (1 << (pos % 8)) : PORTJ & ~(1 << (pos % 8));
      } else if (pos >= 60 && pos <= 63) {
        // pins 6 - 9
        PORTH = mb.Coil(modbusRegs[pos]) ? PORTH | (1 << ((pos - 1) % 8)) : PORTH & ~(1 << ((pos - 1) % 8));
      }
      break;
  } 
}

#ifdef USE_ADC || USE_DAC
void readADCwriteDAC() {
  byte switchState = pos / 8; 
  word adc; // largest value for modbus reg is 16-bits
  word dacVal; 
  
  switch(switchState) {
    case 8:
      switch(pos) {
        case 68:
#ifdef USE_DAC
          dacVal = mb.Hreg(modbusRegs[pos]);
          mb.Hreg(modbusRegs[pos], dacVal);
          dac.setVoltage(dacVal, false);
#endif
          break;
        case 64:
        case 65:
        case 66:
        case 67:
#ifdef USE_ADC
          byte adcChannel = pos % 8;
          adc = ads.readADC_SingleEnded(adcChannel);
          adc = adc/ADC_DIV;
          if (!(adc >= 0 && adc <= 100))
            adc = 0;
          mb.Ireg(modbusRegs[pos], adc);
#endif
          break; 
      } 
  }
}
#endif