#ifndef WINDOW_ACCESSORY_HPP
#define WINDOW_ACCESSORY_HPP

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <cstdint>
#include <esp_matter_attribute_utils.h>
#include <hal/gpio_types.h>
#include <iot_button.h>

#include "accessories/base_accessory/base_accessory.hpp"
#include "arch/sys_arch.h"

class WindowAccessory : public BaseAccessory {
private:
    constexpr static const uint32_t _CLUSTER_ID = chip::app::Clusters::WindowCovering::Id;
    constexpr static const uint32_t _ATTRIBUTE_TARGET_ID =
        chip::app::Clusters::WindowCovering::Attributes::TargetPositionLiftPercent100ths::Id;
    constexpr static const uint32_t _ATTRIBUTE_CURRENT_ID =
        chip::app::Clusters::WindowCovering::Attributes::CurrentPositionLiftPercent100ths::Id;

    uint16_t m_endpoint_id;

    gpio_num_t m_button_up_pin;
    gpio_num_t m_button_down_pin;
    button_handle_t handle_up;
    button_handle_t handle_down;

    gpio_num_t m_motor_up_pin;
    gpio_num_t m_motor_down_pin;

    uint8_t _blindPosition; ///< Current position of the blind. 0-100.
    uint8_t _timeToOpen; ///< Time to fully open the blind, in seconds.
    uint8_t _timeToClose; ///< Time to fully close the blind, in seconds.

    uint8_t _targetPostion; ///< Target position for the blind. 0-100.

    TaskHandle_t _moveBlindToTask_handle; ///< Task handle for the moveBlindToTask.
    uint8_t _checkInterval; ///< Interval to check the postion when the blind moving. in ms.

    enum class MoveDirection { UP, DOWN };

    void start_move_blind(MoveDirection direction); //< Start moving the blind up/down.
    void stop_move_blind(); ///< Stop moving the blind.

    bool targetPositionReached(MoveDirection direction); ///< Check if the blind has reached the target position.

    void moveBlindToTargetTask(); ///< Task to move the blind to a specific position.

    void reportCurrentPosition(); ///< Report the current position of the blind.
    void reportTargetPosition(); ///< Report the target position of the blind.

public:
    WindowAccessory(gpio_num_t button_up_pin, gpio_num_t button_down_pin, gpio_num_t motor_up_pin,
                    gpio_num_t motor_down_pin);
    ~WindowAccessory();
    void setEndpointId(uint16_t endpoint_id);
    esp_err_t attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                               esp_matter_attr_val_t *val) override;
    esp_err_t identification(uint16_t endpoint_id) override;

    static void callback_up(void *button_handle, void *usr_data);
    static void callback_down(void *button_handle, void *usr_data);

    /**
     * @brief Move the blind to a specific position.
     *
     * @param position The target position for the blind.
     */
    void moveBlindTo(uint8_t position);

    float timer = 0;
};

#endif // WINDOW_ACCESSORY_HPP