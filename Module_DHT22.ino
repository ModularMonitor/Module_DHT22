#include <Arduino.h>
#include <Wire.h>
#include "mdht.h"
#include "Serial/packaging.h"

using namespace CS;

PackagedWired* wire;
mDHT* dht = nullptr;
constexpr int port_DHT22 = GPIO_NUM_14;
const auto this_device = device_id::DHT22_SENSOR;

void callback(void*, const uint8_t, const char*, const uint8_t);

void setup()
{
    Serial.begin(115200);
    while(!Serial);
    
    Serial.printf("Starting SLAVE\n");
    
    dht = new mDHT(port_DHT22);
    
    wire = new PackagedWired(config()
        .set_slave(this_device)
        .set_slave_callback(callback)
        .set_led(2)
    );
}

void callback(void* rw, const uint8_t expects, const char* received, const uint8_t length)
{
    if (length != sizeof(Requester)) return;
    
    PackagedWired& w = *(PackagedWired*) rw;
    Requester req(received);
    
    switch(req.get_offset()) {
    case 0:
    {
        const float val = dht->get_temperature();
        Command cmd("/dht/temperature", val);
        w.slave_reply_from_callback(cmd);
        //Serial.printf("Received request {%zu}\nReplying with temperature %.3f\n", req.get_offset(), val);
    }
    break;
    case 1:
    {
        const float val = dht->get_humidity();
        Command cmd("/dht/humidity", val);
        w.slave_reply_from_callback(cmd);
        //Serial.printf("Received request {%zu}\nReplying with humidity %.3f\n", req.get_offset(), val);
    }
    break;
    default:
    {
        Command cmd; // invalid
        w.slave_reply_from_callback(cmd);
        //Serial.printf("Received request {%zu}\nConsidered invalid!\n", req.get_offset());
    }
    }
}

// unused
void loop() { vTaskDelete(NULL); }