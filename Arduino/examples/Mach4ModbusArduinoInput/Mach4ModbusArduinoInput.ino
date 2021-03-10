#include <ModbusIP.h>
ModbusIP mb; // create modbus device

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  
byte ip[] = {192, 168, 0, 50}; 

void setup() {
  Serial.begin(115200);

  mb.config(mac, ip); // modbus tcp/ip setup

  // using pins 2 and 3 on Arduino define them as active low (active when GND applied to pin)
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  
  // create modbus coil registers
  mb.addCoil(1);
  mb.addCoil(2);
}

void loop() {
  mb.task(); // modbus polling function

  // put the current state of the inputs into their coil register (inverse because active low)
  mb.Coil(1, !digitalRead(2)); // modbus register 1 holds the state of input 2 
  mb.Coil(2, !digitalRead(3)); // modbus register 2 holds the state of input 3
}
