#include "on_off_plugin.hpp"
#include "checker.hpp"
#include "accessories/plugin_accessory/plugin_accessory.hpp"
#include "endpoints/bridge_node/bridge_node.hpp"

namespace metahouse::endpoint::on_off_plugin {
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator,
                               PluginAccessory *priv_data)
{
    _CHECK_NULL_RETURN(node, "Node is null", nullptr);
    esp_matter::endpoint_t *endpoint;
    // If the aggregator is not null, create a bridged endpoint
    if (aggregator != nullptr) {
        endpoint = metahouse::endpoint::bridge_node::create_bridged_endpoint(node, aggregator, priv_data);
        _CHECK_NULL_RETURN(endpoint, "Failed to create the bridged endpoint", nullptr);

        esp_matter::cluster_t *bridged_device_basic_information_cluster =
            esp_matter::cluster::bridged_device_basic_information::create(
                endpoint, &(config->bridged_device_basic_information), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        _CHECK_NULL_RETURN(bridged_device_basic_information_cluster,
                           "Failed to create the bridged device basic information cluster", nullptr);
    } else { // If the aggregator is null, create a standalone endpoint
        endpoint = esp_matter::endpoint::create(node, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, priv_data);
        _CHECK_NULL_RETURN(endpoint, "Failed to create the endpoint", nullptr);
    }

    priv_data->setEndpointId(esp_matter::endpoint::get_id(endpoint));

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

    esp_matter::cluster_t *on_off_cluster =
        esp_matter::cluster::on_off::create(endpoint, &(config->on_off), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER,
                                            esp_matter::cluster::on_off::feature::lighting::get_id());
    _CHECK_NULL_RETURN(on_off_cluster, "Failed to create the on/off cluster", nullptr);

    return endpoint;
}
} // namespace metahouse::endpoint::on_off_plugin