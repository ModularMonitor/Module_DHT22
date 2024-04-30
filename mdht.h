#pragma once

#include "Adafruit_Sensor/Adafruit_Sensor.h"
#include "DHT-sensor-library/DHT.h"

#include "SelfThreadable/self_threadable.h"

// mDHT wraps DHT in a async class. This way, less things to worry about.
class mDHT : protected SelfThreadable {
    const int m_pin;
    float m_temp{}, m_hum{};

    void async() {
        DHT dht(m_pin, DHT22);
        dht.begin();

        while(1) {
            delay(2050);
            m_temp = dht.readTemperature();
            m_hum = dht.readHumidity();
        }

        vTaskDelete(NULL);
    }
public:
    mDHT(const int pin) : SelfThreadable("ASYNC_DHT"), m_pin(pin) { async_start(); }

    float get_temperature() const;
    float get_humidity() const;
};

inline float mDHT::get_temperature() const
{
    return m_temp;
}

inline float mDHT::get_humidity() const
{
    return m_hum;
}
