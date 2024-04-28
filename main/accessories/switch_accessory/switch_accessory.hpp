#ifndef SWITCH_ACCESSORY_HPP
#define SWITCH_ACCESSORY_HPP

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <cstdint>
#include <esp_matter_attribute_utils.h>
#include <fcntl.h>
#include <hal/gpio_types.h>

#include "accessories/base_accessory/base_accessory.hpp"

class SwitchAccessory : public BaseAccessory {
private:
    constexpr static const uint32_t _CLUSTER_ID = chip::app::Clusters::Switch::Id;
    constexpr static const uint32_t _ATTRIBUTE_ID = chip::app::Clusters::Switch::Attributes::CurrentPosition::Id;

    uint16_t m_endpoint_id;
    gpio_num_t m_button_pin;

    static void single_press_release(void *button_handle, void *usr_data);
    static void long_press_release(void *button_handle, void *usr_data);
    static void double_press_release(void *button_handle, void *usr_data);
    static void on_press_down(void *button_handle, void *usr_data);

public:
    SwitchAccessory(gpio_num_t button_pin);
    ~SwitchAccessory();
    void setEndpointId(uint16_t endpoint_id);
    esp_err_t attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                               esp_matter_attr_val_t *val) override;
    esp_err_t identification(uint16_t endpoint_id) override;
};

#endif // SWITCH_ACCESSORY_HPP