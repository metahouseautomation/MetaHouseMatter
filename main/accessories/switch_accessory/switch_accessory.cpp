#include <esp_matter_attribute_utils.h>
#include <esp_matter_core.h>
#include <hal/gpio_types.h>
#include <iot_button.h>

#include "checker.hpp"
#include "esp_matter_event.h"
#include "accessories/base_accessory/base_accessory.hpp"
#include "accessories/switch_accessory/switch_accessory.hpp"

SwitchAccessory::SwitchAccessory(gpio_num_t button_pin)
    : BaseAccessory()
    , m_endpoint_id(0)
{
    this->m_button_pin = button_pin;

    gpio_set_direction(m_button_pin, GPIO_MODE_INPUT);
    button_config_t config = {.type = BUTTON_TYPE_GPIO,
                              .long_press_time = 2000,
                              .gpio_button_config = {
                                  .gpio_num = m_button_pin,
                                  .active_level = 1,
                              }};
    button_handle_t handle = iot_button_create(&config);

    iot_button_register_cb(handle, BUTTON_SINGLE_CLICK, single_press_release, this);
    iot_button_register_cb(handle, BUTTON_LONG_PRESS_START, long_press_release, this);
    iot_button_register_cb(handle, BUTTON_DOUBLE_CLICK, double_press_release, this);
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, on_press_down, this);
}

SwitchAccessory::~SwitchAccessory()
{
    gpio_set_direction(m_button_pin, GPIO_MODE_DISABLE);
}

esp_err_t SwitchAccessory::attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                                            esp_matter_attr_val_t *val)
{
    return ESP_OK;
}

esp_err_t SwitchAccessory::identification(uint16_t endpoint_id)
{
    return ESP_OK;
}

void SwitchAccessory::setEndpointId(uint16_t endpoint_id)
{
    m_endpoint_id = endpoint_id;
}

void SwitchAccessory::single_press_release(void *button_handle, void *usr_data)
{
    SwitchAccessory *accessory = static_cast<SwitchAccessory *>(usr_data);
    _CHECK_NULL_RETURN(accessory, "SwitchAccessory is null", );

    esp_matter_attr_val_t val = esp_matter_uint8(0);
    esp_matter::attribute::report(accessory->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_ID, &val);

    esp_matter::cluster::switch_cluster::event::send_multi_press_complete(accessory->m_endpoint_id, 0, 1);
}

void SwitchAccessory::long_press_release(void *button_handle, void *usr_data)
{
    SwitchAccessory *accessory = static_cast<SwitchAccessory *>(usr_data);
    _CHECK_NULL_RETURN(accessory, "SwitchAccessory is null", );

    esp_matter_attr_val_t val = esp_matter_uint8(0);
    esp_matter::attribute::report(accessory->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_ID, &val);

    esp_matter::cluster::switch_cluster::event::send_long_press(accessory->m_endpoint_id, 0);
}

void SwitchAccessory::double_press_release(void *button_handle, void *usr_data)
{
    SwitchAccessory *accessory = static_cast<SwitchAccessory *>(usr_data);
    _CHECK_NULL_RETURN(accessory, "SwitchAccessory is null", );

    esp_matter_attr_val_t val = esp_matter_uint8(0);
    esp_matter::attribute::report(accessory->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_ID, &val);

    esp_matter::cluster::switch_cluster::event::send_multi_press_complete(accessory->m_endpoint_id, 0, 2);
}

void SwitchAccessory::on_press_down(void *button_handle, void *usr_data)
{
    // SwitchAccessory *accessory = static_cast<SwitchAccessory *>(usr_data);
    // _CHECK_NULL_RETURN(accessory, "SwitchAccessory is null", );

    // esp_matter::node_t *node = esp_matter::node::get();
    // esp_matter::endpoint_t *endpoint = esp_matter::endpoint::get(node, accessory->m_endpoint_id);
    // esp_matter::cluster_t *cluster = esp_matter::cluster::get(endpoint, _CLUSTER_ID);
    // esp_matter::attribute_t *attribute = esp_matter::attribute::get(cluster, _ATTRIBUTE_ID);

    // esp_matter_attr_val_t val = esp_matter_uint8(1);
    // esp_matter::attribute::set_val(attribute, &val);
}