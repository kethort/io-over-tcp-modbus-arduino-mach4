#include <ModbusIP.h>
ModbusIP mb; // create modbus device

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  
byte ip[] = {192, 168, 0, 50}; 

void setup() {
  Serial.begin(115200);

  mb.config(mac, ip); // modbus tcp/ip setup

  // create modbus holding registers
  mb.addHreg(58);
}

void loop() {
  mb.task(); // modbus polling function

  int16_t regVal = mb.Hreg(58); //read holding register
  Serial.println(regVal);
}
