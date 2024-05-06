#include <esp_matter_attribute_utils.h>
#include <esp_timer.h>
#include <hal/gpio_types.h>
#include <iot_button.h>

#include "checker.hpp"
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

esp_err_t DoorAccessory::setState(bool value)
{
    gpio_set_level(m_door_pin, value);
    if (value) {
        esp_timer_create_args_t timer_args = {
            .callback = delay_close_door,
            .arg = (void *)m_door_pin,
            .dispatch_method = ESP_TIMER_TASK,
            .name = "delay_close_door",
        };
        esp_timer_handle_t timer;
        esp_timer_create(&timer_args, &timer);
        esp_timer_start_once(timer, m_opening_time_sec * 1000000);
    }
    return ESP_OK;
}

bool DoorAccessory::getState() const
{
    return gpio_get_level(m_door_pin);
}

esp_err_t DoorAccessory::attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                                          esp_matter_attr_val_t *val)
{
    if (val == nullptr || cluster_id != _CLUSTER_ID || attribute_id != _ATTRIBUTE_LOCK_STATE_ID) {
        return ESP_OK;
    }

    return setState(val->val.u8 == 1);
}

esp_err_t DoorAccessory::identification(uint16_t endpoint_id)
{
    return ESP_OK;
}

void DoorAccessory::callback(void *button_handle, void *usr_data)
{
    DoorAccessory *self = static_cast<DoorAccessory *>(usr_data);
    self->setState(!self->getState());

    esp_matter_attr_val_t val;
    if (self->getState()) {
        val = esp_matter_enum8(1);
    } else {
        val = esp_matter_enum8(0);
    }
    esp_matter::attribute::report(self->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_LOCK_STATE_ID, &val);
}

void DoorAccessory::delay_close_door(void *usr_data)
{
    DoorAccessory *self = static_cast<DoorAccessory *>(usr_data);
    self->setState(false);

    esp_matter_attr_val_t val = esp_matter_enum8(0);
    esp_matter::attribute::report(self->m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_LOCK_STATE_ID, &val);
}

void DoorAccessory::setEndpointId(uint16_t endpoint_id)
{
    m_endpoint_id = endpoint_id;
}
