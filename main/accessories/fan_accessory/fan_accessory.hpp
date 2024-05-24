#ifndef FAN_ACCESSORY_HPP
#define FAN_ACCESSORY_HPP

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <cstdint>
#include <esp_matter_attribute_utils.h>
#include <hal/gpio_types.h>

#include "accessories/base_accessory/base_accessory.hpp"

class FanAccessory : public BaseAccessory {
private:
    constexpr static const uint32_t _CLUSTER_ID = chip::app::Clusters::FanControl::Id;
    constexpr static const uint32_t _ATTRIBUTE_PERCENT_SETTING_ID =
        chip::app::Clusters::FanControl::Attributes::PercentSetting::Id;
    constexpr static const uint32_t _ATTRIBUTE_PERCENT_CURRENT_ID =
        chip::app::Clusters::FanControl::Attributes::PercentCurrent::Id;
    constexpr static const uint32_t _ATTRIBUTE_FAN_MODE_ID = chip::app::Clusters::FanControl::Attributes::FanMode::Id;

    uint16_t m_endpoint_id;
    gpio_num_t m_button_pin;
    gpio_num_t m_fan_pin;

public:
    FanAccessory(gpio_num_t button_pin, gpio_num_t fan_pin);
    ~FanAccessory();
    esp_err_t setState(bool value);
    bool getState() const;
    void setEndpointId(uint16_t endpoint_id);
    esp_err_t attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                               esp_matter_attr_val_t *val) override;
    esp_err_t identification(uint16_t endpoint_id) override;

    static void callback(void *button_handle, void *usr_data);
};

#endif // FAN_ACCESSORY_HPP