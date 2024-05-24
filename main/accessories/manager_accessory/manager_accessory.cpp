#include "manager_accessory.hpp"
#include "accesspoint/url_handlers/helper_handler.hpp"

#include <cstdint>
#include <esp_log.h>

// #define JSMN_HEADER
#include <jsmn.h>

void Accessory::println()
{
    ESP_LOGI("ACCESSORY", "Name: %s", name);
    ESP_LOGI("ACCESSORY", "AID: %s", aid);
    switch (type) {
    case DOOR_TYPE:
        ESP_LOGI("ACCESSORY", "Type: DOOR");
        ESP_LOGI("ACCESSORY", "Button pin: %d", u_door.button_pin);
        ESP_LOGI("ACCESSORY", "Door pin: %d", u_door.door_pin);
        ESP_LOGI("ACCESSORY", "Opening time: %d", u_door.opening_time_sec);
        break;
    case FAN_TYPE:
        ESP_LOGI("ACCESSORY", "Type: FAN");
        ESP_LOGI("ACCESSORY", "Button pin: %d", u_fan.button_pin);
        ESP_LOGI("ACCESSORY", "Fan pin: %d", u_fan.fan_pin);
        break;
    case LIGHT_TYPE:
        ESP_LOGI("ACCESSORY", "Type: LIGHT");
        ESP_LOGI("ACCESSORY", "Button pin: %d", u_light.button_pin);
        ESP_LOGI("ACCESSORY", "LED pin: %d", u_light.led_pin);
        break;
    case PLUGIN_TYPE:
        ESP_LOGI("ACCESSORY", "Type: PLUGIN");
        ESP_LOGI("ACCESSORY", "Button pin: %d", u_plugin.button_pin);
        ESP_LOGI("ACCESSORY", "Plugin pin: %d", u_plugin.plugin_pin);
        break;
    case SWITCH_TYPE:
        ESP_LOGI("ACCESSORY", "Type: SWITCH");
        ESP_LOGI("ACCESSORY", "Button pin: %d", u_switch.button_pin);
        break;
    case WINDOW_type:
        ESP_LOGI("ACCESSORY", "Type: WINDOW");
        ESP_LOGI("ACCESSORY", "Button up pin: %d", u_window.button_up_pin);
        ESP_LOGI("ACCESSORY", "Button down pin: %d", u_window.button_down_pin);
        ESP_LOGI("ACCESSORY", "Motor up pin: %d", u_window.motor_up_pin);
        ESP_LOGI("ACCESSORY", "Motor down pin: %d", u_window.motor_down_pin);
        ESP_LOGI("ACCESSORY", "Opening time: %d", u_window.opening_time_sec);
        ESP_LOGI("ACCESSORY", "Closing time: %d", u_window.closing_time_sec);
        break;

    default:
        break;
    }
}

AccessoryType_t get_accessory_type(const char *type)
{
    if (strcmp(type, "DOOR") == 0) {
        return DOOR_TYPE;
    } else if (strcmp(type, "FAN") == 0) {
        return FAN_TYPE;
    } else if (strcmp(type, "LIGHT") == 0) {
        return LIGHT_TYPE;
    } else if (strcmp(type, "PLUGIN") == 0) {
        return PLUGIN_TYPE;
    } else if (strcmp(type, "SWITCH") == 0) {
        return SWITCH_TYPE;
    } else if (strcmp(type, "BLIND") == 0) {
        return WINDOW_type;
    } else if (strcmp(type, "WINDOW") == 0) {
        return WINDOW_type;
    }

    return LIGHT_TYPE;
}

esp_err_t fill_accessory(Accessory *accessory, const char *accessory_json, jsmntok_t *tokens, int ObjectIndex)
{
    if (accessory == NULL || accessory_json == NULL || tokens == NULL || ObjectIndex < 0) {
        ESP_LOGE("ACCESSORY", "One of the parameters is NULL");
        return ESP_FAIL;
    }
    if (tokens[ObjectIndex].type != JSMN_OBJECT) {
        ESP_LOGE("ACCESSORY", "ObjectIndex is not an object");
        return ESP_FAIL;
    }
    for (int i = 1; i <= tokens[ObjectIndex].size; i = i + 2) {
        jsmntok_t key = tokens[ObjectIndex + i];
        jsmntok_t value = tokens[ObjectIndex + i + 1];
        if (key.type != JSMN_STRING) {
            ESP_LOGE("ACCESSORY", "Key is not a string");
            return ESP_FAIL;
        }
        if (value.type != JSMN_STRING) {
            ESP_LOGE("ACCESSORY", "Value is not a string");
            return ESP_FAIL;
        }
    }
}