#include "window_covering.hpp"
#include "bridge_node.hpp"
#include "checker.hpp"

namespace metahouse::endpoint::window_covering {
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator)
{
    _CHECK_NULL_RETURN(node, "Node is null", nullptr);
    esp_matter::endpoint_t *endpoint;
    // If the aggregator is not null, create a bridged endpoint
    if (aggregator != nullptr) {
        endpoint = metahouse::endpoint::bridge_node::create_bridged_endpoint(node, aggregator);
        _CHECK_NULL_RETURN(endpoint, "Failed to create the bridged endpoint", nullptr);

        esp_matter::cluster_t *bridged_device_basic_information_cluster =
            esp_matter::cluster::bridged_device_basic_information::create(
                endpoint, &(config->bridged_device_basic_information), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        _CHECK_NULL_RETURN(bridged_device_basic_information_cluster,
                           "Failed to create the bridged device basic information cluster", nullptr);
    } else { // If the aggregator is null, create a standalone endpoint
        endpoint = esp_matter::endpoint::create(node, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, nullptr);
        _CHECK_NULL_RETURN(endpoint, "Failed to create the endpoint", nullptr);
    }

    esp_err_t err = esp_matter::endpoint::add_device_type(endpoint, DEVICE_TYPE_ID, DEVICE_TYPE_VERSION);
    _CHECK_ERROR_RETURN(err, "Failed to add the device type", nullptr);

    esp_matter::cluster_t *descriptor_cluster = esp_matter::cluster::descriptor::create(
        endpoint, &(config->descriptor), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(descriptor_cluster, "Failed to create the descriptor cluster", nullptr);

    esp_matter::cluster_t *identify_cluster = esp_matter::cluster::identify::create(
        endpoint, &(config->identify), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(identify_cluster, "Failed to create the identify cluster", nullptr);
    esp_matter::command_t *trigger_effect_command =
        esp_matter::cluster::identify::command::create_trigger_effect(identify_cluster);
    _CHECK_NULL_RETURN(trigger_effect_command, "Failed to create the trigger effect command", nullptr);

    esp_matter::cluster_t *groups_cluster = esp_matter::cluster::groups::create(
        endpoint, &(config->groups), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(groups_cluster, "Failed to create the groups cluster", nullptr);

    esp_matter::cluster_t *scenes_cluster = esp_matter::cluster::scenes::create(
        endpoint, &(config->scenes), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(scenes_cluster, "Failed to create the scenes cluster", nullptr);

    esp_matter::cluster_t *window_covering_cluster = esp_matter::cluster::window_covering::create(
        endpoint, &(config->window_covering), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER,
        ESP_MATTER_NONE_FEATURE_ID);
    _CHECK_NULL_RETURN(window_covering_cluster, "Failed to create the window covering cluster", nullptr);

    // Add the lift feature to the window covering cluster
    esp_matter::cluster::window_covering::feature::lift::config_t lift_config;
    err = esp_matter::cluster::window_covering::feature::lift::add(window_covering_cluster, &lift_config);
    _CHECK_ERROR_RETURN(err, "Failed to add the lift feature", nullptr);

    // Add the position aware lift feature to the window covering cluster
    esp_matter::cluster::window_covering::feature::position_aware_lift::config_t position_aware_lift_config;
    nullable<uint8_t> percentage = nullable<uint8_t>(0);
    nullable<uint16_t> percentage_100ths = nullable<uint16_t>(0);
    position_aware_lift_config.current_position_lift_percentage = percentage;
    position_aware_lift_config.target_position_lift_percent_100ths = percentage_100ths;
    position_aware_lift_config.current_position_lift_percent_100ths = percentage_100ths;
    err = esp_matter::cluster::window_covering::feature::position_aware_lift::add(window_covering_cluster,
                                                                                  &position_aware_lift_config);
    _CHECK_ERROR_RETURN(err, "Failed to add the position aware lift feature", nullptr);

    // Add the absolute position feature to the window covering cluster
    esp_matter::cluster::window_covering::feature::absolute_position::config_t absolute_position_config;
    err = esp_matter::cluster::window_covering::feature::absolute_position::add(window_covering_cluster,
                                                                                &absolute_position_config);
    _CHECK_ERROR_RETURN(err, "Failed to add the absolute position feature", nullptr);

    return endpoint;
}
} // namespace metahouse::endpoint::window_covering