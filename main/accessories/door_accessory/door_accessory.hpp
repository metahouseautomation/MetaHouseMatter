#ifndef DOOR_ACCESSORY_HPP
#define DOOR_ACCESSORY_HPP

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <cstdint>
#include <esp_matter_attribute_utils.h>
#include <esp_timer.h>
#include <hal/gpio_types.h>

#include "accessories/base_accessory/base_accessory.hpp"

class DoorAccessory : public BaseAccessory {
private:
    constexpr static const uint32_t _CLUSTER_ID = chip::app::Clusters::DoorLock::Id;
    constexpr static const uint32_t _ATTRIBUTE_LOCK_STATE_ID =
        chip::app::Clusters::DoorLock::Attributes::LockState::Id; // LockState::Id;

    uint16_t m_endpoint_id;
    gpio_num_t m_button_pin;
    gpio_num_t m_door_pin;
    uint8_t m_opening_time_sec;

    static const uint8_t _LOCK_STATE_LOCKED = 1;
    static const uint8_t _LOCK_STATE_UNLOCKED = 2;

    esp_timer_handle_t timer;

    void reportAttribute();

public:
    DoorAccessory(gpio_num_t button_pin, gpio_num_t door_pin, uint8_t opening_time_sec = 5);
    ~DoorAccessory();
    esp_err_t setState(uint8_t value);
    uint8_t getState() const;
    void setEndpointId(uint16_t endpoint_id);
    esp_err_t attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                               esp_matter_attr_val_t *val) override;
    esp_err_t identification(uint16_t endpoint_id) override;

    static void callback(void *button_handle, void *usr_data);

    static void delay_close_door(void *usr_data);
};

#endif // DOOR_ACCESSORY_HPP