#include "generic_switch.hpp"
#include "bridge_node.hpp"
#include "checker.hpp"

namespace metahouse::endpoint::generic_switch {
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

    esp_matter::cluster_t *switch_cluster = esp_matter::cluster::switch_cluster::create(
        endpoint, &(config->switch_cluster), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(switch_cluster, "Failed to create the switch cluster", nullptr);

    // Add Single press features to the switch cluster
    err = esp_matter::cluster::switch_cluster::feature::momentary_switch::add(switch_cluster);
    _CHECK_ERROR_RETURN(err, "Failed to add the momentary switch feature", nullptr);
    err = esp_matter::cluster::switch_cluster::feature::momentary_switch_release::add(switch_cluster);
    _CHECK_ERROR_RETURN(err, "Failed to add the momentary switch release feature", nullptr);

    // Add Long press features to the switch cluster
    err = esp_matter::cluster::switch_cluster::feature::momentary_switch_long_press::add(switch_cluster);
    _CHECK_ERROR_RETURN(err, "Failed to add the momentary switch long press feature", nullptr);

    // Add Multi press features to the switch cluster(Double press)
    esp_matter::cluster::switch_cluster::feature::momentary_switch_multi_press::config_t doublepress;
    err = esp_matter::cluster::switch_cluster::feature::momentary_switch_multi_press::add(switch_cluster, &doublepress);
    _CHECK_ERROR_RETURN(err, "Failed to add the momentary switch multi press feature", nullptr);

    return endpoint;
}
} // namespace metahouse::endpoint::generic_switch