#include <iot_button.h>
#include "freertos/FreeRTOSConfig_arch.h"
#include "hal/gpio_types.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define INCLUDE_uxTaskGetStackHighWaterMark 1 // TaskHandle_t
#define INCLUDE_vTaskDelete 1 // vTaskDelete

#include "checker.hpp"
#include "esp_log.h"
#include "esp_matter_attribute_utils.h"
#include "esp_matter_core.h"
#include <accessories/window_accessory/window_accessory.hpp>
#include "accessories/base_accessory/base_accessory.hpp"

WindowAccessory::WindowAccessory(gpio_num_t button_up_pin, gpio_num_t button_down_pin, gpio_num_t motor_up_pin,
                                 gpio_num_t motor_down_pin)
    : BaseAccessory()
    , m_endpoint_id(0)
{
    _blindPosition = 0;
    _timeToOpen = 10;
    _timeToClose = 10;
    _targetPostion = 0;

    _checkInterval = 100;
    _moveBlindToTask_handle = nullptr;
    handle_up = nullptr;
    handle_down = nullptr;

    this->m_button_up_pin = button_up_pin;
    this->m_button_down_pin = button_down_pin;
    this->m_motor_up_pin = motor_up_pin;
    this->m_motor_down_pin = motor_down_pin;

    gpio_set_direction(m_motor_up_pin, GPIO_MODE_OUTPUT);
    gpio_set_direction(m_motor_down_pin, GPIO_MODE_OUTPUT);

    gpio_set_direction(m_button_up_pin, GPIO_MODE_INPUT);
    gpio_set_direction(m_button_down_pin, GPIO_MODE_INPUT);

    button_config_t config_up = {.type = BUTTON_TYPE_GPIO,
                                 .gpio_button_config = {
                                     .gpio_num = m_button_up_pin,
                                     .active_level = 1,
                                 }};
    handle_up = iot_button_create(&config_up);
    iot_button_register_cb(handle_up, BUTTON_PRESS_DOWN, callback_up, this);

    button_config_t config_down = {.type = BUTTON_TYPE_GPIO,
                                   .gpio_button_config = {
                                       .gpio_num = m_button_down_pin,
                                       .active_level = 1,
                                   }};
    handle_down = iot_button_create(&config_down);
    iot_button_register_cb(handle_down, BUTTON_PRESS_DOWN, callback_down, this);
}

WindowAccessory::~WindowAccessory()
{
    gpio_set_level(m_motor_up_pin, 0);
    gpio_set_level(m_motor_down_pin, 0);
    gpio_set_direction(m_motor_up_pin, GPIO_MODE_DISABLE);
    gpio_set_direction(m_motor_down_pin, GPIO_MODE_DISABLE);
    gpio_set_direction(m_button_up_pin, GPIO_MODE_DISABLE);
    gpio_set_direction(m_button_down_pin, GPIO_MODE_DISABLE);
    iot_button_delete(handle_down);
    iot_button_delete(handle_up);

    if (_moveBlindToTask_handle != nullptr) {
        vTaskDelete(_moveBlindToTask_handle);
        _moveBlindToTask_handle = nullptr;
    }
}

void WindowAccessory::callback_up(void *button_handle, void *usr_data)
{
    WindowAccessory *windowAccessory = static_cast<WindowAccessory *>(usr_data);
    if (windowAccessory->_blindPosition != windowAccessory->_targetPostion) {
        windowAccessory->moveBlindTo(windowAccessory->_blindPosition);
    } else {
        windowAccessory->moveBlindTo(100);
    }

    windowAccessory->reportCurrentPosition();
    windowAccessory->reportTargetPosition();
}

void WindowAccessory::callback_down(void *button_handle, void *usr_data)
{
    WindowAccessory *windowAccessory = static_cast<WindowAccessory *>(usr_data);
    if (windowAccessory->_blindPosition != windowAccessory->_targetPostion) {
        windowAccessory->moveBlindTo(windowAccessory->_blindPosition);
    } else {
        windowAccessory->moveBlindTo(0);
    }

    windowAccessory->reportCurrentPosition();
    windowAccessory->reportTargetPosition();
}

void WindowAccessory::setEndpointId(uint16_t endpoint_id)
{
    m_endpoint_id = endpoint_id;

    reportCurrentPosition();
    reportTargetPosition();
}

esp_err_t WindowAccessory::attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                                            esp_matter_attr_val_t *val)
{
    if (val == nullptr || cluster_id != _CLUSTER_ID ||
        (attribute_id != _ATTRIBUTE_TARGET_ID && attribute_id != _ATTRIBUTE_CURRENT_ID)) {
        return ESP_OK;
    }

    if (attribute_id == _ATTRIBUTE_TARGET_ID) {
        _targetPostion = val->val.u16 / 100;
        moveBlindTo(_targetPostion);
    }

    return ESP_OK;
}

esp_err_t WindowAccessory::identification(uint16_t endpoint_id)
{
    return ESP_OK;
}

void WindowAccessory::moveBlindTo(uint8_t position)
{
    if (position >= 100) {
        position = 100;
    }
    if (position <= 0) {
        position = 0;
    }

    if (_moveBlindToTask_handle != nullptr) {
        vTaskDelete(_moveBlindToTask_handle);
        _moveBlindToTask_handle = nullptr;
    }

    xTaskCreate(
        [](void *pParamter) {
            WindowAccessory *windowAccessory = static_cast<WindowAccessory *>(pParamter);
            windowAccessory->moveBlindToTargetTask();
            windowAccessory->_moveBlindToTask_handle = nullptr;
            vTaskDelete(nullptr);
        },
        "moveBlindToTargetTask", 10000, this, configMAX_PRIORITIES - 1, &_moveBlindToTask_handle);
}

void WindowAccessory::start_move_blind(MoveDirection direction)
{
    if (direction == MoveDirection::UP) {
        gpio_set_level(m_motor_up_pin, 1);
        gpio_set_level(m_motor_down_pin, 0);
    } else {
        gpio_set_level(m_motor_up_pin, 0);
        gpio_set_level(m_motor_down_pin, 1);
    }
    timer = esp_timer_get_time();
}

void WindowAccessory::stop_move_blind()
{
    gpio_set_level(m_motor_up_pin, 0);
    gpio_set_level(m_motor_down_pin, 0);
    ESP_LOGE("WindowAccessory", "Time taken to move blind: %f", ((esp_timer_get_time() - timer) / 1000000));
}

bool WindowAccessory::targetPositionReached(MoveDirection direction)
{
    if (direction == MoveDirection::UP) {
        if (_blindPosition >= _targetPostion) {
            return true;
        }
    } else {
        if (_blindPosition <= _targetPostion) {
            return true;
        }
    }

    return false;
}

void WindowAccessory::moveBlindToTargetTask()
{
    double currentPosition = _blindPosition;

    // Ensure target position is within valid range
    if (_targetPostion < 0)
        _targetPostion = 0;
    else if (_targetPostion > 100)
        _targetPostion = 100;

    // Return if the blind is already at the target position
    if (_targetPostion == _blindPosition) {
        stop_move_blind();
        return;
    }

    MoveDirection direction = (_targetPostion > _blindPosition) ? MoveDirection::UP : MoveDirection::DOWN;
    start_move_blind(direction);

    uint8_t startPostion = _blindPosition;
    bool updateCallback = true;

    while (!targetPositionReached(direction)) {
        // Delay to check the position at regular intervals
        vTaskDelay(pdMS_TO_TICKS(_checkInterval));

        // Update the current position based on the movement direction
        currentPosition += (direction == MoveDirection::UP) ? (double)_checkInterval / (_timeToOpen * 1000) * 100
                                                            : -(double)_checkInterval / (_timeToClose * 1000) * 100;
        _blindPosition = (uint8_t)currentPosition;

        // Call update callback after 1% change in position
        if ((startPostion - _blindPosition) != 0 && updateCallback) {
            updateCallback = false;
            reportCurrentPosition();
            reportTargetPosition();
        }
    }

    // Stop the motor after reaching the target position
    stop_move_blind();

    // Update the current position to the target position
    _blindPosition = _targetPostion;

    // Report the final position
    reportCurrentPosition();
    reportTargetPosition();
}

void WindowAccessory::reportCurrentPosition()
{
    esp_matter_attr_val_t val = esp_matter_nullable_uint16(_blindPosition * 100);
    esp_matter::attribute::report(m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_CURRENT_ID, &val);
}

void WindowAccessory::reportTargetPosition()
{
    esp_matter_attr_val_t val = esp_matter_nullable_uint16(_targetPostion * 100);
    esp_matter::attribute::report(m_endpoint_id, _CLUSTER_ID, _ATTRIBUTE_TARGET_ID, &val);
}
