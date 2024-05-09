// TODO: Check All Endpoints cluster groups and scenes requirements

#include <cstring>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_matter_attribute_utils.h>
#include <esp_matter_core.h>
#include <esp_matter_identify.h>
#include <nvs_flash.h>

#include "callback_event.hpp"
#include "checker.hpp"
#include "root_node.hpp"
#include "endpoints/aggregator/aggregator.hpp"

// #include "endpoints/door_lock/door_lock.hpp"
#include "fan.hpp"
#include "generic_switch.hpp"
#include "on_off_light.hpp"
#include "on_off_plugin.hpp"
#include "endpoints/window_covering/window_covering.hpp"

#include "accessories/door_accessory/door_accessory.hpp"
#include "accessories/fan_accessory/fan_accessory.hpp"
#include "accessories/light_accessory/light_accessory.hpp"
#include "accessories/plugin_accessory/plugin_accessory.hpp"
#include "accessories/switch_accessory/switch_accessory.hpp"
#include "accessories/window_accessory/window_accessory.hpp"
#include "endpoints/door_lock/door_lock.hpp"

#include "accesspoint/webserver/webserver.hpp"

extern "C" void app_main()
{
    gpio_num_t buttonPins[] = {gpio_num_t(34), gpio_num_t(35), gpio_num_t(32), gpio_num_t(33),
                               gpio_num_t(25), gpio_num_t(26), gpio_num_t(27), gpio_num_t(14)};
    // gpio_num_t relayPins[] = {gpio_num_t(23), gpio_num_t(22), gpio_num_t(21), gpio_num_t(19),
    //                           gpio_num_t(18), gpio_num_t(17), gpio_num_t(16), gpio_num_t(4)};
    gpio_num_t relayPins[] = {gpio_num_t(4),  gpio_num_t(16), gpio_num_t(17), gpio_num_t(18),
                              gpio_num_t(19), gpio_num_t(21), gpio_num_t(22), gpio_num_t(23)};

    /* Initialize the ESP NVS layer */
    nvs_flash_init();

    /* Initialize the Matter Callbacks */
    esp_matter::attribute::set_callback(metahouse::callback_event::attribute_update);
    esp_matter::identification::set_callback(metahouse::callback_event::identification);
    metahouse::callback_event::enable_factory_reset(GPIO_NUM_5, 5);

    /* Create a Matter root node on endpoint 0 */
    metahouse::endpoint::root_node::config_t root_node_config;
    esp_matter::node_t *root_node = metahouse::endpoint::root_node::create(&root_node_config);
    _CHECK_NULL_(root_node, "Failed to create root node");

    /* Create a Matter aggregator endpoint */
    metahouse::endpoint::aggregator::config_t aggregator_config;
    esp_matter::endpoint_t *aggregator = metahouse::endpoint::aggregator::create(root_node, &aggregator_config);
    _CHECK_NULL_(aggregator, "Failed to create aggregator");

    for (int counterI = 0; counterI < 5; counterI++) {
        /* Create a Matter on/off light endpoint */
        LightAccessory *lightAccessory = new LightAccessory(buttonPins[counterI], relayPins[counterI]);
        metahouse::endpoint::on_off_light::config_t *on_off_light_config =
            new metahouse::endpoint::on_off_light::config_t;
        char name[30] = "Bulb ";
        char i_str[2] = {0};
        i_str[0] = (char)(counterI + '0');
        strcat(name, i_str);
        strncpy(on_off_light_config->bridged_device_basic_information.node_label, name,
                metahouse::clusters::bridged_device_basic_information::MAX_NAME_LENGTH);

        esp_matter::endpoint_t *on_off_light =
            metahouse::endpoint::on_off_light::create(root_node, on_off_light_config, aggregator, lightAccessory);
        _CHECK_NULL_(on_off_light, "Failed to create on/off light");
    }

    /* Create a Matter fan endpoint */
    for (int counterI = 5; counterI < 7; counterI++) {
        FanAccessory *fanAccessory = new FanAccessory(buttonPins[counterI], relayPins[counterI]);
        metahouse::endpoint::fan::config_t fan_config;
        fan_config.fan_control.fan_mode_sequence = 5;
        esp_matter::endpoint_t *fan =
            metahouse::endpoint::fan::create(root_node, &fan_config, aggregator, fanAccessory);
        _CHECK_NULL_(fan, "Failed to create fan");
    }

    /* Create a Matter door lock endpoint */
    DoorAccessory *doorAccessory = new DoorAccessory(buttonPins[7], relayPins[7]);
    _CHECK_NULL_(doorAccessory, "Failed to create door accessory");
    metahouse::endpoint::door_lock::config_t door_lock_config;
    esp_matter::endpoint_t *door_lock =
        metahouse::endpoint::door_lock::create(root_node, &door_lock_config, aggregator, doorAccessory);
    _CHECK_NULL_(door_lock, "Failed to create door lock");

    /* Start the Matter stack */
    esp_err_t err = esp_matter::start(metahouse::callback_event::event);
    _CHECK_ERROR_(err, "Matter start failed");

    ESP_LOGI("app_main", "Matter started successfully");
}