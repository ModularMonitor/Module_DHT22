#include <Arduino.h>
#include "mdht.h"
#include "Serial/package.h"

mDHT* dht = nullptr;

void setup() {
  Serial.begin(9600);
  while(!Serial){}

	//MMSerial::setup(MMSerial::device_id::DHT22);

  dht = new mDHT(GPIO_NUM_8);
}

void loop() {
  delay(1025);
  
  MMSerial::send_package("/dht22/temperature", dht->get_temperature());
  MMSerial::send_package("/dht22/humidity", dht->get_humidity());

  Serial.printf("READ: %.2fC %.2f%%\n", dht->get_temperature(), dht->get_humidity());
}
