#include <mdht.h>

void mDHT::_async()
{
    DHT dht(m_pin, DHT22);
    dht.begin();

    while(1) {
        delay(3000);
        m_temp = dht.readTemperature();
        m_hum = dht.readHumidity();
    }

    vTaskDelete(NULL);
}

mDHT::mDHT(const gpio_num_t pin)
    : m_pin(pin)
{
    xTaskCreate([](void* thus){((mDHT*)thus)->_async();}, "ASYNC_DHT", 2048, this, tskIDLE_PRIORITY, &m_thr);
}

mDHT::~mDHT()
{
    vTaskDelete(m_thr);
    m_thr = nullptr;
}

float mDHT::get_temperature() const
{
    return m_temp;
}

float mDHT::get_humidity() const
{
    return m_hum;
}
