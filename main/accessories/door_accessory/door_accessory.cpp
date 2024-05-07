#include <esp_matter_attribute_utils.h>
#include <esp_timer.h>
#include <hal/gpio_types.h>
#include <iot_button.h>

#include "checker.hpp"
#include "esp_matter_core.h"
#include "accessories/base_accessory/base_accessory.hpp"
#include "accessories/door_accessory/door_accessory.hpp"

DoorAccessory::DoorAccessory(gpio_num_t button_pin, gpio_num_t door_pin, uint8_t opening_time_sec)
    : BaseAccessory()
    , m_endpoint_id(0)
{
    this->m_button_pin = button_pin;
    this->m_door_pin = door_pin;
    this->m_opening_time_sec = opening_time_sec;

    gpio_set_direction(m_door_pin, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_level(m_door_pin, 0);

    gpio_set_direction(m_button_pin, GPIO_MODE_INPUT);
    button_config_t config = {.type = BUTTON_TYPE_GPIO,
                              .gpio_button_config = {
                                  .gpio_num = m_button_pin,
                                  .active_level = 1,
                              }};
    button_handle_t handle = iot_button_create(&config);
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, callback, this);
}

DoorAccessory::~DoorAccessory()
{
    gpio_set_level(m_door_pin, 0);
    gpio_set_direction(m_door_pin, GPIO_MODE_DISABLE);
    gpio_set_direction(m_button_pin, GPIO_MODE_DISABLE);
}

esp_err_t DoorAccessory::setState(uint8_t value)
{
    bool newState = value == _LOCK_STATE_UNLOCKED;

    ESP_LOGE("SET_STATE", "Door state: %d", newState);

    gpio_set_level(m_door_pin, newState);
    if (newState) {
        esp_timer_create_args_t timer_args = {
            .callback = delay_close_door,
            .arg = (void *)this,
            .dispatch_method = ESP_TIMER_TASK,
            .name = "delay_close_door",
        };
        esp_timer_handle_t timer;
        esp_timer_create(&timer_args, &timer);
        esp_timer_start_once(timer, m_opening_time_sec * 1000000);
    }
    return ESP_OK;
}

uint8_t DoorAccessory::getState() const
{
    ESP_LOGE("GET_STATE", "Door state: %d", gpio_get_level(m_door_pin));

    if (gpio_get_level(m_door_pin) == 1) {
        return _LOCK_STATE_UNLOCKED;
    }
    return _LOCK_STATE_LOCKED;
}

esp_err_t DoorAccessory::attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                                          esp_matter_attr_val_t *val)
{
    if (val == nullptr || cluster_id != _CLUSTER_ID || attribute_id != _ATTRIBUTE_LOCK_STATE_ID) {
        return ESP_OK;
    }

    ESP_LOGE("ATTRIBUTE_UPDATE", "Door state: %d", val->val.u8);

    return setState(val->val.u8);
}

esp_err_t DoorAccessory::identification(uint16_t endpoint_id)
{
    return ESP_OK;
}

void DoorAccessory::callback(void *button_handle, void *usr_data)
{
    DoorAccessory *self = static_cast<DoorAccessory *>(usr_data);

    esp_matter::node_t *node = esp_matter::node::get();
    esp_matter::endpoint_t *endpoint = esp_matter::endpoint::get(node, self->m_endpoint_id);
    esp_matter::cluster_t *cluster = esp_matter::cluster::get(endpoint, _CLUSTER_ID);
    esp_matter::attribute_t *attribute = esp_matter::attribute::get(cluster, _ATTRIBUTE_LOCK_STATE_ID);
    esp_matter_attr_val_t val;
    esp_matter::attribute::get_val(attribute, &val);
    val.val.u8 = _LOCK_STATE_LOCKED;

    if (self->getState() == _LOCK_STATE_LOCKED) {
        val.val.u8 = _LOCK_STATE_UNLOCKED;
    } else {
        val.val.u8 = _LOCK_STATE_LOCKED;
    }
    self->setState(val.val.u8);

    esp_matter::attribute::report(self->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_LOCK_STATE_ID, &val);
}

void DoorAccessory::delay_close_door(void *usr_data)
{
    ESP_LOGE("DELAY_CLOSE_DOOR", "Closing door");

    DoorAccessory *self = static_cast<DoorAccessory *>(usr_data);
    self->setState(_LOCK_STATE_LOCKED);

    esp_matter::node_t *node = esp_matter::node::get();
    esp_matter::endpoint_t *endpoint = esp_matter::endpoint::get(node, self->m_endpoint_id);
    esp_matter::cluster_t *cluster = esp_matter::cluster::get(endpoint, _CLUSTER_ID);
    esp_matter::attribute_t *attribute = esp_matter::attribute::get(cluster, _ATTRIBUTE_LOCK_STATE_ID);
    esp_matter_attr_val_t val;
    esp_matter::attribute::get_val(attribute, &val);
    val.val.u8 = _LOCK_STATE_LOCKED;
    esp_matter::attribute::report(self->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_LOCK_STATE_ID, &val);
}

void DoorAccessory::setEndpointId(uint16_t endpoint_id)
{
    m_endpoint_id = endpoint_id;
}
