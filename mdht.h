#include "DHT.h"

#include <FreeRTOS.h>
#include <task.h>
#include <driver/gpio.h>

class mDHT {
    TaskHandle_t m_thr = nullptr;
    const gpio_num_t m_pin;
    float m_temp{}, m_hum{};

    void _async();
public:
    mDHT(const mDHT&) = delete;
    mDHT(mDHT&&) = delete;
    void operator=(const mDHT&) = delete;
    void operator=(mDHT&&) = delete;

    mDHT(const gpio_num_t);
    ~mDHT();

    float get_temperature() const;
    float get_humidity() const;
};

inline void mDHT::_async()
{
    DHT dht(m_pin, DHT22);
    dht.begin();

    while(1) {
        delay(2050);
        m_temp = dht.readTemperature();
        m_hum = dht.readHumidity();
    }

    vTaskDelete(NULL);
}

inline mDHT::mDHT(const gpio_num_t pin)
    : m_pin(pin)
{
    xTaskCreate([](void* thus){((mDHT*)thus)->_async();}, "ASYNC_DHT", 2048, this, tskIDLE_PRIORITY, &m_thr);
}

inline mDHT::~mDHT()
{
    vTaskDelete(m_thr);
    m_thr = nullptr;
}

inline float mDHT::get_temperature() const
{
    return m_temp;
}

inline float mDHT::get_humidity() const
{
    return m_hum;
}
