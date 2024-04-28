#ifndef ACCESSORY_LIGHT_HPP
#define ACCESSORY_LIGHT_HPP

#include <base_accessory.hpp>
#include <cstdint>
#include <driver/gpio.h>
#include <esp_matter_attribute_utils.h>

class LightAccessory : public BaseAccessory {
public:
    LightAccessory(uint8_t button_pin, uint8_t led_pin);
    ~LightAccessory();
    esp_err_t setLight(bool on);
    bool getLight();
    esp_err_t attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                               esp_matter_attr_val_t *val) override;
    esp_err_t identification(uint16_t endpoint_id) override;
};

#endif // ACCESSORY_LIGHT_HPP