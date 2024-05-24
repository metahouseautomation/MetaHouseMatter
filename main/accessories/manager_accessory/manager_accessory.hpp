#ifndef MANAGER_ACCESSORY_HPP
#define MANAGER_ACCESSORY_HPP

#include <esp_err.h>
#include <hal/gpio_types.h>
#include <jsmn.h>

enum AccessoryType_t { DOOR_TYPE, FAN_TYPE, LIGHT_TYPE, PLUGIN_TYPE, SWITCH_TYPE, WINDOW_type };

struct Accessory {
    char name[50];
    char aid[10];
    AccessoryType_t type;
    union {
        struct {
            gpio_num_t button_pin;
            gpio_num_t door_pin;
            uint8_t opening_time_sec;
        } u_door;
        struct {
            gpio_num_t button_pin;
            gpio_num_t fan_pin;
        } u_fan;
        struct {
            gpio_num_t button_pin;
            gpio_num_t led_pin;
        } u_light;
        struct {
            gpio_num_t button_pin;
            gpio_num_t plugin_pin;
        } u_plugin;
        struct {
            gpio_num_t button_pin;
        } u_switch;
        struct {
            gpio_num_t button_up_pin;
            gpio_num_t button_down_pin;
            gpio_num_t motor_up_pin;
            gpio_num_t motor_down_pin;
            uint8_t opening_time_sec;
            uint8_t closing_time_sec;
        } u_window;
    };
    void println();
};

/**
    * @brief Initialize accessories from database
    *
    * @return esp_err_t

    * @note This function is used to initialize the accessories from the database.
*/
esp_err_t init_accessories_from_db();

/**
    * @brief Get accessory type
    *
    * @param type
    *
    * @return AccessoryType_t

    * @note This function is used to get the accessory type.
*/
AccessoryType_t get_accessory_type(const char *type);

/**
    * @brief Fill accessory
    *
    * @param accessory
    * @param accessory_json
    * @param tokens
    * @param ObjectIndex
    *
    * @return esp_err_t

    * @note This function is used to fill the accessory structure.
*/
esp_err_t fill_accessory(Accessory *accessory, const char *accessory_json, jsmntok_t *tokens, int ObjectIndex);

/**
    * @brief Create accessory
    *
    * @param accessory
    *
    * @return esp_err_t

    * @note This function is used to create an accessory.
*/
esp_err_t create_accessory(Accessory *accessory);

#endif // MANAGER_ACCESSORY_HPP