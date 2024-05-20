#include <iot_button.h>
#include "hal/gpio_types.h"

#include "checker.hpp"
#include "esp_matter_core.h"
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

/**
    Should report new value for :-
    - Cluster ID: 0x0006 (On/Off), Attribute ID: 0x0000 (OnOff)                     ON(1) / OFF(0)
    - Cluster ID: 0x0006 (On/Off), Attribute ID: 0x4000 (GlobalSceneControl)        ON(1) /
    - Cluster ID: 0x0006 (On/Off), Attribute ID: 0x4001 (OnTime)                          / OFF(0)
    - Cluster ID: 0x0006 (On/Off), Attribute ID: 0x4002 (OffWaitTime)               ON(0) /
    - Cluster ID: 0x0005 (Scenes), Attribute ID: 0x0003 (SceneValid)                ON(0) / OFF(0)
*/
void PluginAccessory::callback(void *button_handle, void *usr_data)
{
    PluginAccessory *self = static_cast<PluginAccessory *>(usr_data);
    self->setState(!self->getState());

    // get the value of the attribute OnOff
    esp_matter::node_t *node = esp_matter::node::get();
    esp_matter::endpoint_t *endpoint = esp_matter::endpoint::get(node, self->m_endpoint_id);
    esp_matter::cluster_t *cluster = esp_matter::cluster::get(endpoint, _CLUSTER_ID);
    esp_matter::attribute_t *attribute = esp_matter::attribute::get(cluster, _ATTRIBUTE_ID);
    esp_matter_attr_val_t onOff_val;
    esp_matter::attribute::get_val(attribute, &onOff_val);

    // get the value of the attribute GlobalSceneControl
    esp_matter::attribute_t *attribute_globalSceneControl = esp_matter::attribute::get(cluster, 0x4000);
    esp_matter_attr_val_t globalSceneControl_val;
    esp_matter::attribute::get_val(attribute_globalSceneControl, &globalSceneControl_val);

    // get the value of the attribute OnTime
    esp_matter::attribute_t *attribute_onTime = esp_matter::attribute::get(cluster, 0x4001);
    esp_matter_attr_val_t onTime_val;
    esp_matter::attribute::get_val(attribute_onTime, &onTime_val);

    // get the value of the attribute OffWaitTime
    esp_matter::attribute_t *attribute_offWaitTime = esp_matter::attribute::get(cluster, 0x4002);
    esp_matter_attr_val_t offWaitTime_val;
    esp_matter::attribute::get_val(attribute_offWaitTime, &offWaitTime_val);

    // get the value of the attribute SceneValid
    esp_matter::cluster_t *cluster_scene = esp_matter::cluster::get(endpoint, 0x0005);
    esp_matter::attribute_t *attribute_sceneValid = esp_matter::attribute::get(cluster_scene, 0x0003);
    esp_matter_attr_val_t sceneValid_val;
    esp_matter::attribute::get_val(attribute_sceneValid, &sceneValid_val);

    // Report new value
    if (self->getState()) {
        onOff_val.val.b = 1;
        globalSceneControl_val.val.b = 1;
        offWaitTime_val.val.u16 = 0;
        sceneValid_val.val.b = 0;
    } else {
        onOff_val.val.b = 0;
        onTime_val.val.u16 = 0;
        sceneValid_val.val.b = 0;
    }

    esp_matter::attribute::set_val(attribute, &onOff_val);
    esp_matter::attribute::set_val(attribute_globalSceneControl, &globalSceneControl_val);
    esp_matter::attribute::set_val(attribute_onTime, &onTime_val);
    esp_matter::attribute::set_val(attribute_offWaitTime, &offWaitTime_val);
    esp_matter::attribute::set_val(attribute_sceneValid, &sceneValid_val);

    esp_matter::attribute::report(self->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_ID, &onOff_val);
    esp_matter::attribute::report(self->m_endpoint_id, _CLUSTER_ID, 0x4000, &globalSceneControl_val);
    esp_matter::attribute::report(self->m_endpoint_id, _CLUSTER_ID, 0x4001, &onTime_val);
    esp_matter::attribute::report(self->m_endpoint_id, _CLUSTER_ID, 0x4002, &offWaitTime_val);
    esp_matter::attribute::report(self->m_endpoint_id, 0x0005, 0x0003, &sceneValid_val);
}

void PluginAccessory::setEndpointId(uint16_t endpoint_id)
{
    m_endpoint_id = endpoint_id;
}