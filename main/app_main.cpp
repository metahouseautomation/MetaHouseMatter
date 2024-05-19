// TODO: Check All Endpoints cluster groups and scenes requirements

#include <cstdint>
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

#include "accessories/plugin_accessory/plugin_accessory.hpp"
#include "endpoints/on_off_plugin/on_off_plugin.hpp"

extern "C" void app_main()
{
    gpio_num_t buttonPins[] = {gpio_num_t(34), gpio_num_t(35), gpio_num_t(32), gpio_num_t(33),
                               gpio_num_t(25), gpio_num_t(26), gpio_num_t(27), gpio_num_t(14)};

    gpio_num_t relayPins[] = {gpio_num_t(4),  gpio_num_t(16), gpio_num_t(17), gpio_num_t(18),
                              gpio_num_t(19), gpio_num_t(21), gpio_num_t(22), gpio_num_t(23)};

    /* Initialize the ESP NVS layer */
    nvs_flash_init();

    /* Initialize the Matter Callbacks */
    esp_matter::attribute::set_callback(metahouse::callback_event::attribute_update);
    esp_matter::identification::set_callback(metahouse::callback_event::identification);
    metahouse::callback_event::enable_factory_reset(GPIO_NUM_5, 5);
    metahouse::callback_event::enable_restart_device(GPIO_NUM_5);

    /* Create a Matter root node on endpoint 0 */
    metahouse::endpoint::root_node::config_t root_node_config;
    esp_matter::node_t *root_node = metahouse::endpoint::root_node::create(&root_node_config);
    _CHECK_NULL_(root_node, "Failed to create root node");

    /* Create a Matter aggregator endpoint */
    metahouse::endpoint::aggregator::config_t aggregator_config;
    esp_matter::endpoint_t *aggregator = metahouse::endpoint::aggregator::create(root_node, &aggregator_config);
    _CHECK_NULL_(aggregator, "Failed to create aggregator");

    /* Create a Matter plugin */
    PluginAccessory *pluginAccessories = new PluginAccessory(buttonPins[0], relayPins[0]);
    metahouse::endpoint::on_off_plugin::config_t on_off_plugin_config;
    esp_matter::endpoint_t *on_off_plugin =
        metahouse::endpoint::on_off_plugin::create(root_node, &on_off_plugin_config, aggregator, pluginAccessories);
    _CHECK_NULL_(on_off_plugin, "Failed to create on/off plugin");

    /* Start the Matter stack */
    esp_err_t err = esp_matter::start(metahouse::callback_event::event);
    _CHECK_ERROR_(err, "Matter start failed");

    ESP_LOGI("app_main", "Matter started successfully");
}