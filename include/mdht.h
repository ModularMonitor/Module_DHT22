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