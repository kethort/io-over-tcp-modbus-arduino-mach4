#include <ModbusIP.h>
ModbusIP mb; // create modbus device

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  
byte ip[] = {192, 168, 0, 50}; 

void setup() {
  Serial.begin(115200);

  mb.config(mac, ip); // modbus tcp/ip setup

  // create modbus holding registers
  mb.addHreg(55);
  mb.addHreg(56);
}

void loop() {
  mb.task(); // modbus polling function

  printFloatValue(55, 56);
}

void printFloatValue(byte hreg1, byte hreg2) {
  int16_t numPrefix = mb.Hreg(hreg1); //read first holding register
  int16_t numPostfix = mb.Hreg(hreg2); // read second holding register
  float intToDec = numPrefix + (numPostfix / 10000.0); // convert registers into decimal

  Serial.println(intToDec, 4); 
}
