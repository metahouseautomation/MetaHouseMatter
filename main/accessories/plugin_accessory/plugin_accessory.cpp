#include <iot_button.h>
#include "hal/gpio_types.h"

#include "checker.hpp"
#include "accessories/base_accessory/base_accessory.hpp"
#include "accessories/plugin_accessory/plugin_accessory.hpp"

PluginAccessory::PluginAccessory(gpio_num_t button_pin, gpio_num_t led_pin)
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

PluginAccessory::~PluginAccessory()
{
    gpio_set_level(m_led_pin, 0);
    gpio_set_direction(m_led_pin, GPIO_MODE_DISABLE);
    gpio_set_direction(m_button_pin, GPIO_MODE_DISABLE);
}

esp_err_t PluginAccessory::setState(bool value)
{
    gpio_set_level(m_led_pin, value);
    return ESP_OK;
}

bool PluginAccessory::getState() const
{
    return gpio_get_level(m_led_pin);
}

esp_err_t PluginAccessory::attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                                            esp_matter_attr_val_t *val)
{
    if (val == nullptr || m_endpoint_id != endpoint_id || cluster_id != _CLUSTER_ID || attribute_id != _ATTRIBUTE_ID) {
        return ESP_OK;
    }

    setState(val->val.b);
    return ESP_OK;
}

esp_err_t PluginAccessory::identification(uint16_t endpoint_id)
{
    return ESP_OK;
}

void PluginAccessory::callback(void *button_handle, void *usr_data)
{
    PluginAccessory *self = static_cast<PluginAccessory *>(usr_data);
    self->setState(!self->getState());
}

void PluginAccessory::setEndpointId(uint16_t endpoint_id)
{
    m_endpoint_id = endpoint_id;
}