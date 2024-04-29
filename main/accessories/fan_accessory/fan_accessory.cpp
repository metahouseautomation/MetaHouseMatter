#include <iot_button.h>
#include "hal/gpio_types.h"

#include "checker.hpp"
#include "esp_log.h"
#include "esp_matter_attribute_utils.h"
#include "esp_matter_core.h"
#include "accessories/base_accessory/base_accessory.hpp"
#include "accessories/fan_accessory/fan_accessory.hpp"

FanAccessory::FanAccessory(gpio_num_t button_pin, gpio_num_t led_pin)
    : BaseAccessory()
    , m_endpoint_id(0)
{
    this->m_button_pin = button_pin;
    this->m_led_pin = led_pin;

    gpio_set_direction(m_led_pin, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_level(m_led_pin, 0);

    gpio_set_direction(m_button_pin, GPIO_MODE_INPUT);
    button_config_t config = {.type = BUTTON_TYPE_GPIO,
                              .gpio_button_config = {
                                  .gpio_num = m_button_pin,
                                  .active_level = 1,
                              }};
    button_handle_t handle = iot_button_create(&config);
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, callback, this);
}

FanAccessory::~FanAccessory()
{
    gpio_set_level(m_led_pin, 0);
    gpio_set_direction(m_led_pin, GPIO_MODE_DISABLE);
    gpio_set_direction(m_button_pin, GPIO_MODE_DISABLE);
}

esp_err_t FanAccessory::setState(bool value)
{
    gpio_set_level(m_led_pin, value);
    return ESP_OK;
}

bool FanAccessory::getState() const
{
    return gpio_get_level(m_led_pin);
}

esp_err_t FanAccessory::attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                                         esp_matter_attr_val_t *val)
{
    if (val == nullptr || cluster_id != _CLUSTER_ID ||
        (attribute_id != _ATTRIBUTE_PERCENT_SETTING_ID && attribute_id != _ATTRIBUTE_PERCENT_CURRENT_ID)) {
        return ESP_OK;
    }

    if (val->val.i > 0) {
        gpio_set_level(m_led_pin, 1);
    } else {
        gpio_set_level(m_led_pin, 0);
    }
    return ESP_OK;
}

esp_err_t FanAccessory::identification(uint16_t endpoint_id)
{
    return ESP_OK;
}

void FanAccessory::callback(void *button_handle, void *usr_data)
{
    FanAccessory *fanAccessory = static_cast<FanAccessory *>(usr_data);

    esp_matter_attr_val_t val_nullable;
    esp_matter_attr_val_t val;
    esp_matter_attr_val_t fanModeNewValue;
    if (fanAccessory->getState() > 0) {
        fanModeNewValue = esp_matter_enum8(0);
        val_nullable = esp_matter_nullable_uint8(0);
        val = esp_matter_uint8(0);
        fanAccessory->setState(0);
    } else {
        fanModeNewValue = esp_matter_enum8(3);
        val_nullable = esp_matter_nullable_uint8(1);
        val = esp_matter_uint8(1);
        fanAccessory->setState(1);
    }

    esp_matter::attribute::report(fanAccessory->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_PERCENT_SETTING_ID,
                                  &val_nullable);
    esp_matter::attribute::report(fanAccessory->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_PERCENT_CURRENT_ID, &val);
    esp_matter::attribute::report(fanAccessory->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_FAN_MODE_ID, &fanModeNewValue);
}

void FanAccessory::setEndpointId(uint16_t endpoint_id)
{
    m_endpoint_id = endpoint_id;
}
