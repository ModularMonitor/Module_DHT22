#include <Arduino.h>
#include <mdht.h>

mDHT* dht = nullptr;

void setup() {
  Serial.begin(9600);
  while(!Serial){}

  dht = new mDHT(GPIO_NUM_8);
}

void loop() {
  delay(1000);
  Serial.printf("READ: %.2fC %.2f%%\n", dht->get_temperature(), dht->get_humidity());
}
