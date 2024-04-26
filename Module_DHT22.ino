#include <Arduino.h>
#include <Wire.h>
#include "mdht.h"
#include "Serial/package.h"

mDHT* dht = nullptr;
constexpr int port_DHT22 = GPIO_NUM_14;

const auto this_device = CustomSerial::device_id::DHT22_SENSOR;

// request event call:
void send_to_wire_on_request();

void setup()
{
    Serial.begin(115200);
    while(!Serial);
    
    CustomSerial::set_logging(Serial);
    CustomSerial::print_info();
    CustomSerial::begin_slave(this_device, send_to_wire_on_request);
    
    dht = new mDHT(port_DHT22);
}

void send_to_wire_on_request()
{
  CustomSerial::command_package cmd(this_device, 
    "/temperature", dht->get_temperature(),
    "/humidity", dht->get_humidity()
  );

  CustomSerial::write(cmd);
}

// unused
void loop() { vTaskDelete(NULL); }