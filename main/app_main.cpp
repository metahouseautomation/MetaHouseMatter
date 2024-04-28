#include <esp_err.h>
#include <esp_log.h>
#include <esp_matter_attribute_utils.h>
#include <esp_matter_core.h>
#include <esp_matter_identify.h>
#include <nvs_flash.h>

#include "aggregator.hpp"
#include "callback_event.hpp"
#include "checker.hpp"
#include "root_node.hpp"

#include "fan.hpp"
#include "on_off_light.hpp"

extern "C" void app_main()
{
    /* Initialize the ESP NVS layer */
    nvs_flash_init();

    /* Initialize the Matter Callbacks */
    esp_matter::attribute::set_callback(metahouse::callback_event::attribute_update);
    esp_matter::identification::set_callback(metahouse::callback_event::identification);

    /* Create a Matter root node on endpoint 0 */
    metahouse::endpoint::root_node::config_t root_node_config;
    esp_matter::node_t *root_node = metahouse::endpoint::root_node::create(&root_node_config);
    _CHECK_NULL_(root_node, "Failed to create root node");

    /* Create a Matter aggregator endpoint */
    metahouse::endpoint::aggregator::config_t aggregator_config;
    esp_matter::endpoint_t *aggregator = metahouse::endpoint::aggregator::create(root_node, &aggregator_config);
    _CHECK_NULL_(aggregator, "Failed to create aggregator");

    /* Create a Matter on/off light endpoint */
    metahouse::endpoint::on_off_light::config_t on_off_light_config;
    esp_matter::endpoint_t *on_off_light =
        metahouse::endpoint::on_off_light::create(root_node, &on_off_light_config, aggregator);
    _CHECK_NULL_(on_off_light, "Failed to create on/off light");

    /* Create a Matter fan endpoint */
    metahouse::endpoint::fan::config_t fan_config;
    esp_matter::endpoint_t *fan = metahouse::endpoint::fan::create(root_node, &fan_config, aggregator);
    _CHECK_NULL_(fan, "Failed to create fan");

    /* Start the Matter stack */
    esp_err_t err = esp_matter::start(metahouse::callback_event::event);
    _CHECK_ERROR_(err, "Matter start failed");
}