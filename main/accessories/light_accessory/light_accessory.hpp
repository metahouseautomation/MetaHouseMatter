#ifndef ACCESSORY_LIGHT_HPP
#define ACCESSORY_LIGHT_HPP

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <cstdint>
#include <esp_matter_attribute_utils.h>
#include <hal/gpio_types.h>

#include "accessories/base_accessory/base_accessory.hpp"

class LightAccessory : public BaseAccessory {
private:
    constexpr static const uint32_t _CLUSTER_ID = chip::app::Clusters::OnOff::Id;
    constexpr static const uint32_t _ATTRIBUTE_ID = chip::app::Clusters::OnOff::Attributes::OnOff::Id;

    uint16_t m_endpoint_id;
    gpio_num_t m_button_pin;
    gpio_num_t m_led_pin;

public:
    LightAccessory(gpio_num_t button_pin, gpio_num_t led_pin);
    ~LightAccessory();
    esp_err_t setState(bool value);
    bool getState() const;
    void setEndpointId(uint16_t endpoint_id);
    esp_err_t attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                               esp_matter_attr_val_t *val) override;
    esp_err_t identification(uint16_t endpoint_id) override;

    static void callback(void *button_handle, void *usr_data);
};

#endif // ACCESSORY_LIGHT_HPP