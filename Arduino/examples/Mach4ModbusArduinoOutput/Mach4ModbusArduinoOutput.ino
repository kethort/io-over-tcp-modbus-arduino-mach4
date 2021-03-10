#include <ModbusIP.h>
ModbusIP mb; // create modbus device

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  
byte ip[] = {192, 168, 0, 50}; 

void setup() {
  Serial.begin(115200);

  mb.config(mac, ip); // modbus tcp/ip setup

  // using pin 2 on Arduino as the output
  pinMode(2, OUTPUT);
  
  // create modbus coil registers
  mb.addCoil(5);
}

void loop() {
  mb.task(); // modbus polling function

  // change the state of the Arduino output based on the value of the modbus register
  digitalWrite(2, mb.Coil(5)); // if the modbus register is high (value is 1) then pin 2 will be on
}
