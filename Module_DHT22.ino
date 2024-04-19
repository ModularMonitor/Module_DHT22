#define ESP32C3_M5_KIT

#include <Arduino.h>
#include <Wire.h>
#include "mdht.h"
#include "Serial/package.h"

mDHT* dht = nullptr;
const auto this_device = CustomSerial::device_id::DHT22_SENSOR;

void send_to_wire_on_request();

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  Serial.printf("Begin!\n");

  CustomSerial::begin_slave(this_device, send_to_wire_on_request);
  dht = new mDHT(GPIO_NUM_14);
}

void loop()
{
  delay(2000);
  Serial.printf("T=%.2fC; H=%.2f%%\n", dht->get_temperature(), dht->get_humidity());
}

void send_to_wire_on_request()
{  
  CustomSerial::command_package cmd(this_device, 
    "/temperature", dht->get_temperature(),
    "/humidity", dht->get_humidity()
  );

  CustomSerial::write(cmd);
}
